//
//  DemoBasic.cpp
//  DevEng
//
//  Created by Sherman Chen on 10/10/16.
//  Copyright (c) 2016 Simviu Technology Inc.
//  All rights reserved.
//  http://www.simviu.com/dev
//

#include "DemoBasic.hpp"

using namespace Ic3d;

//----------------------------------------------
//  DemoBasic::onInit
//----------------------------------------------
void DemoBasic::onInit()
{
    //--- Always call parent class onInit()
    IcScene::onInit();
    
    //--- Load Object
    IcMeshData mshd; mshd.createSphere(1, 20, 20);
    auto pModel = ctl::makeSp<IcModel>(mshd);
    auto pObj   = ctl::makeSp<IcObject>(pModel);
    // Add this obj to Scene, will be rendered
    addObj(pObj);
    
    //---- Set Camera
    auto& cam = *getCamera();

    // Put Camera somewhere apart.
    cam.setPos(TVec3(10, 4, -6));
    
    // Set camera look at obj, up vector is +y.
    // Our obj is at (0,0,0) position by default.
    cam.lookAt(TVec3(0,0,0), TVec3(0,1,0));
    
   
    
}
