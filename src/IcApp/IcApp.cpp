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
    
    static IcApp* l_pIcAppInstance = nullptr;
    //----------------------------
    //  IcApp
    //----------------------------
    IcApp::IcApp()
    {
      //  setInstance(this);
    };
    IcApp::~IcApp()
    {
        logInfo("IcApp::~IcApp()");
    }

    //----------------------------
    //  singleton
    //----------------------------
    void IcApp::setInstance(IcApp* pApp)
    {
        //---- Set as default IcApp
        if(l_pIcAppInstance!=nullptr)
            logWarn("IcApp::IcApp() : In most case there should be only one IcApp instanciation");
       l_pIcAppInstance = pApp;
    }
    IcApp* IcApp::getInstance()
    {
        return l_pIcAppInstance;
    }

    //----------------------------
    //  get winMng singleton
    //----------------------------
    ctl::Sp<IcWinMng> IcApp::getWinMng()
    {
        return IcWinMng::getInstance();
    }
    //----------------------------
    // onInit()/onRelease()
    //----------------------------
    void IcApp::onInit()
    {
        logInfo("IcApp::onInit()");
    };
    void IcApp::onRelease()
    {
        logInfo("IcApp::onRelease()");
    };

    //----------------------------
    //  addWindow
    //----------------------------
    void IcApp::addWindow(ctl::Sp<IcWindow> pWin)
    {
        auto pMng = IcWinMng::getInstance();
        pMng->addWindow(pWin);
    }

 
    
    

}
