//
//  IcMaterial.cpp
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
	//-----------------------------------
	//	draw
	//-----------------------------------
	void IcMaterial::draw() const
	{
		auto pEng = IcRenderEng::getInstance();
        auto pAdp = pEng->getCurRenderAdp();
		pAdp->applyMaterial(*this);
	}

}
