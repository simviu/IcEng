//
//  IcCamera.cpp
//  DevEng
//
//  Created by Sherman Chen on 3/8/16.
//  Copyright (c) 2016 Sherman Chen. All rights reserved.
//


#include "Ic3d.h"
#include "IcRenderEng.h"

namespace Ic3d
{
	using namespace std;
	using namespace ctl;
	//-------------------------------------------
	//	CameraHelper
	//-------------------------------------------
	struct CameraHelper
	{
		//---------------------------
		static TMat4 setOrtho_LRBT(float left, float right,
								   float bottom, float top,
								   float zNear, float zFar)
		{	return glm::ortho(left, right,
							  bottom, top,
							  zNear, zFar);
		}
		
		//---------------------------
		static TMat4 setFrustum_LRBT(float left, float right,
									 float bottom, float top,
									 float zNear, float zFar)
		{	return glm::frustum(left, right,
								bottom, top,
								zNear, zFar);
		}
	};
	
	//-------------------------------------------
	//	setFrustum
	//-------------------------------------------
	void IcCamera::setFrustum(const ctl::TSize& viewSize,
							  const TCfg& cfg)
	{
		if(viewSize.h ==0) return;
		//----- Consider the landscape mode
        // Note:
		float fx = cfg.m_zNear * tanf(deg2rad(cfg.m_FOV)/2.0);
		float fy = fx*viewSize.h/viewSize.w;
		
		m_matProj = CameraHelper::setFrustum_LRBT(-fx, fx, -fy, fy,
												  cfg.m_zNear, cfg.m_zFar);
	}
	//-------------------------------------------
	//	lookAt
	//-------------------------------------------
	void IcCamera::lookAt(const TVec3& pos,
						  const TVec3& vUp)
	{
		m_matView = glm::lookAt(m_pos, pos, vUp);
	}
    //-------------------------------------------
    //	updateViewMat
    //-------------------------------------------
    void IcCamera::updateViewMat()
    {
       // auto m = calcMat();
        TMat4 m;
        auto pos = -getPos();
        auto mT = glm::translate(m, pos);
        auto q = glm::inverse(getQuat());
        auto mR = TMat4(q);
        m_matView = mR * mT;
    }
    
	//-------------------------------------------
	//	drawObj
	//-------------------------------------------
	void IcCamera::drawObj(const Ic3d::IcObject &rootObj) const
	{
		drawObjTree(rootObj, TMat4());
	}
	//-----------------------------------------
	//	drawText
	//-----------------------------------------
	void IcCamera::drawText(const string& str,
							const TColor& c,
							const TVec3& pos)
	{
		auto pEng = IcRenderEng::getInstance();
        auto pAdp = pEng->getCurRenderAdp();
		CRenderAdp::TRenderMatrix rm;
		rm.m_matProj =m_matProj;
		TMat4 m;
		m = glm::translate(m, pos);
		rm.m_matModel = m;
		rm.m_matView = m_matView;
		TVec2 pos1(0,0);
        // TODO: not implemented
	//	pRIF->applyMatrix(rm);
	//	pRIF->drawText(str, c, pos1);
	}
	
	//-------------------------------------------
	//	drawObj
	//-------------------------------------------
	void IcCamera::drawObjTree(const IcObject& obj,
							   const TMat4& matModelParent) const
	{
		auto pRIF = IcRenderEng::getInstance();
        auto pAdp = pRIF->getCurRenderAdp();
		//----- Draw this
		TMat4 matModel = matModelParent * obj.calcMat();
		CRenderAdp::TRenderMatrix rm;
		rm.m_matProj =m_matProj;
		rm.m_matModel = matModel;
		rm.m_matView = m_matView;
		pAdp->applyMatrix(rm);
		obj.draw();
		
		//---- Draw Childs
		auto& childs = obj.getChildObjs();
		for(auto pObj : childs.all())
			drawObjTree(*pObj, matModel);
	}
	
	
	
}
