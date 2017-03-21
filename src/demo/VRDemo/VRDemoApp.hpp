//
//  VRDemoApp.hpp
//  IcVRDemo
//
//  Created by Sherman Chen on 3/14/17.
//  Copyright © 2017 Simviu Technology Inc. All rights reserved.
//

#ifndef VRDemoApp_hpp
#define VRDemoApp_hpp

#include "Ic3d.h"

//--------------------------------------
//  VRDemoApp
//--------------------------------------
class VRDemoApp : public Ic3d::IcApp
{
public:
    virtual void onInit() override;
};

#endif /* VRDemoApp_hpp */
