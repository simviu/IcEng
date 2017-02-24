#include <jni.h>
#include <string>
#include "DemoApp.hpp"
#include "IcEngJNI.h"

using namespace Ic3d;
static DemoApp l_demoApp;
extern "C" JNIEXPORT void JNICALL Java_com_simviu_IcEngDemo_DemoListActivity_initIcApp
        (JNIEnv * env, jobject obj,  jstring jsPathCache)
{
    std::string sPathCache = IcEngJNI::jstr2str(env, jsPathCache);
    IcEngJNI::initIcApp(&l_demoApp, sPathCache, sPathCache);

}
