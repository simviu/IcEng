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

static MyTestApp l_myApp;
extern "C" JNIEXPORT void JNICALL Java_com_simviu_IcEngTest_IcTestActivity_initApp(JNIEnv * env, jobject obj, jstring jsPathRes)
{
    std::string sPathRes = IcEngJNI::jstr2str(env, jsPathRes);
    //---- Configure App
    LOGI("MyTestApp instanciated at [0x%x]", &l_myApp);
    l_myApp.m_cfg.m_sPathRes = sPathRes;

}
