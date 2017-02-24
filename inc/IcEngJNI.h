//
//  IcEngJNI.h
//  IcEngLib
//
//  Created by Sherman Chen on 1/4/17.
//  Copyright (c) 2016 Simviu Technology Inc.
//  All rights reserved.
//  http://www.simviu.com/dev
//
//

#ifndef IcEngJNI_h
#define IcEngJNI_h
#include "Ic3d.h"
#include <jni.h>
#include <string>

namespace Ic3d {
    //----------------
    //  JNI Wrapper
    //----------------
    // All android JNI related util put here.
    class IcEngJNI
    {
    public:
        //---- JNI util
        static std::string jstr2str(JNIEnv * env, jstring jstr);
        static void initIcApp(IcApp* pApp,
                              const std::string& sPathRes,
                              const std::string& sPathDoc);
    };
}

#endif /* IcEngJNI_h */
