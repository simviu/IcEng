//
//  IcTexture.cpp
//  DevEng
//
//  Created by Sherman Chen on 9/20/16.
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

	//--------------------------------------
	//	Texture
	//--------------------------------------
	IcTexture::IcTexture()
	{
		m_pRenderAdp =  IcEng::getInstance()->getDfltTexAdp();
	}
    IcTexture::IcTexture(const ctl::TSize& sz)
    {
        auto pEng = IcRenderEng::getInstance();
        auto pAdp = pEng->getCurRenderAdp();
        if(pAdp==nullptr) return;
        m_pRenderAdp = pAdp->createTextureAdp(sz);

    }


	//--------------------------------------
	//	loadFile
	//--------------------------------------
    bool IcTexture::loadFile(const std::string& sFile)
	{
        auto pEng = IcRenderEng::getInstance();
        auto pAdp = pEng->getCurRenderAdp();
        if(pAdp==nullptr) return false;
		m_pRenderAdp = pAdp->createTextureAdp(sFile);
        if(m_pRenderAdp==nullptr) return false;
        return m_pRenderAdp->isValid();
	}
	//--------------------------------------
	//	IcTexture
	//--------------------------------------
	IcTexture::IcTexture(const ctl::IcImg& rImg)
	{
        auto pEng = IcRenderEng::getInstance();
        auto pAdp = pEng->getCurRenderAdp();
        if(pAdp==nullptr) return;
		m_pRenderAdp = pAdp->createTextureAdp(rImg);
	}
    //--------------------------------------
    //	IcTexture
    //--------------------------------------
    TSize IcTexture::getSize() const
    {
        if(m_pRenderAdp==nullptr) return TSize(0,0);
        return m_pRenderAdp->getSize();

    }

    //--------------------------------------
    //	Render Texture
    //--------------------------------------
    bool IcTexture::setAsRenderTarget()
    {
        if(m_pRenderAdp==nullptr) return false;
        return m_pRenderAdp->setAsRenderTarget();
        
    }
    void IcTexture::startRenderOn() const
    {
        if(m_pRenderAdp==nullptr) return;
        m_pRenderAdp->startRenderOn();
    
    }
    void IcTexture::finishRenderOn() const
    {
        if(m_pRenderAdp==nullptr) return;
        m_pRenderAdp->finishRenderOn();
    }

	
} // namespace Ic3d

