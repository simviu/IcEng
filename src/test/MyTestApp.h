//
//  MyTestApp.h
//  IcEngLib
//
//  Created by Sherman Chen on 2/15/17.
//  Copyright © 2017 Simviu Technology Inc. All rights reserved.
//

#ifndef MyTestApp_h
#define MyTestApp_h
#include "Ic3d.h"

//---------------------------------
//  MyTestApp
//---------------------------------
class MyTestApp : public Ic3d::IcApp
{
    //---- Override onInit
    virtual void onInit() override;
};

#endif /* MyTestApp_h */
