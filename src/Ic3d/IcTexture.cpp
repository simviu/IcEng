//
//  IcTexture.cpp
//  DevEng
//
//  Created by Sherman Chen on 9/20/16.
//  Copyright © 2016 Sherman Chen. All rights reserved.
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
	struct CTexHelper
	{
		ctl::Sp<CRenderAdp::CTexAdp> m_pDfltTexAdp = nullptr;
		auto getDfltTexAdp()-> decltype(m_pDfltTexAdp)
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
			auto p = pAdp->createTextureAdp(img);
			m_pDfltTexAdp = p;
			return p;
		}
	};
	static CTexHelper l_texHelper;
	//--------------------------------------
	//	Default Texture
	//--------------------------------------
	auto IcTexture::initWithDfltAdp()->decltype(m_pRenderAdp)
	{
		return  l_texHelper.getDfltTexAdp();
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

