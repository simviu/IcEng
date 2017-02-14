//
//  IcApp.cpp
//  IcEngLib
//
//  Created by Sherman Chen on 2/14/17.
//  Copyright © 2017 Simviu Technology Inc. All rights reserved.
//


#include "Ic3d.h"

namespace Ic3d
{
    using namespace ctl;
    using namespace std;
    //----------------------------
    //  addWindow
    //----------------------------
    void IcApp::addWindow(ctl::Sp<IcWindow> pWin)
    {
        auto pMng = IcWinMng::getInstance();
        pMng->addWindow(pWin);
    }
    //----------------------------
    //  onScreenSize
    //----------------------------
    void IcApp::onScreenSize(const ctl::TSize& sz)
    {
        auto pMng = IcWinMng::getInstance();
        pMng->onScreenSize(sz);
    }
    //----------------------------
    //  initWithScn
    //----------------------------
    void IcApp::initWithScn(ctl::Sp<IcScene> pScn)
    {
        auto pWin = makeSp<IcWindow>();
        pWin->addScene(pScn);
        addWindow(pWin);
    }
    //----------------------------------
    //  runCmd
    //----------------------------------
    int IcApp::runCmd(int argc, char **argv)
    {
        onInit();
        auto pMng = IcWinMng::getInstance();
        pMng->initMng(argc, argv);
        pMng->startMainLoop();
        return 0;
    }
    

}
