//
//  IcWinSys.cpp
//  DevEng
//
//  Created by Sherman Chen on 12/24/16.
//  Copyright (c) 2016 Simviu Technology Inc.
//  All rights reserved.
//  http://www.simviu.com/dev
//
//


#include "Ic3d.h"

namespace Ic3d
{
    using namespace ctl;
    using namespace std;
    //----------------------------
    //  IcWinMng Singleton
    //----------------------------
    ctl::Sp<IcWinMng> IcWinMng::m_pInstance = nullptr;
    ctl::Sp<IcWinMng> IcWinMng::getInstance()
    {
        if(m_pInstance==nullptr)
            m_pInstance = createWinMngImpl();
       //   m_pInstance = ctl::makeSp<IcWinMng>();
        return m_pInstance;
    }
    //---- Can be set externally
    void IcWinMng::setInstance(ctl::Sp<IcWinMng> p)
    {
        m_pInstance = p;
    }
    //-------------------------------------------
    //	onInitWindows
    //-------------------------------------------
    void IcWinMng::onInitWindows()
    {
        for(auto pWin : m_winAry.getAry())
            pWin->onInit();
    }

    //-------------------------------------------
    //	IcWinMng
    //-------------------------------------------
    void IcWinMng::drawUpdate(float deltaT)
    {
        for(auto pWin : m_winAry.getAry())
            pWin->onDrawUpdate(deltaT);
    }
    
    //-------------------------------------------
    //	onScreenSize
    //-------------------------------------------
    bool IcWinMng::onScreenSize(const ctl::TSize& screenSize)
    {
        m_screenSize = screenSize;
        for(auto pWin : m_winAry.getAry())
            pWin->onScreenSize(screenSize);
        return true;
    };
    //----------------------------
    //  onQuit
    //----------------------------
    void IcWinMng::onQuit()
    {
        m_winAry.clear();
    }
    
    
}
