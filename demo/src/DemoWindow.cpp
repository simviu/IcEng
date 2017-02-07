//
//  DemoApp.cpp
//  IcEngDemo
//
//  Created by Sherman Chen on 12/24/16.
//  Copyright (c) 2016 Simviu Technology Inc.
//  All rights reserved.
//  http://www.simviu.com/dev
//

#include "DemoWindow.hpp"
#include "DemoScene.h"

using namespace std;
using namespace ctl;
using namespace Ic3d;

//-----------------------------------
//  DemoWindow
//-----------------------------------
DemoWindow::DemoWindow(const std::string& sPathRes)
:m_sPathRes(sPathRes)
{
    //---- Init demo list
    m_demoAry = {
        {"Basic"},              // 0
        {"Nested Transform"},   // 1
        {"Model Create"},       // 2
        {"Lights"},             // 3
    };
 
}

//-----------------------------------
//  createDemoScn
//-----------------------------------
void DemoWindow::createDemoScn(int sel)
{
    DemoScene::m_cfg.m_sPathRes = m_sPathRes;
    ctl::Sp<IcScene> pScn = nullptr;
    switch (sel) {
        case 0: pScn = ctl::makeSp<DemoBasic>();        break;
        case 1: pScn = ctl::makeSp<DemoNestedTrans>();  break;
        case 2: pScn = ctl::makeSp<DemoModelCreate>();  break;
        case 3: pScn = ctl::makeSp<DemoLights>();       break;
        default:
            break;
    }
    if(pScn==nullptr) return;
    removeAllScene();
    addScene(pScn);
}
//-----------------------------------
//  onInit
//-----------------------------------
void DemoWindow::onInit()
{
    IcWindow::onInit();
    logInfo("DemoWindow::onInit()");
    createDemoScn(m_demoSel);
    logInfo("DemoWindow::onInit() done");
}

