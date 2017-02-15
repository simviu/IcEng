//
//  DemoApp.cpp
//  IcEngDemo
//
//  Created by Sherman Chen on 12/24/16.
//  Copyright (c) 2016 Simviu Technology Inc.
//  All rights reserved.
//  http://www.simviu.com/dev
//

#include "DemoApp.hpp"
#include "DemoScene.h"

using namespace std;
using namespace ctl;
using namespace Ic3d;

//-----------------------------------
//  createDemoScn
//-----------------------------------
ctl::Sp<IcScene> DemoApp::createDemoScn(int sel)
{
    ctl::Sp<IcScene> pScn = nullptr;
    switch (sel) {
        case 0: pScn = ctl::makeSp<DemoBasic>();        break;
        case 1: pScn = ctl::makeSp<DemoNestedTrans>();  break;
        case 2: pScn = ctl::makeSp<DemoModelCreate>();  break;
        case 3: pScn = ctl::makeSp<DemoLights>();       break;
        default:
            break;
    }
    return pScn;
}

//-----------------------------------
//  runCmdLine
//-----------------------------------
int DemoApp::runCmdLine(int argc, char **argv)
{
    if(argc>1)
        onCmd(argv[1]);
    return IcApp::runCmdLine(argc, argv);
}

//-----------------------------------
//  onCmd
//-----------------------------------
std::string DemoApp::onCmd(const std::string& sCmd)
{
    string sRet = "OK";
    if(sCmd=="") return sRet;
    int i = sCmd[0]-'0';
    m_demoSel = i;
    auto pScn = createDemoScn(i);

    return sRet;
}

//-----------------------------------
//  onInit
//-----------------------------------
void DemoApp::onInit()
{
    IcApp::onInit();
    //---- Init demo list
    m_demoAry = {
        {"Basic"},              // 0
        {"Nested Transform"},   // 1
        {"Model Create"},       // 2
        {"Lights"},             // 3
    };
    
    logInfo("DemoApp::onInit()");
    auto pWin = makeSp<IcWindow>();
    auto pScn = createDemoScn(m_demoSel);
    pWin->addScene(pScn);
    addWindow(pWin);
    logInfo("DemoApp::onInit() done");
}
//---- Instanciatiation of App
static DemoApp l_app;
namespace Ic3d {
    extern IcApp& getIcAppInstance()
    {
        return l_app;
    }
}

