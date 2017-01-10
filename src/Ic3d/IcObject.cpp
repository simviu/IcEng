/*
 *  icube_texture.cpp
 *  AppleCoder-OpenGLES-00
 *
 *  Created by X. Chen on 8/14/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */


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
