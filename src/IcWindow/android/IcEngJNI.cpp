//
//  IcEngJNI.cpp
//  DevEng
//
//  Created by Sherman Chen on 12/24/16.
//  Copyright (c) 2016 Simviu Technology Inc.
//  All rights reserved.
//  http://www.simviu.com/dev
//
//


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
