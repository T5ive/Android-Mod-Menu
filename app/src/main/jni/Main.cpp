#include <list>
#include <vector>
#include <string.h>
#include <pthread.h>
#include <cstring>
#include <jni.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <dlfcn.h>
#include "Includes/Logger.h"
#include "Includes/obfuscate.h"
#include "Includes/Utils.h"
#include "KittyMemory/MemoryPatch.h"
#include "Menu.h"

#define targetLibName OBFUSCATE("(yourTargetLibName)")
#include "Includes/Macros.h"

struct My_Patches {
    //VariableHere
} hexPatches;

//NewVariableHere

//NewMethodHere

void *hack_thread(void *) {
    LOGI(OBFUSCATE("pthread created"));
    do {
        sleep(1);
    } while (!isLibraryLoaded(targetLibName));

    LOGI(OBFUSCATE("%s has been loaded"), (const char *) targetLibName);

#if defined(__aarch64__)
    //(hackThread64)
#else
    //(hackThread)
#endif

    return NULL;
}

extern "C" {

JNIEXPORT jobjectArray
JNICALL
Java_com_tfive_modmenu_FloatingModMenuService_getFeatureList(JNIEnv *env, jobject context) {
    jobjectArray ret;

    //ToastHere

    const char *features[] = {//(yourFeaturesList)
            OBFUSCATE("RichWebView_<html><body><marquee style=\"color: white; font-weight:bold;\" direction=\"left\" scrollamount=\"5\" behavior=\"scroll\">"
                      "(yourEndCredit)"
                      "</marquee></body></html>")
    };

    int Total_Feature = (sizeof features / sizeof features[0]);
    ret = (jobjectArray)
            env->NewObjectArray(Total_Feature, env->FindClass(OBFUSCATE("java/lang/String")),
                                env->NewStringUTF(""));

    for (int i = 0; i < Total_Feature; i++)
        env->SetObjectArrayElement(ret, i, env->NewStringUTF(features[i]));

    pthread_t ptid;
    pthread_create(&ptid, NULL, antiLeech, NULL);

    return (ret);
}

JNIEXPORT void JNICALL
Java_com_tfive_modmenu_Preferences_Changes(JNIEnv *env, jclass clazz, jobject obj,
                                        jint featNum, jstring featName, jint value,
                                        jboolean boolean, jstring str) {

    LOGD(OBFUSCATE("Feature name: %d - %s | Value: = %d | Bool: = %d | Text: = %s"), featNum,
         env->GetStringUTFChars(featName, 0), value,
         boolean, str != NULL ? env->GetStringUTFChars(str, 0) : "");

    switch (featNum) {
        //(yourFeatures)
    }
}
}

__attribute__((constructor))
void lib_main() {
    pthread_t ptid;
    pthread_create(&ptid, NULL, hack_thread, NULL);
}