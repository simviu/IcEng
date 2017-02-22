#include <jni.h>
#include <string>
#include <android/log.h>


#include "MyTestApp.h"
#include "IcEngJNI.h"
#include "../../../../../../../src/test/MyTestApp.h"

#define  LOG_TAG    "IcAppJNI"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

using namespace Ic3d;

static MyTestApp l_app;
extern "C" JNIEXPORT void JNICALL Java_com_simviu_IcEngTest_MainActivity_initIcApp(JNIEnv * env,  jobject obj,  jstring jsPathCache)
{
    std::string sPathCache = IcEngJNI::jstr2str(env, jsPathCache);
    IcEngJNI::initIcApp(&l_app, sPathCache, sPathCache);
}
