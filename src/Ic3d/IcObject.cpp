//
//  IcObject.cpp
//  DevEng
//
//  Created by Sherman Chen on 3/11/16.
//  Copyright (c) 2016 Simviu Technology Inc.
//  All rights reserved.
//  http://www.simviu.com/dev
//

#include "Ic3d.h"
#include "IcRenderEng.h"

namespace Ic3d {
	
	//--------------------------------------------
	//	calcMat
	//--------------------------------------------
	TMat4 IcObject::calcMat() const
	{
		TMat4 m0;
		TMat4 ms = glm::scale(m0, m_scale);
		TMat4 mr = glm::mat4_cast(m_quat);
		TMat4 mt = glm::translate(m0, m_pos);

		return mt*mr*ms;
	}

	//--------------------------------------------
	//	draw
	//--------------------------------------------
	void IcObject::draw() const
	{
		if(m_pModel!=nullptr)
			m_pModel->draw();
	}
	
} // namespace Ic3d
