bool titleValid, headingValid, iconValid, settingsValid, isLeeched;

void setText(JNIEnv *env, jobject obj, const char* text){
    jclass html = (*env).FindClass(OBFUSCATE("android/text/Html"));
    jmethodID fromHtml = (*env).GetStaticMethodID(html, OBFUSCATE("fromHtml"), OBFUSCATE("(Ljava/lang/String;)Landroid/text/Spanned;"));

    //setText("");
    jclass textView = (*env).FindClass(OBFUSCATE("android/widget/TextView"));
    jmethodID setText = (*env).GetMethodID(textView, OBFUSCATE("setText"), OBFUSCATE("(Ljava/lang/CharSequence;)V"));

    //Java string
    jstring jstr = (*env).NewStringUTF(text);
    (*env).CallVoidMethod(obj, setText,  (*env).CallStaticObjectMethod(html, fromHtml, jstr));
}

void setTitleText(JNIEnv *env, jobject thiz, jobject obj) {
    setText(env, obj, OBFUSCATE("<b>Modded by (yourName)</b>"));

    titleValid = true;
}

void setHeadingText(JNIEnv *env, jobject thiz, jobject obj) {
    setText(env, obj, OBFUSCATE("<b><marquee><p style=\"font-size:30\">"
                                      "<p style=\"color:green;\">Modded by (yourName)</p> | "
                                      "(yourSite) | (yourText)</p>"
                                      "</marquee></b>"));

    headingValid = true;
}

jstring Icon(JNIEnv *env, jobject thiz) {
    iconValid = true;

    return env->NewStringUTF(
            OBFUSCATE("(yourImage)"));
}

jstring IconWebViewData(JNIEnv *env, jobject thiz) {
    iconValid = true;
    return NULL;
}

jobjectArray settingsList(JNIEnv *env, jobject activityObject) {
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

    settingsValid = true;

    return (ret);
}
