//
//  IcWinMngAndroid.cpp
//  DevEng
//
//  Created by Sherman Chen on 1/5/17.
//  Copyright (c) 2016 Simviu Technology Inc.
//  All rights reserved.
//  http://www.simviu.com/dev
////

#include "Ic3d.h"

namespace Ic3d
{
    using namespace ctl;
    using namespace std;
    using namespace Ic3d;
    //----------------------------
    //  Platform Implementation
    //----------------------------
    ctl::Sp<IcWinMng> IcWinMng::createWinMngImpl()
    {
        return ctl::makeSp<IcWinMng>();
    }
   
}
