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
static const string K_sModel = "IcDemo/MixShapes/MixShapes.obj";

//----------------------------------------------
//  DemoBasic::onInit
//----------------------------------------------
void DemoBasic::onInit()
{
    //--- Always call parent class onInit()
    IcScene::onInit();
    
    //--- Load Object
    string sFile = DemoScene::m_cfg.m_sPathRes + K_sModel;
    auto pModel = ctl::makeSp<IcModel>(sFile);
    auto pObj   = ctl::makeSp<IcObject>(pModel);
    // Add this obj to Scene, will be rendered
    addObj(pObj);
    
    
   
    
}
