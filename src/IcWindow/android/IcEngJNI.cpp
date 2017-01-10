/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// OpenGL ES 2.0 code

#include <jni.h>
#include <android/log.h>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "Ic3d.h"
#include "IcEngJNI.h"

using namespace Ic3d;


#define  LOG_TAG    "IcAppJNI"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

//------------------------------------------
//  Static pointer of IcApp
//------------------------------------------
static IcWindow* l_pWinInstance = nullptr;
void IcEngJNI::setAppInstance(IcWindow* pWin)
{
    l_pWinInstance = pWin;
}

//------------------------------------------
//  IcAppJNI.onInit()
//------------------------------------------
extern "C" JNIEXPORT void JNICALL Java_com_simviu_IcEng_IcEngJNI_onInit(JNIEnv * env, jobject obj, jstring jsPathRes)
{
    //---- Configure IcEng
    std::string sPathRes = IcEngJNI::jstr2str(env, jsPathRes);
    auto pEng = IcEng::getInstance();
    pEng->m_cfg.m_sPath_shader = sPathRes + "/IcShader/";


    //---- Init window
    auto pWin = l_pWinInstance;
    if(pWin== nullptr) return;
    pWin->onInit();
}

//------------------------------------------
//  IcAppJNI.onScreenSize()
//------------------------------------------
extern "C" JNIEXPORT void JNICALL Java_com_simviu_IcEng_IcEngJNI_onScreenSize(JNIEnv * env, jobject obj,
                                                                              jint width, jint height)
{
    auto pWin = l_pWinInstance;
    if(pWin== nullptr) return;
    pWin->onScreenSize(ctl::TSize(width, height));
}

//------------------------------------------
//  IcAppJNI.step()
//------------------------------------------
extern "C" JNIEXPORT void JNICALL Java_com_simviu_IcEng_IcEngJNI_onDrawUpdate(JNIEnv * env, jobject obj, jfloat deltaT)
{
    auto pWin = l_pWinInstance;
    if(pWin== nullptr) return;
 //   auto pApp = g_pIcAppJniInstnace;
    pWin->onDrawUpdate(deltaT);
}

//------------------------------------------
//  IcAppJNI.debugPrint()
//------------------------------------------
extern "C" JNIEXPORT void JNICALL Java_com_simviu_IcEng_IcEngJNI_debugPrint(JNIEnv * env, jobject obj, jstring sMsg)
{
    const char* cstr = (*env).GetStringUTFChars(sMsg, 0);
    LOGI("JNI debugPrint[%s]\n", cstr);
    (*env).ReleaseStringUTFChars(sMsg, cstr);

}

//------------------------------------------
//  util jstr2str()
//------------------------------------------
std::string IcEngJNI::jstr2str(JNIEnv * env, jstring jstr)
{
    //---- Get res path from para
    const char* cstr = (*env).GetStringUTFChars(jstr, 0);
    std::string str(cstr);
    (*env).ReleaseStringUTFChars(jstr, cstr);
    return str;
}
