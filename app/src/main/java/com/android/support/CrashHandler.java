package com.android.support;

import android.content.Context;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.os.Build;
import android.util.Log;
import android.widget.Toast;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.PrintWriter;
import java.io.StringWriter;
import java.lang.Thread.UncaughtExceptionHandler;
import java.nio.charset.StandardCharsets;
import java.text.SimpleDateFormat;
import java.util.Arrays;
import java.util.Date;
import java.util.Locale;
import java.util.Map;

public final class CrashHandler {

    private static final String TAG = "AppCrash";
    private static final String LOG_DIR = "crash_logs";
    private static final String FILE_PREFIX = "crash_";
    private static final String FILE_EXTENSION = ".txt";
    private static UncaughtExceptionHandler defaultHandler;
    private static Context appContext;
    final static boolean showToasts = true;

    /// Due to changes in access to application folders and to maintain convenience, the path has been changed to: android/media/PACKAGE/files/LOG_DIR.
    /// This path should be accessible on all devices
    public static void init(final Context context) {
        appContext = context.getApplicationContext();
        defaultHandler = Thread.getDefaultUncaughtExceptionHandler();

        cleanOldLogs();

        Thread.setDefaultUncaughtExceptionHandler(new UncaughtExceptionHandler() {
            @Override
            public void uncaughtException(Thread thread, Throwable throwable) {
                Log.e(TAG, "Uncaught exception detected", throwable);

                try {
                    handleException(thread, throwable);
                } catch (Exception e) {
                    Log.e(TAG, "Error in crash handler", e);
                    if (defaultHandler != null) {
                        defaultHandler.uncaughtException(thread, throwable);
                    } else {
                        android.os.Process.killProcess(android.os.Process.myPid());
                        System.exit(10);
                    }
                }
            }
        });
    }

    private static void handleException(Thread thread, Throwable throwable) {
        // Save crash log
        File crashFile = saveCrashLog(throwable);

        // Show notification to user
        if (showToasts && appContext != null) {
            showCrashNotification(crashFile);
        }

        // Wait a bit for toast to show and log to be saved
        try {
            Thread.sleep(2000);
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }

        // Call default handler
        if (defaultHandler != null) {
            defaultHandler.uncaughtException(thread, throwable);
        } else {
            android.os.Process.killProcess(android.os.Process.myPid());
            System.exit(10);
        }
    }

    private static File saveCrashLog(Throwable throwable) {
        String timeStamp = new SimpleDateFormat("yyyy_MM_dd-HH_mm_ss", Locale.US)
                .format(new Date());

        String fileName = FILE_PREFIX + timeStamp + FILE_EXTENSION;
        File logFile = new File(getLogDirectory(), fileName);

        try {
            writeCrashLog(logFile, throwable);
            Log.i(TAG, "Crash log saved to: " + logFile.getAbsolutePath());
            return logFile;
        } catch (IOException e) {
            Log.e(TAG, "Failed to save crash log", e);
            return null;
        }
    }

    private static void writeCrashLog(File file, Throwable throwable) throws IOException {
        File parentDir = file.getParentFile();
        if (parentDir != null && !parentDir.exists()) {
            if (!parentDir.mkdirs()) {
                Log.w(TAG, "Failed to create directory: " + parentDir.getAbsolutePath());
            }
        }

        String logContent = buildCrashReport(throwable);

        try (FileOutputStream fos = new FileOutputStream(file)) {
            fos.write(logContent.getBytes(StandardCharsets.UTF_8));
            fos.flush();
        }
    }

    private static String buildCrashReport(Throwable throwable) {
        StringBuilder report = new StringBuilder();

        // App and device info
        report.append("=============== CRASH REPORT ===============\n");
        report.append("Time: ").append(getCurrentTime()).append("\n");
        report.append("App Version: ").append(getAppVersion()).append("\n");
        report.append("Android API: ").append(Build.VERSION.SDK_INT).append("\n");
        report.append("Android Version: ").append(Build.VERSION.RELEASE).append("\n");
        report.append("Device: ").append(Build.MANUFACTURER).append(" ").append(Build.MODEL).append("\n");
        report.append("Product: ").append(Build.PRODUCT).append("\n");
        report.append("Board: ").append(Build.BOARD).append("\n");
        report.append("Fingerprint: ").append(Build.FINGERPRINT).append("\n");
        report.append("Thread: ").append(Thread.currentThread().getName()).append("\n");
        report.append("=============== STACK TRACE ===============\n");

        // Stack trace
        StringWriter sw = new StringWriter();
        PrintWriter pw = new PrintWriter(sw);
        throwable.printStackTrace(pw);
        report.append(sw.toString());

        // Cause chain
        Throwable cause = throwable.getCause();
        int depth = 0;
        while (cause != null && depth < 10) {
            report.append("\nCaused by: ");
            cause.printStackTrace(pw);
            cause = cause.getCause();
            depth++;
        }

        // Thread dump
        report.append("\n\n=============== ALL THREADS ===============\n");
        Map<Thread, StackTraceElement[]> allThreads = Thread.getAllStackTraces();
        for (Map.Entry<Thread, StackTraceElement[]> entry : allThreads.entrySet()) {
            Thread thread = entry.getKey();
            report.append("\nThread: ").append(thread.getName())
                    .append(" [ID: ").append(thread.getId())
                    .append(", State: ").append(thread.getState())
                    .append(", Priority: ").append(thread.getPriority())
                    .append("]\n");

            for (StackTraceElement element : entry.getValue()) {
                report.append("    at ").append(element.toString()).append("\n");
            }
        }

        pw.close();
        return report.toString();
    }

    private static File getLogDirectory() {
        File standardAppDir = appContext.getExternalFilesDir(null);
        if (standardAppDir == null) {
            Log.e(TAG, "Cannot get app directory, using internal storage");
            return new File(appContext.getFilesDir(), LOG_DIR);
        }

        String standardPath = standardAppDir.getAbsolutePath();
        String targetPath = standardPath.replace(
                "Android/data/" + appContext.getPackageName(),
                "Android/media/" + appContext.getPackageName()
        );
        File logDir = new File(targetPath, LOG_DIR);

        if (!logDir.exists()) {
            if (logDir.mkdirs()) {
                Log.i(TAG, "Created log directory: " + logDir.getAbsolutePath());
            } else {
                Log.e(TAG, "Failed to create Android/media directory, falling back to standard location");
                return new File(standardAppDir, LOG_DIR);
            }
        }
        return logDir;
    }

    private static String getDisplayPath(File file) {
        if (file == null) return null;
        String absolutePath = file.getAbsolutePath();
        int androidIndex = absolutePath.indexOf("/Android/");
        if (androidIndex != -1) {
            return absolutePath.substring(androidIndex + 1);
        }
        return file.getName();
    }

    private static void showCrashNotification(File crashFile) {
        String message;

        if (crashFile != null && crashFile.exists()) {
            String shortPath = getDisplayPath(crashFile);
            message = "CRASH! check:\n" + shortPath;
        } else {
            message = "CRASH! Could not save log.";
        }

        Toast.makeText(appContext, message, Toast.LENGTH_LONG).show();
    }

    private static String getAppVersion() {
        if (appContext == null) return "unknown";

        try {
            PackageInfo pInfo = appContext.getPackageManager()
                    .getPackageInfo(appContext.getPackageName(), 0);
            long versionCode = Build.VERSION.SDK_INT >= Build.VERSION_CODES.P ?
                    pInfo.getLongVersionCode() : pInfo.versionCode;
            return pInfo.versionName + " (" + versionCode + ")";
        } catch (PackageManager.NameNotFoundException e) {
            return "unknown";
        }
    }

    private static String getCurrentTime() {
        return new SimpleDateFormat("yyyy-MM-dd HH:mm:ss Z", Locale.US)
                .format(new Date());
    }

    /// utility method to manually log an exception
    public static void logException(Throwable throwable) {
        Log.e(TAG, "Manual exception logging", throwable);
        saveCrashLog(throwable);
    }

    /// clear old crash logs (keep last 5)
    public static void cleanOldLogs() {
        File logDir = getLogDirectory();
        if (!logDir.exists() || !logDir.isDirectory()) return;

        File[] logs = logDir.listFiles((dir, name) ->
                name.startsWith(FILE_PREFIX) && name.endsWith(FILE_EXTENSION));

        if (logs != null && logs.length > 5) {
            Arrays.sort(logs, (f1, f2) ->
                    Long.compare(f2.lastModified(), f1.lastModified()));

            for (int i = 10; i < logs.length; i++) {
                if (!logs[i].delete()) {
                    Log.w(TAG, "Failed to delete old log: " + logs[i].getName());
                }
            }
        }
    }
}