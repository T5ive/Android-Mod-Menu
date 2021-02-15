/*
 * Credits:
 *
 * Octowolve - Mod menu: https://github.com/z3r0Sec/Substrate-Template-With-Mod-Menu
 * And hooking: https://github.com/z3r0Sec/Substrate-Hooking-Example
 * VanHoevenTR A.K.A Nixi: https://github.com/LGLTeam/VanHoevenTR_Android_Mod_Menu
 * MrIkso - Mod menu: https://github.com/MrIkso/FloatingModMenu
 * Rprop - https://github.com/Rprop/And64InlineHook
 * MJx0 A.K.A Ruit - KittyMemory: https://github.com/MJx0/KittyMemory
 * */
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

#if defined(__aarch64__) //Compile for arm64 lib only
#include <And64InlineHook/And64InlineHook.hpp>
#else //Compile for armv7 lib only. Do not worry about greyed out highlighting code, it still works

#include <Substrate/SubstrateHook.h>
#include <Substrate/CydiaSubstrate.h>

#endif

struct My_Patches {
    MemoryPatch _OneHitKill, _OneHitKill2, _HpHero, _Diamond;
} hexPatches;

bool _oneHitKill = false;
bool _isAdFree = false;
int _setGoldValue = 1;
int _setExpValue = 100;

bool _setMP = false;
int _setMpValue = 100;

void *btnInstance;

void (*SetGold)(void *instance, long amount, bool alwaysTrue);
void (*SetExp)(void *instance, int amount);
void (*LvlUp)(void *instance);

//Target lib here
#define targetLibName OBFUSCATE("libil2cpp.so")

extern "C" {
JNIEXPORT jobjectArray
JNICALL
Java_com_tfive_modmenu_FloatingModMenuService_getFeatureList(JNIEnv *env, jobject activityObject) {
    jobjectArray ret;

    const char *features[] = {
            OBFUSCATE("0_Category_The Category"),
            OBFUSCATE("1_Toggle_1 Hit Kill"), // ใช้ได้แล้ว
            OBFUSCATE("2_SeekBar_HP_1_3"), // เอาออก ไม่เวิร์ค
            OBFUSCATE("3_SeekBar_Gold_1000_100000"), // ใช้ได้แล้ว
            OBFUSCATE("4_Button_Set Gold"),
            OBFUSCATE("5_Button_Set Exp 100"),
            OBFUSCATE("6_ButtonOnOff_Ad Free"), // คาดว่าใช้ได้แล้ว
            OBFUSCATE("7_Button_LevelUp"),
            OBFUSCATE("8_SeekBar_MP_100_500"), // ใช้ได้แล้ว
            OBFUSCATE("9_Toggle_MP") // ใช้ได้แล้ว
    };

    int Total_Feature = (sizeof features /
                         sizeof features[0]); //Now you dont have to manually update the number everytime;
    ret = (jobjectArray)
            env->NewObjectArray(Total_Feature, env->FindClass(OBFUSCATE("java/lang/String")),
                                env->NewStringUTF(""));
    int i;
    for (i = 0; i < Total_Feature; i++)
        env->SetObjectArrayElement(ret, i, env->NewStringUTF(features[i]));

    pthread_t ptid;
    pthread_create(&ptid, NULL, antiLeech, NULL);

    return (ret);
}


JNIEXPORT void JNICALL
Java_com_tfive_modmenu_Preferences_Changes(JNIEnv *env, jclass clazz, jobject obj,
                                        jint feature, jint value, jboolean boolean, jstring str) {

    const char *featureName = env->GetStringUTFChars(str, 0);

    LOGD(OBFUSCATE("Feature name: %d - %s | Value: = %d | Bool: = %d"), feature, featureName, value,
         boolean);

    switch (feature) {
        case 1:
            _oneHitKill = boolean;
            if (_oneHitKill) {
                hexPatches._OneHitKill.Modify();
                hexPatches._OneHitKill2.Modify();
                LOGI(OBFUSCATE("On"));
            } else {
                hexPatches._OneHitKill.Restore();
                hexPatches._OneHitKill2.Restore();
                LOGI(OBFUSCATE("Off"));
            }
            break;
        case 3:
            if (value >= 1) {
                 _setGoldValue = value;
            }
            break;
        case 4:
            LOGD(OBFUSCATE("Set Gold"));
            //if (btnInstance != NULL && _setGoldValue > 1){
                SetGold(btnInstance, (long) _setGoldValue, true);
                MakeToast(env, obj, OBFUSCATE("Done"), Toast::LENGTH_SHORT);
            //}
            break;
        case 5:
            LOGD(OBFUSCATE("Set Exp"));
            //if (btnInstance != NULL && _setExpValue > 1){
                SetExp(btnInstance, _setExpValue);
                MakeToast(env, obj, OBFUSCATE("Done"), Toast::LENGTH_SHORT);
            //}
            break;
        case 6:
            _isAdFree = boolean;
            break;
        case 7:
            LOGD(OBFUSCATE("LevelUp"));
            //if (btnInstance != NULL){
                LvlUp(btnInstance);
                MakeToast(env, obj, OBFUSCATE("Done"), Toast::LENGTH_SHORT);
            //}
            break;
        case 8:
            if (value >= 1) {
                _setMpValue = value;
            }
            break;
        case 9:
            _setMP = boolean;
            break;
    }
}
}

// ---------- Hooking ---------- //
void (*old_Update)(void *instance);
void Update(void *instance) {
    btnInstance = instance;
    old_Update(instance);
}

void (*old_LateUpdate)(void *instance);
void LateUpdate(void *instance) {
    btnInstance = instance;
    old_LateUpdate(instance);
}

bool (*old_IsAdFree)(void *instance);
bool get_IsAdFree(void *instance) {
    if (instance != NULL && _isAdFree) {
        return true;
    }
    return old_IsAdFree(instance);
}

int (*old_Mp)(void *instance);
int UpdateMp(void *instance) {
    if (instance != NULL && _setMP && _setMpValue > 1) {
        return _setMpValue*old_Mp(instance);
    }
    return old_Mp(instance);
}

void *hack_thread(void *) {
    LOGI(OBFUSCATE("pthread called"));
    do {
        sleep(1);
    } while (!isLibraryLoaded(targetLibName));

    LOGI(OBFUSCATE("%s has been loaded"), (const char *) targetLibName);

#if defined(__aarch64__)

#else
    hexPatches._OneHitKill = MemoryPatch::createWithHex(targetLibName,
                                                    string2Offset(OBFUSCATE_KEY("0x7E4864", 't')),
                                                    OBFUSCATE("7F 04 B0 E3 1E FF 2F E1"));
    hexPatches._OneHitKill2 = MemoryPatch::createWithHex(targetLibName,
                                                    string2Offset(OBFUSCATE_KEY("0x7E4970", 't')),
                                                    OBFUSCATE("7F 04 B0 E3 1E FF 2F E1"));

    hexPatches._Diamond = MemoryPatch::createWithHex(targetLibName,
                                                        string2Offset(OBFUSCATE_KEY("0xBD5308", 't')),
                                                        OBFUSCATE("DC 0F 0F E3 1E FF 2F E1"));
    hexPatches._Diamond.Modify();

    //get_Available
    MSHookFunction((void *) getAbsoluteAddress(targetLibName, string2Offset(OBFUSCATE_KEY("0x7E445C", '?'))),
                       (void *) get_IsAdFree, (void **) &old_IsAdFree);
    //get_Hp
    MSHookFunction((void *) getAbsoluteAddress(targetLibName, string2Offset(OBFUSCATE_KEY("0x7E4704", '?'))),
                   (void *) UpdateMp, (void **) &old_Mp);

    SetGold = (void (*)(void *, long, bool)) getAbsoluteAddress(targetLibName, 0xBD4B6C);
    SetExp = (void (*)(void *, int)) getAbsoluteAddress(targetLibName, 0xBD4370);
    LvlUp = (void (*)(void *)) getAbsoluteAddress(targetLibName, 0xBD5488);
    LOGI(OBFUSCATE("Done"));
#endif

    return NULL;
}

__attribute__((constructor))
void lib_main() {
    pthread_t ptid;
    pthread_create(&ptid, NULL, hack_thread, NULL);
}