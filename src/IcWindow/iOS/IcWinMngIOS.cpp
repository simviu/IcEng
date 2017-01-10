//
//  IcWinMngIOS.cpp
//  IcEngLib
//
//  Created by Sherman Chen on 1/5/17.
//  Copyright © 2017 Simviu Technology Inc. All rights reserved.
//


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
