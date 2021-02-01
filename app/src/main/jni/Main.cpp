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

#define targetLibName OBFUSCATE("(yourTargetLibName)")

extern "C" {
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
