//
//  DemoBasic.cpp
//  DevEng
//
//  Created by Sherman Chen on 10/10/16.
//  Copyright (c) 2016 Simviu Technology Inc.
//  All rights reserved.
//  http://www.simviu.com/dev
//

#include "DemoScene.h"

using namespace std;
using namespace ctl;
using namespace Ic3d;
//static const string K_sModel = "IcDemo/MixShapes/MixShapes.obj";
static const string K_sModel = "IcDemo/monkey/monkey.obj";

//----------------------------------------------
//  DemoBasic::onInit
//----------------------------------------------
void DemoBasic::onInit()
{
    //--- Always call parent class onInit()
    IcScene::onInit();
    
    //--- Load Object
    const auto& cfg = IcApp::getInstance()->m_cfg;
    string sPathRes = cfg.m_sPathRes;
    string sFile = sPathRes + K_sModel;
    auto pModel = ctl::makeSp<IcModel>(sFile);
    auto pObj   = ctl::makeSp<IcObject>(pModel);
    // Add this obj to Scene, will be rendered
    addObj(pObj);
    
    
   
    
}
