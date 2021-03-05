
bool titleValid = false, headingValid = false, iconValid = false;

void *antiLeech(void *) {
    sleep(15);
    if (!titleValid || !headingValid || !iconValid) {
        int *p = 0;
        *p = 0;
    }
    return NULL;
}

extern "C" {
JNIEXPORT jstring
JNICALL
Java_com_tfive_modmenu_FloatingModMenuService_Title(JNIEnv *env, jobject thiz) {
    titleValid = true;
    return env->NewStringUTF(OBFUSCATE("<b>Modded by (yourName)</b>"));
}

JNIEXPORT jstring
JNICALL
Java_com_tfive_modmenu_FloatingModMenuService_Heading(JNIEnv *env, jobject thiz) {
    headingValid = true;
    return env->NewStringUTF(OBFUSCATE("<b><marquee><p style=\"font-size:30\">"
                                       "<p style=\"color:green;\">Modded by (yourName)</p> | "
                                       "(yourSite) | (yourText)</p>"
                                       "</marquee></b>"));
}

JNIEXPORT jstring
JNICALL
Java_com_tfive_modmenu_FloatingModMenuService_Icon(JNIEnv *env, jobject thiz) {
    iconValid = true;
    return env->NewStringUTF(
            OBFUSCATE("(yourImage)"));
}

JNIEXPORT jstring
JNICALL
Java_com_tfive_modmenu_FloatingModMenuService_IconWebViewData(JNIEnv *env, jobject thiz) {
    iconValid = true;
    return NULL;
}

JNIEXPORT jobjectArray
JNICALL
Java_com_tfive_modmenu_FloatingModMenuService_settingsList(JNIEnv *env, jobject activityObject) {
    jobjectArray ret;

    const char *features[] = {
            OBFUSCATE("Category_Settings"),
            OBFUSCATE("-1_Toggle_Color animation"),
            OBFUSCATE("-2_Toggle_Auto size vertically"),
            OBFUSCATE("-3_Toggle_Save feature preferences (Radio Button is not saved)"),
            OBFUSCATE("Category_Logcat"),
            OBFUSCATE("RichTextView_Save logcat if a bug occured and sent it to the modder. Clear logcat and reproduce bug again if the log file is too large"),
            OBFUSCATE("RichTextView_<small>Saving logcat does not need file permission. Logcat location:<br/>Android 11: /storage/emulated/0/Documents/Mod Menu logs/"
                      "<br/>Android 10 and below: /storage/emulated/0/Android/data/(package name)/files/Mod Menu logs</small>"),
            OBFUSCATE("-4_Button_Save logcat to file"),
            OBFUSCATE("-5_Button_Clear logcat"),
            OBFUSCATE("Category_Menu"),
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
}