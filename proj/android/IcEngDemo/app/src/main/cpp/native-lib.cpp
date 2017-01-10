#include <jni.h>
#include <string>
#include "DemoWindow.hpp"
#include "IcEngJNI.h"

using namespace Ic3d;
static ctl::Sp<DemoWindow> l_pWin = nullptr;
extern "C" JNIEXPORT void JNICALL Java_com_simviu_IcEngDemo_DemoActivity_initApp(JNIEnv * env, jobject obj, jstring jsPathRes)
{
    std::string sPathRes = IcEngJNI::jstr2str(env, jsPathRes) + "/";
    l_pWin = ctl::makeSp<DemoWindow>(sPathRes);
    l_pWin->setDemoSel(4);
    IcEngJNI::setAppInstance(l_pWin.get());


}
