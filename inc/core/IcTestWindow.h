//
//  IcTestApp.h
//  IcEngLib
//
//  Created by Sherman Chen on 12/24/16.
//  Copyright © 2016 Simviu Technology Inc. All rights reserved.
//

#ifndef IcTestApp_h
#define IcTestApp_h
#include "Ic3d.h"
//-----------------------------------------
//  Embeded test App
//-----------------------------------------
class IcTestWindow : public Ic3d::IcWindow
{
public:
    virtual void onInit() override;
};

#endif /* IcTestApp_h */
