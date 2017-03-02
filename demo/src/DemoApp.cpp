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
    logInfo("DemoApp::onCmd('"+sCmd+"')");
    string sRet = "";

    //---- Sep Main cmd and args
    auto tkns = s2tokens(sCmd, ":");
    if(tkns.size()==0)
        return sRet;

    string sCmdMain = tkns[0];
    string sCmdArgs;
    if(tkns.size()>1) sCmdArgs = tkns[1];

    //---------------------
    //  Decode Cmd
    //---------------------

    //---- get Demo list
    if(sCmdMain=="list")
    {
        string sDemos;
        int N = DemoScene::getDemoNum();
        for(int i=0;i<N;i++)
        {
            const auto& item = DemoScene::getDemoItem(i);
            sDemos += item.m_sTitle +";";
        }
        sRet = sDemos;
    }//---- req select demo
    else if(sCmdMain=="sel")
    {
        if(sCmdArgs.size()==0)
        {
            logErr("DemoApp::onCmd(): wrong selection");
            return sRet;
        }
        char c= sCmdArgs[0];
        int i = c-'0';
        reqSetDemo(i);
        sRet = "OK";
    }
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
    m_demoSel = sel;
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
    m_pDemoWin->reqSetDemo(m_demoSel);
    logInfo("DemoApp::onInit() done");
}

//-----------------------------------
//  DemoWindow override
//-----------------------------------
void DemoWindow::onInit()
{
    IcWindow::onInit();
    m_demoSel = -1;
}

//-----------------------------------
//  DemoWindow override
//-----------------------------------
void DemoWindow::reqSetDemo(int sel) {
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

