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

using namespace std;
using namespace ctl;
using namespace Ic3d;


//-----------------------------------
//  onCmd
//-----------------------------------
std::string DemoApp::onCmd(const std::string& sCmd)
{
    string sRet = "OK";
    if(sCmd=="") return sRet;
    int i = sCmd[0]-'0';
    reqSetDemo(i);
    return sRet;
}
//-----------------------------------
//  DemoApp
//-----------------------------------
DemoApp::DemoApp()
{
}
void DemoApp::reqSetDemo(int sel)
{
    if(m_pDemoWin != nullptr)
        m_pDemoWin->reqSetDemo(sel);
};

//-----------------------------------
//  onInit
//-----------------------------------
void DemoApp::onInit()
{
    IcApp::onInit();
    
    // Copy res path
    DemoScene::m_cfg.m_sPathRes = m_cfg.m_sPathRes; // TODO: Simplify
    
    logInfo("DemoApp::onInit()");
    m_pDemoWin = makeSp<DemoWindow>();
    addWindow(m_pDemoWin);
    logInfo("DemoApp::onInit() done");
}
//-----------------------------------
//  DemoWindow override
//-----------------------------------
void DemoWindow::reqSetDemo(int sel)
{
    m_demoSel_req = sel;
}

void DemoWindow::onDrawUpdate(float deltaT)
{
    IcWindow::onDrawUpdate(deltaT);
    if(m_demoSel != m_demoSel_req)
    {
        auto pScn = DemoScene::createDemoScn(m_demoSel_req);
         removeAllScene();
        addScene(pScn);
        m_demoSel = m_demoSel_req;
    }
}

void DemoWindow::onKeyboard(unsigned char key)
{
    IcWindow::onKeyboard(key);
    switch(key)
    {
        case ' ' :
        {
            int N = DemoScene::getDemoNum();
            int i = m_demoSel +1;
            if(i>=N) i = 0;
            reqSetDemo(i);
        }break;
        default:break;
    }
}

