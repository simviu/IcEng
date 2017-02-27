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
	//	CTexHelper
	//--------------------------------------
    /*
	struct CTexHelper
	{
		ctl::Sp<CRenderAdp::CTexAdp> m_pDfltTexAdp = nullptr;
	};
	static CTexHelper l_texHelper;
     */
	//--------------------------------------
	//	Default Texture
	//--------------------------------------
	auto IcTexture::initWithDfltAdp()->decltype(m_pRenderAdp)
	{
        return  IcEng::getInstance()->getDfltTexAdp();
	}


	//--------------------------------------
	//	IcTexture
	//--------------------------------------
	IcTexture::IcTexture(const string& sFile)
	{
        auto pEng = IcRenderEng::getInstance();
        auto pAdp = pEng->getCurRenderAdp();
        if(pAdp==nullptr) return;
		m_pRenderAdp = pAdp->createTextureAdp(sFile);
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
	
} // namespace Ic3d

