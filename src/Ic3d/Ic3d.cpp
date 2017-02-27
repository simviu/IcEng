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
    bool IcEng::initEng(const std::string& sPathShader)
	{
		logInfo("IcEng::initEng()");
		auto pEng = IcRenderEng::getInstance();
		bool isOK = pEng->initEng(sPathShader);
        m_hasInit = true;
        if(isOK)
            m_isEnabled = true;
        return isOK;
	}
    //--------------------------------------------------------------
    //	releaseEng
    //--------------------------------------------------------------
    void IcEng::releaseEng()
    {
        m_pDfltTexAdp = nullptr; // TODO: move into RenderAdp for more auto
        m_isEnabled = false;
        m_hasInit = false;
        auto pRE = IcRenderEng::getInstance();
        pRE->releaseEng();
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

    //----------------------------
    // getDfltTexAdp
    //----------------------------
    auto IcEng::getDfltTexAdp()-> decltype(m_pDfltTexAdp)
    {
        //---- TODO: Texture auto resize 2^n
        if(m_pDfltTexAdp!=nullptr)
            return m_pDfltTexAdp;
        
        const static ctl::TSize K_dfltTexSize(64, 64);
        auto size = K_dfltTexSize;
        IcImg img(K_dfltTexSize);
        IcImg::TPixel c(255, 255, 255, 255);
        img.fillColor(c);
        
        auto pEng = IcRenderEng::getInstance();
        auto pAdp = pEng->getCurRenderAdp();
        if(pAdp==nullptr) return nullptr;
        auto p = pAdp->createTextureAdp(img);
        m_pDfltTexAdp = p;
        return p;
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
