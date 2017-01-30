//
//  DemoNestedTrans.cpp
//  DevEng
//
//  Created by Sherman Chen on 10/25/16.
//  Copyright (c) 2016 Simviu Technology Inc.
//  All rights reserved.
//  http://www.simviu.com/dev
//

#include "DemoNestedTrans.hpp"


using namespace Ic3d;
using namespace ctl;

//----------------------------------------------
//  DemoNestedTrans::onInit
//----------------------------------------------
void DemoNestedTrans::onInit()
{
    //--- Always call parent class onInit()
    IcScene::onInit();
    
    //---- Let's create 3 cube models, with different color
    float L0 = 8;   // Length of upper arm
    float L1 = 16;  // Length of lower arm
    float L2 = 4;   // Length of palm
    auto pModel0 = IcModel::createCube(TVec3(1.1,L0/2,1), TVec3(0,L0/2,0), TColor(1,0,0,1)); // Red Cube
    auto pModel1 = IcModel::createCube(TVec3(1,L1/2,1),   TVec3(0,L1/2,0), TColor(0,1,1,1)); // Green Cube
    auto pModel2 = IcModel::createCube(TVec3(1.2,L2/2,1), TVec3(0,L2/2,0), TColor(0,0,1,1)); // Blue Cube
    
    //---- Create robot arm objects with above models
    // 1) Upper Arm, 2) lower Arm, 3)palm
    auto pObj_upArm   = ctl::makeSp<IcObject>(pModel0);
    auto pObj_lowArm  = ctl::makeSp<IcObject>(pModel1);
    auto pObj_palm    = ctl::makeSp<IcObject>(pModel2);
    
    
    //---- Scale them differently to make them looks like a arm
    // Extrude toward +y (upward)
    pObj_upArm->setPos(TVec3(0,0,0));
    pObj_lowArm->setPos(TVec3(0,L0,0));   // Move center to tip of upArm
    pObj_palm->setPos(TVec3(0,L1,0));     // Move center to tip of lowArm
   
    //---- Chian the robot arm, by addChildObj()
    addObj(pObj_upArm);
    pObj_upArm->addChildObj(pObj_lowArm);
    pObj_lowArm->addChildObj(pObj_palm);
    
    //---- Play around robot arm's rotating
    TVec3 angleEuler0(deg2rad(10), deg2rad(0), deg2rad(0));
    TVec3 angleEuler1(deg2rad(60), deg2rad(0), deg2rad(0));
    TVec3 angleEuler2(deg2rad(30), deg2rad(45), deg2rad(0));

    pObj_upArm->setQuat(TQuat(angleEuler0));
    pObj_lowArm->setQuat(TQuat(angleEuler1));
    pObj_palm->setQuat(TQuat(angleEuler2));
    
    //---- Set Camera
    auto& cam = *getCamera();
    cam.setPos(TVec3(-40, 0, 0));   // Put camera somewhere
    // Set camera look at obj, up vector is +y.
    // Our obj is at (0,0,0) position by default.
    cam.lookAt(TVec3(0,0,0), TVec3(0,1,0));
    
    // Note: Since we are using std::shared_ptr,
    //  memory management is handled automatically.
    //  No need to delete anything manually.
    
    
    
}
