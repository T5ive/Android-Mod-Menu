#include <list>
#include <vector>
#include <string.h>
#include <pthread.h>
#include <cstring>
#include <jni.h>
#include <unistd.h>
#include <fstream>
#include "Includes/obfuscate.h"
#include "KittyMemory/MemoryPatch.h"
#include "Includes/Logger.h"
#include "Includes/Utils.h"
#include "Menu.h"
#include "Toast.h"

#if defined(__aarch64__)
#include <And64InlineHook/And64InlineHook.hpp>
#else
#include <Substrate/SubstrateHook.h>
#include <Substrate/CydiaSubstrate.h>
#endif

struct My_Patches {
    //VariableHere
} hexPatches;

//NewVariableHere

//NewMethodHere

#define targetLibName OBFUSCATE("(yourTargetLibName)")

extern "C" {
JNIEXPORT jobjectArray
JNICALL
Java_com_tfive_modmenu_FloatingModMenuService_settingsList(JNIEnv *env, jobject activityObject) {
    jobjectArray ret;

    const char *features[] = {
            OBFUSCATE("0_Category_Settings"),
            OBFUSCATE("-1_Toggle_Color animation"),
            OBFUSCATE("-2_Toggle_Auto size vertically"),
            OBFUSCATE("-3_Toggle_Save feature preferences (Radio Button is not saved)"),
            OBFUSCATE("0_Category_Logcat"),
            OBFUSCATE("0_RichTextView_Save logcat if a bug occured and sent it to the modder. Clear logcat and reproduce bug again if the log file is too large"),
            OBFUSCATE("0_RichTextView_<small>Saving logcat does not need file permission. Logcat location:<br/>Android 11: /storage/emulated/0/Documents/Mod Menu logs/"
                      "<br/>Android 10 and below: /storage/emulated/0/Android/data/(package name)/files/Mod Menu logs</small>"),
            OBFUSCATE("-4_Button_Save logcat to file"),
            OBFUSCATE("-5_Button_Clear logcat"),
            OBFUSCATE("0_Category_Menu"),
            OBFUSCATE("-6_Button_<font color='red'>Close settings</font>"),
            //(TFiveEndCredit)
    };

    int Total_Feature = (sizeof features /
                         sizeof features[0]);
    ret = (jobjectArray)
            env->NewObjectArray(Total_Feature, env->FindClass(OBFUSCATE("java/lang/String")),
                                env->NewStringUTF(""));
    int i;
    for (i = 0; i < Total_Feature; i++)
        env->SetObjectArrayElement(ret, i, env->NewStringUTF(features[i]));

    return (ret);
}

JNIEXPORT void JNICALL
Java_com_tfive_modmenu_Preferences_Changes(JNIEnv *env, jclass clazz, jobject obj,
                                        jint feature, jint value, jboolean boolean, jstring str) {

    const char *featureName = env->GetStringUTFChars(str, 0);

    LOGD(OBFUSCATE("Feature name: %d - %s | Value: = %d | Bool: = %d"), feature, featureName, value,
         boolean);

    switch (feature) {
          //(yourFeatures)
    }
}
}

void *hack_thread(void *) {
    LOGI(OBFUSCATE("pthread called"));

    do {
        sleep(1);
    } while (!isLibraryLoaded(targetLibName));

    LOGI(OBFUSCATE("%s has been loaded"), (const char *) targetLibName);

#if defined(__aarch64__)
    //(hackThread64)
    LOGI(OBFUSCATE("Done"));
#else
    //(hackThread)
    LOGI(OBFUSCATE("Done"));
#endif

    return NULL;
}

__attribute__((constructor))
void lib_main() {
    pthread_t ptid;
    pthread_create(&ptid, NULL, hack_thread, NULL);
}
