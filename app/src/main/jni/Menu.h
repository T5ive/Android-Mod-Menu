bool titleValid, headingValid, iconValid, settingsValid, isLeeched;

void *antiLeech(void *) {
    sleep(20);

    if (!titleValid || !headingValid || !iconValid || !settingsValid) {
        int *p = 0;
        *p = 0;
    }
    return NULL;
}

void setText(JNIEnv *env, jobject obj, const char* text){
    //https://stackoverflow.com/a/33627640/3763113
    //A little JNI calls here. You really really need a great knowledge if you want to play with JNI stuff
    //Html.fromHtml("");
    jclass html = (*env).FindClass(OBFUSCATE("android/text/Html"));
    jmethodID fromHtml = (*env).GetStaticMethodID(html, OBFUSCATE("fromHtml"), OBFUSCATE("(Ljava/lang/String;)Landroid/text/Spanned;"));

    //setText("");
    jclass textView = (*env).FindClass(OBFUSCATE("android/widget/TextView"));
    jmethodID setText = (*env).GetMethodID(textView, OBFUSCATE("setText"), OBFUSCATE("(Ljava/lang/CharSequence;)V"));

    //Java string
    jstring jstr = (*env).NewStringUTF(text);
    (*env).CallVoidMethod(obj, setText,  (*env).CallStaticObjectMethod(html, fromHtml, jstr));
}

extern "C" {
JNIEXPORT void JNICALL
Java_com_tfive_modmenu_FloatingModMenuService_setTitleText(JNIEnv *env, jobject thiz, jobject obj) {
    setText(env, obj, OBFUSCATE("<b>Modded by TFive</b>"));

    titleValid = true;
}

JNIEXPORT void JNICALL
Java_com_tfive_modmenu_FloatingModMenuService_setHeadingText(JNIEnv *env, jobject thiz, jobject obj) {
    setText(env, obj, OBFUSCATE("<b><marquee><p style=\"font-size:30\">"
                                      "<p style=\"color:green;\">Modded by TFive</p> | "
                                      "https://github.com/T5ive | Available for free</p>"
                                      "</marquee></b>"));

    headingValid = true;
}

JNIEXPORT jstring JNICALL
Java_com_tfive_modmenu_FloatingModMenuService_Icon(JNIEnv *env, jobject thiz) {
    iconValid = true;

    //Use https://www.base64encode.org/ to encode your image to base64
    return env->NewStringUTF(
            OBFUSCATE("iVBORw0KGgoAAAANSUhEUgAAAJYAAACWCAMAAAAL34HQAAAABGdBTUEAALGPC/xhBQAAAAFzUkdCAK7OHOkAAAcwaVRYdFhNTDpjb20uYWRvYmUueG1wAAAAAAA8P3hwYWNrZXQgYmVnaW49Iu+7vyIgaWQ9Ilc1TTBNcENlaGlIenJlU3pOVGN6a2M5ZCI/PiA8eDp4bXBtZXRhIHhtbG5zOng9ImFkb2JlOm5zOm1ldGEvIiB4OnhtcHRrPSJBZG9iZSBYTVAgQ29yZSA1LjYtYzE0MiA3OS4xNjA5MjQsIDIwMTcvMDcvMTMtMDE6MDY6MzkgICAgICAgICI+IDxyZGY6UkRGIHhtbG5zOnJkZj0iaHR0cDovL3d3dy53My5vcmcvMTk5OS8wMi8yMi1yZGYtc3ludGF4LW5zIyI+IDxyZGY6RGVzY3JpcHRpb24gcmRmOmFib3V0PSIiIHhtbG5zOnhtcD0iaHR0cDovL25zLmFkb2JlLmNvbS94YXAvMS4wLyIgeG1sbnM6eG1wTU09Imh0dHA6Ly9ucy5hZG9iZS5jb20veGFwLzEuMC9tbS8iIHhtbG5zOnN0RXZ0PSJodHRwOi8vbnMuYWRvYmUuY29tL3hhcC8xLjAvc1R5cGUvUmVzb3VyY2VFdmVudCMiIHhtbG5zOnBob3Rvc2hvcD0iaHR0cDovL25zLmFkb2JlLmNvbS9waG90b3Nob3AvMS4wLyIgeG1sbnM6ZGM9Imh0dHA6Ly9wdXJsLm9yZy9kYy9lbGVtZW50cy8xLjEvIiB4bXA6Q3JlYXRvclRvb2w9IkFkb2JlIFBob3Rvc2hvcCBDQyAoV2luZG93cykiIHhtcDpDcmVhdGVEYXRlPSIyMDE4LTA4LTA4VDE5OjU4OjIyKzA3OjAwIiB4bXA6TWV0YWRhdGFEYXRlPSIyMDE4LTA4LTA4VDIwOjA1OjQ1KzA3OjAwIiB4bXA6TW9kaWZ5RGF0ZT0iMjAxOC0wOC0wOFQyMDowNTo0NSswNzowMCIgeG1wTU06SW5zdGFuY2VJRD0ieG1wLmlpZDpjZTA3MzQzNC1jNjI2LTk1NGYtYmQwNy02MzQ3ZGQ5YWE2OTMiIHhtcE1NOkRvY3VtZW50SUQ9ImFkb2JlOmRvY2lkOnBob3Rvc2hvcDo3ZjM1Yzc5OC1hNTM4LTU1NGMtODYxMC01NjRhNWE0M2I4YzYiIHhtcE1NOk9yaWdpbmFsRG9jdW1lbnRJRD0ieG1wLmRpZDozMTY0NDEwZi1iZGQ5LWE4NGQtOTM0Ny0wOTA0NWY2ODcyMjkiIHBob3Rvc2hvcDpDb2xvck1vZGU9IjMiIHBob3Rvc2hvcDpJQ0NQcm9maWxlPSIqIHdzUkdCIiBkYzpmb3JtYXQ9ImltYWdlL3BuZyI+IDx4bXBNTTpIaXN0b3J5PiA8cmRmOlNlcT4gPHJkZjpsaSBzdEV2dDphY3Rpb249ImNyZWF0ZWQiIHN0RXZ0Omluc3RhbmNlSUQ9InhtcC5paWQ6MzE2NDQxMGYtYmRkOS1hODRkLTkzNDctMDkwNDVmNjg3MjI5IiBzdEV2dDp3aGVuPSIyMDE4LTA4LTA4VDE5OjU4OjIyKzA3OjAwIiBzdEV2dDpzb2Z0d2FyZUFnZW50PSJBZG9iZSBQaG90b3Nob3AgQ0MgKFdpbmRvd3MpIi8+IDxyZGY6bGkgc3RFdnQ6YWN0aW9uPSJzYXZlZCIgc3RFdnQ6aW5zdGFuY2VJRD0ieG1wLmlpZDo2NDBhZTk0Yi05MjcyLWMxNGYtYWJkMy1mYWQwYThiNTBiYjUiIHN0RXZ0OndoZW49IjIwMTgtMDgtMDhUMTk6NTg6MjIrMDc6MDAiIHN0RXZ0OnNvZnR3YXJlQWdlbnQ9IkFkb2JlIFBob3Rvc2hvcCBDQyAoV2luZG93cykiIHN0RXZ0OmNoYW5nZWQ9Ii8iLz4gPHJkZjpsaSBzdEV2dDphY3Rpb249InNhdmVkIiBzdEV2dDppbnN0YW5jZUlEPSJ4bXAuaWlkOmNlMDczNDM0LWM2MjYtOTU0Zi1iZDA3LTYzNDdkZDlhYTY5MyIgc3RFdnQ6d2hlbj0iMjAxOC0wOC0wOFQyMDowNTo0NSswNzowMCIgc3RFdnQ6c29mdHdhcmVBZ2VudD0iQWRvYmUgUGhvdG9zaG9wIENDIChXaW5kb3dzKSIgc3RFdnQ6Y2hhbmdlZD0iLyIvPiA8L3JkZjpTZXE+IDwveG1wTU06SGlzdG9yeT4gPHBob3Rvc2hvcDpEb2N1bWVudEFuY2VzdG9ycz4gPHJkZjpCYWc+IDxyZGY6bGk+QzgxOUI2Njg3NkE2NDA2N0M5QzhDRUM4OTk2ODBGNkE8L3Jk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"));
}

JNIEXPORT jstring JNICALL
Java_com_tfive_modmenu_FloatingModMenuService_IconWebViewData(JNIEnv *env, jobject thiz) {
    iconValid = true;
    //WebView support GIF animation. Upload your image or GIF on imgur.com or other sites

    // From internet (Requires android.permission.INTERNET)
    // return env->NewStringUTF(OBFUSCATE_KEY("https://i.imgur.com/SujJ85j.gif", 'd'));

    // From assets folder: (Requires android.permission.INTERNET)
    // return env->NewStringUTF(OBFUSCATE_KEY("file:///android_asset/example.gif", 'r');

    // Base64 html:
    // return env->NewStringUTF("data:image/png;base64, <encoded base64 here>");

    // To disable it, return NULL. It will use normal image above:
    // return NULL

    return env->NewStringUTF(OBFUSCATE_KEY("https://i.imgur.com/qfKEHya.png", 'u'));
    return NULL;
}

JNIEXPORT jobjectArray JNICALL
Java_com_tfive_modmenu_FloatingModMenuService_settingsList(JNIEnv *env, jobject activityObject) {
    jobjectArray ret;

    const char *features[] = {
            OBFUSCATE("Category_Settings"),
            OBFUSCATE("-1_Toggle_Save feature preferences"), //-1 is checked on Preferences.java
            OBFUSCATE("-3_Toggle_Auto size vertically"),
            OBFUSCATE("Category_Logcat"),
            OBFUSCATE("RichTextView_Save logcat if a bug occured and sent it to the modder. Clear logcat and reproduce bug again if the log file is too large"),
            OBFUSCATE("RichTextView_<small>Saving logcat does not need file permission. Logcat location:"
                            "<br/>Android 11: /storage/emulated/0/Documents/"
                            "<br/>Android 10 and below: /storage/emulated/0/Android/data/(package name)/files/Mod Menu</small>"),
            OBFUSCATE("-4_Button_Save logcat to file"),
            OBFUSCATE("-5_Button_Clear logcat"),
            OBFUSCATE("Category_Menu"),
            OBFUSCATE("-6_Button_<font color='red'>Close settings</font>"),
    };

    int Total_Feature = (sizeof features /
                         sizeof features[0]); //Now you dont have to manually update the number everytime;
    ret = (jobjectArray)
            env->NewObjectArray(Total_Feature, env->FindClass(OBFUSCATE("java/lang/String")),
                                env->NewStringUTF(""));
    int i;
    for (i = 0; i < Total_Feature; i++)
        env->SetObjectArrayElement(ret, i, env->NewStringUTF(features[i]));

    settingsValid = true;

    return (ret);
}
}
