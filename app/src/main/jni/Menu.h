
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
Java_com_tfive_modmenu_FloatingModMenuService_getFeatureList(JNIEnv *env, jobject activityObject) {
    jobjectArray ret;

    const char *features[] = {
            //(yourFeatures)
            OBFUSCATE(
                    "0_RichWebView_<html><body><marquee style=\"color: white; font-weight:bold;\" direction=\"left\" scrollamount=\"5\" behavior=\"scroll\">"
                    "(yourEndCredit)"
                    "</marquee></body></html>")
    };

    int Total_Feature = (sizeof features /
                         sizeof features[0]);
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
}