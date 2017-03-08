#include <jni.h>
#include <string>
#include "DemoApp.hpp"
#include "IcEngJNI.h"

using namespace Ic3d;
static DemoApp l_demoApp;
extern "C" JNIEXPORT void JNICALL Java_com_simviu_IcEng_IcEngJNI_createIcApp
        (JNIEnv * env, jobject obj)
{
    IcEngJNI::setIcAppInstance(&l_demoApp);

}
