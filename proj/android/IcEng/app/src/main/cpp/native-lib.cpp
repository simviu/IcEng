#include <jni.h>
#include <string>


#include "IcTestWindow.h"
#include "IcEngJNI.h"

#define  LOG_TAG    "IcAppJNI"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

using namespace Ic3d;
static ctl::Sp<IcTestWindow> l_pTestWin = nullptr;
extern "C" JNIEXPORT void JNICALL Java_com_simviu_IcEngTest_IcTestActivity_initApp(JNIEnv * env, jobject obj, jstring jsPathRes)
{
    std::string sPathRes = IcEngJNI::jstr2str(env, jsPathRes);
    //---- Configure App
    l_pTestWin = ctl::makeSp<IcTestWindow>();
    IcEngJNI::setAppInstance(l_pTestWin.get());
}
