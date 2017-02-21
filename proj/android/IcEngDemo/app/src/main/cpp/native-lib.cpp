#include <jni.h>
#include <string>
#include "DemoApp.hpp"
#include "IcEngJNI.h"

using namespace Ic3d;
static DemoApp l_demoApp;
extern "C" JNIEXPORT void JNICALL Java_com_simviu_IcEngDemo_DemoListActivity_initApp
        (JNIEnv * env, jobject obj)
{
    auto pApp = &l_demoApp;
    ctl::logDbg("DemoApp instantiated at[0x" +
                        ctl::v2hex((intptr_t)(pApp))+"]");

}
