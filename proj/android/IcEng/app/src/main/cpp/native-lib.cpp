#include <jni.h>
#include <string>
#include <android/log.h>


#include "MyTestApp.h"
#include "IcEngJNI.h"
#include "../../../../../../../src/test/MyTestApp.h"


using namespace Ic3d;

static MyTestApp l_app;
extern "C" JNIEXPORT void JNICALL Java_com_simviu_IcEngTest_MainActivity_initIcApp(JNIEnv * env,  jobject obj,  jstring jsPathCache)
{

    std::string sPathCache = IcEngJNI::jstr2str(env, jsPathCache);
    IcEngJNI::initIcApp(&l_app, sPathCache, sPathCache);
}
