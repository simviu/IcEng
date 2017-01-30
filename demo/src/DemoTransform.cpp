//
//  DemoTransform.cpp
//  DevEng
//
//  Created by Sherman Chen on 10/25/16.
//  Copyright (c) 2016 Simviu Technology Inc.
//  All rights reserved.
//  http://www.simviu.com/dev
//

#include "DemoTransform.hpp"


using namespace Ic3d;
using namespace ctl;

//----------------------------------------------
//  DemoTransform::onInit
//----------------------------------------------
void DemoTransform::onInit()
{
    //--- Always call parent class onInit()
    IcScene::onInit();
    
    //---- Let's create 3 cube models, with different color
    auto pModel0 = IcModel::createCube(TVec3(1,1,1), TVec3(0,0,0), TColor(1,0,0,1)); // Red Cube
    auto pModel1 = IcModel::createCube(TVec3(1,1,1), TVec3(0,0,0), TColor(0,1,1,1)); // Green Cube
    auto pModel2 = IcModel::createCube(TVec3(1,1,1), TVec3(0,0,0), TColor(0,0,1,1)); // Blue Cube
    
    //---- Create 3 objects with above models
    auto pObj0   = ctl::makeSp<IcObject>(pModel0);
    auto pObj1   = ctl::makeSp<IcObject>(pModel1);
    auto pObj2   = ctl::makeSp<IcObject>(pModel2);
    
    //---- Add these obj to Scene, will be rendered
    addObj(pObj0);
    addObj(pObj1);
    addObj(pObj2);
    
    //---- Play around with transform
    pObj0->setPos(TVec3(-2, -3, -5));
    
    TVec3 angleEuler1(ctl::deg2rad(10), deg2rad(30), deg2rad(15));
    pObj1->setPos(TVec3(3, -1, -15));
    pObj1->setQuat(TQuat(angleEuler1));
    
    TVec3 angleEuler2(deg2rad(10), deg2rad(60), deg2rad(-45));
    pObj2->setPos(TVec3(-4, -5, -20));
    pObj2->setQuat(TQuat(angleEuler2));
    pObj2->setScale(TVec3(1, 2, 5));
    
    //---- Set Camera
    auto& cam = *getCamera();
    cam.setPos(TVec3(0, 0, 10));   // Put camera somewhere
    // Set camera look at obj, up vector is +y.
    // Our obj is at (0,0,0) position by default.
    cam.lookAt(TVec3(0,0,0), TVec3(0,1,0));
    
    // Note: Since we are using std::shared_ptr,
    //  memory management is handled automatically.
    //  No need to delete anything manually.

}
