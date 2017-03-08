#include <jni.h>
#include <string>
#include <android/log.h>


#include "MyTestApp.h"
#include "IcEngJNI.h"
#include "../../../../../../../src/test/MyTestApp.h"


using namespace Ic3d;

static MyTestApp l_app;
extern "C" JNIEXPORT void JNICALL Java_com_simviu_IcEng_IcEngJNI_createIcApp(JNIEnv * env,  jobject obj)
{

    IcEngJNI::setIcAppInstance(&l_app);
}
