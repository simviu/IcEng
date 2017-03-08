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

using namespace ctl;
using namespace Ic3d;


#define  LOG_TAG    "IcAppJNI"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)


//------------------------------------------
//  Android Log handler to replace ctl::Log
//------------------------------------------
struct LogHandlerJNI : LogHandler
{
    virtual void logMsgBase(const std::string& sMsg) override
    { LOGI("%s", sMsg.c_str()); };

};
static LogHandlerJNI l_logHandlerJNI;



//------------------------------------------
//  IcEngJNI::initIcApp()
//------------------------------------------
void IcEngJNI::setIcAppInstance(IcApp* pApp)
{
    //---- Set logHandler
    ctl::LogHandler::setCurHandler(&l_logHandlerJNI);
    if(pApp== nullptr) return;
    IcApp::setInstance(pApp);
}

//------------------------------------------
//  IcEngJNI::initIcApp()
//------------------------------------------
void IcEngJNI::initIcApp(const std::string& sPathRes,
                         const std::string& sPathDoc)
{

    auto pApp = IcApp::getInstance();

    //---- Init App
    logInfo("------------------------------------------");
    logInfo("IcEngJNI::initIcApp:[0x"+v2hex(pApp)+"]");
    auto& cfg = pApp->m_cfg;
    cfg.m_sPathRes = sPathRes +"/";
    cfg.m_sPathDoc = sPathDoc +"/";
    pApp->onInit();
}


//------------------------------------------
//  JNI interfcae
//------------------------------------------
extern "C" JNIEXPORT void JNICALL Java_com_simviu_IcEng_IcEngJNI_initIcAppWithDir(JNIEnv * env, jobject obj,
                                                                            jstring jsPathRes, jstring jsPathDoc)
{

    std::string sPathRes = IcEngJNI::jstr2str(env, jsPathRes);
    std::string sPathDoc = IcEngJNI::jstr2str(env, jsPathDoc);
    IcEngJNI::initIcApp(sPathRes, sPathDoc);
}
extern "C" JNIEXPORT void JNICALL Java_com_simviu_IcEng_IcEngJNI_initWindow(JNIEnv * env, jobject obj)
{
    //---- Init App
    auto pApp = IcApp::getInstance();
    if(pApp== nullptr)
    {
        LOGI("Error: JNI IcEngJNI_onInit(): IcApp instance empty\n");
        return;
    }
    pApp->getWinMng()->initWindows();
}

//------------------------------------------
//  IcAppJNI.onReleaseWindow()
//------------------------------------------
extern "C" JNIEXPORT void JNICALL Java_com_simviu_IcEng_IcEngJNI_releaseWindow(JNIEnv * env, jobject obj)
{
    //---- Init App
    auto pApp = IcApp::getInstance();
    pApp->getWinMng()->releaseWindows();
}
//------------------------------------------
//  IcAppJNI.onScreenSize()
//------------------------------------------
extern "C" JNIEXPORT void JNICALL Java_com_simviu_IcEng_IcEngJNI_setScreenSize(JNIEnv * env, jobject obj,
                                                                              jint width, jint height)
{
    auto pApp = IcApp::getInstance();
    if(pApp== nullptr) return;
    pApp->getWinMng()->onScreenSize(ctl::TSize(width, height));
}

//------------------------------------------
//  IcAppJNI.step()
//------------------------------------------
extern "C" JNIEXPORT void JNICALL Java_com_simviu_IcEng_IcEngJNI_drawUpdate(JNIEnv * env, jobject obj, jfloat deltaT)
{
    auto pApp = IcApp::getInstance();
    if(pApp== nullptr) return;
    pApp->getWinMng()->drawUpdate(deltaT);
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
//  IcAppJNI.debugPrint()
//------------------------------------------
extern "C" JNIEXPORT jstring JNICALL Java_com_simviu_IcEng_IcEngJNI_sendAppCmd(JNIEnv * env, jobject obj, jstring sCmd)
{
    const char* csCmd = (*env).GetStringUTFChars(sCmd, 0);
    LOGI("JNI debugPrint[%s]\n", csCmd);
    auto pApp = IcApp::getInstance();
    std::string sRet = "";
    if(pApp!= nullptr)
        sRet = pApp->onCmd(std::string(csCmd));
    else{
        sRet = "IcEngJNI.sendAppCmd() failed: IcApp instance not instantiated";
        logErr(sRet);
    }
    (*env).ReleaseStringUTFChars(sCmd, csCmd);
    return (*env).NewStringUTF(sRet.c_str());
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

