//
//  Ic3d.cpp
//  DevEng
//
//  Created by Sherman Chen on 3/11/16.
//  Copyright (c) 2016 Simviu Technology Inc.
//  All rights reserved.
//  http://www.simviu.com/dev
//



#include "Ic3d.h"
#include "IcRenderEng.h"

namespace Ic3d
{
    using namespace std;
    using namespace ctl;
    //----------------------------------------------------
    //	def
    //----------------------------------------------------
    static ctl::Sp<IcEng> l_pInstance = nullptr;
    ctl::Sp<IcEng> IcEng::getInstance()
    {
        if(l_pInstance!=nullptr) return l_pInstance;
        l_pInstance = ctl::makeSp<IcEng>();
        return l_pInstance;
    }
	//--------------------------
	//	initEng
	//--------------------------
    bool IcEng::initEng()
	{
		logInfo("IcEng::initEng()");
		auto pEng = IcRenderEng::getInstance();
		bool isOK = pEng->initEng(m_cfg.m_sPath_shader);
        m_hasInit = true;
        if(isOK)
            m_isEnabled = true;
        return isOK;
	}
    //--------------------------------------------------------------
    //	onFrameStart/onFrameEnd
    //--------------------------------------------------------------
    void IcEng::onFrameStart()
    {
        auto pRE = IcRenderEng::getInstance();
        pRE->onFrameStart();
    }
    void IcEng::onFrameEnd()
    {
        auto pRE = IcRenderEng::getInstance();
        pRE->onFrameEnd();
       
    }
    //--------------------------
    //	clearScreen
    //--------------------------
    void IcEng::clearScreen(const TColor& bkColor)
    {
        auto pRE= IcRenderEng::getInstance();
        pRE->clearScreen(bkColor);

    }
    //----------------------------
    // getCurRenderAdp
    //----------------------------
    ctl::Sp<CRenderAdp> IcEng::getCurRenderAdp()
    {
        auto pRE= IcRenderEng::getInstance();
        return pRE->getCurRenderAdp();
    }

    /*
    //----------------------------
    // IcFrameRateCalc::onNewFrame
    //----------------------------
    void IcFrameRateCalc::onNewFrame()
    {
        auto t = std::chrono::steady_clock::now();
        // TODO: LIMIT deltaT to 1 sec
        auto tDiff = t- m_tFrame;
        m_tFrame = t;
        auto N = std::chrono::duration_cast<TMs>(tDiff).count();
        m_deltaT = (double)N/1000000.0;
        if(m_deltaT> K_deltaT_max)
            m_deltaT = K_deltaT_max;
    };
     */
}
