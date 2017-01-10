//
//  DemoApp.cpp
//  IcEngDemo
//
//  Created by Sherman Chen on 12/24/16.
//  Copyright © 2016 Simviu Technology Inc. All rights reserved.
//

#include "DemoWindow.hpp"

#include "DemoBasic.hpp"
#include "DemoTransform.hpp"
#include "DemoNestedTrans.hpp"
#include "DemoAnimation.hpp"
#include "DemoModels.h"
#include "DemoModelCreate.hpp"
#include "DemoLights.hpp"



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
        {"Transform"},          // 1
        {"Nested Transform"},   // 2
        {"Animation"},          // 3
        {"Models"},             // 4
        {"Model Create"},       // 5
        {"Lights"},             // 6
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
        case 1: pScn = ctl::makeSp<DemoTransform>();    break;
        case 2: pScn = ctl::makeSp<DemoNestedTrans>();  break;
        case 3: pScn = ctl::makeSp<DemoAnimation>();    break;
        case 4: pScn = ctl::makeSp<DemoModels>();       break;
        case 5: pScn = ctl::makeSp<DemoModelCreate>();  break;
        case 6: pScn = ctl::makeSp<DemoLights>();       break;
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

