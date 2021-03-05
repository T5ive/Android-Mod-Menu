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


#if defined(__aarch64__) //Compile for arm64 lib only
#include <And64InlineHook/And64InlineHook.hpp>
#else //Compile for armv7 lib only. Do not worry about greyed out highlighting code, it still works

#include <Substrate/SubstrateHook.h>
#include <Substrate/CydiaSubstrate.h>
#include <iostream>

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
JNIEXPORT void JNICALL
Java_uk_lgl_MainActivity_Toast(JNIEnv *env, jclass obj, jobject context) {
    MakeToast(env, context, OBFUSCATE("Modded by TFive"), Toast::LENGTH_LONG);
}

// Note:
// Do not change or translate the first text unless you know what you are doing
// Assigning feature numbers is optional. Without it, it will automatically count for you, starting from 0
// Assigned feature numbers can be like any numbers 1,3,200,10... instead in order 0,1,2,3,4,5...
// ButtonLink, Category, RichTextView and RichWebView is not counted. They can't have feature number assigned
// To learn HTML, go to this page: https://www.w3schools.com/

// Usage:
// (Optional feature number)_Toggle_(feature name)
// (Optional feature number)_SeekBar_(feature name)_(min value)_(max value)
// (Optional feature number)_Spinner_(feature name)_(Items e.g. item1,item2,item3)
// (Optional feature number)_Button_(feature name)
// (Optional feature number)_ButtonOnOff_(feature name)
// (Optional feature number)_InputValue_(feature name)
// (Optional feature number)_CheckBox_(feature name)
// (Optional feature number)_RadioButton_(feature name)_(Items e.g. radio1,radio2,radio3)
// RichTextView_(Text with limited HTML support)
// RichWebView_(Full HTML support)
// ButtonLink_(feature name)_(URL/Link here)
// Category_(text)

// Few examples:
// 10_Toggle_Jump hack
// 100_Toggle_Ammo hack
// Toggle_Ammo hack
// 1_Spinner_Weapons_AK47,9mm,Knife
// Spinner_Weapons_AK47,9mm,Knife
// 2_ButtonOnOff_God mode
// Category_Hello world

JNIEXPORT jobjectArray
JNICALL
Java_com_tfive_modmenu_FloatingModMenuService_getFeatureList(JNIEnv *env, jobject activityObject) {
    jobjectArray ret;

    const char *features[] = {
            OBFUSCATE("Category_The Category"), // ไม่นับ
            OBFUSCATE("Toggle_1 Hit Kill"), // 0
            OBFUSCATE("100_Toggle_Super God Mode"), // 100
            OBFUSCATE("SeekBar_HP_1_3"), // 1
            OBFUSCATE("SeekBar_Gold_1000_100000"), // 2
            OBFUSCATE("Button_Set Gold"), // 3
            OBFUSCATE("Button_Set Exp 100"), // 4
            OBFUSCATE("ButtonOnOff_Ad Free"), // 5
            OBFUSCATE("Button_LevelUp"), // 6
            OBFUSCATE("SeekBar_MP_100_500"), // 7
            OBFUSCATE("Toggle_MP") // 8
    };

    //Now you dont have to manually update the number everytime;
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
    //Convert java string to c++
    const char *featureName = env->GetStringUTFChars(featName, 0);
    const char *TextInput;
    if (str != NULL)
        TextInput = env->GetStringUTFChars(str, 0);
    else
        TextInput = "Empty";

    LOGD(OBFUSCATE("Feature name: %d - %s | Value: = %d | Bool: = %d | Text: = %s"), featNum,
        featureName, value,
        boolean, TextInput);

    //BE CAREFUL NOT TO ACCIDENTLY REMOVE break;

    switch (featNum) {
        case 0:
            _oneHitKill = boolean;
            if (_oneHitKill) {
                hexPatches._OneHitKill.Modify();
                hexPatches._OneHitKill2.Modify();
                //LOGI(OBFUSCATE("On"));
            } else {
                hexPatches._OneHitKill.Restore();
                hexPatches._OneHitKill2.Restore();
                //LOGI(OBFUSCATE("Off"));
            }
            break;
        case 2:
            if (value >= 1) {
                 _setGoldValue = value;
            }
            break;
        case 3:
            LOGD(OBFUSCATE("Set Gold"));
            //if (btnInstance != NULL && _setGoldValue > 1){
                SetGold(btnInstance, (long) _setGoldValue, true);
                MakeToast(env, obj, OBFUSCATE("Done"), Toast::LENGTH_SHORT);
            //}
            break;
        case 4:
            LOGD(OBFUSCATE("Set Exp"));
            //if (btnInstance != NULL && _setExpValue > 1){
                SetExp(btnInstance, _setExpValue);
                MakeToast(env, obj, OBFUSCATE("Done"), Toast::LENGTH_SHORT);
            //}
            break;
        case 5:
            _isAdFree = boolean;
            break;
        case 6:
            LOGD(OBFUSCATE("LevelUp"));
            //if (btnInstance != NULL){
                LvlUp(btnInstance);
                MakeToast(env, obj, OBFUSCATE("Done"), Toast::LENGTH_SHORT);
            //}
            break;
        case 7:
            if (value >= 1) {
                _setMpValue = value;
            }
            break;
        case 8:
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