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
        return m_pRenderAdp->m_isValid;
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

