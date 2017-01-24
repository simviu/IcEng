//
//  IcScene.cpp
//  DevEng
//
//  Created by Sherman Chen on 5/10/16.
//  Copyright (c) 2016 Sherman Chen. All rights reserved.
//

#include "Ic3d.h"
#include "IcRenderEng.h"
inline namespace Ic3d
{
	using namespace std;
	using namespace ctl;

    
    const static TVec3 K_camDfltPos(0,0,0);
    const static TVec3 K_camLookAt(0,0,-1);
    const static TVec3 K_camUpVec(0,1,0);
   //-----------------------------------------
    //	IcScene
    //-----------------------------------------
    IcScene::IcScene()
    {
        //---- Default Light
        m_lights.add(ctl::makeSp<IcLight>(0));
        m_pCamera->setPos(K_camDfltPos);
        m_pCamera->lookAt(K_camLookAt, K_camUpVec);
    }


	//---------------------------------------------
	//	onViewSize()
	//---------------------------------------------
	void IcScene::onViewRect(const TRect& viewRect)
	{
        m_cfg.m_viewRect = viewRect;
		ctl::TSize viewSize = viewRect.getSize();
        const auto& camCfg = m_cfg.m_camCfg;
		m_pCamera->setFrustum(viewSize, camCfg);
		
	}

	//---------------------------------------------
	//	initCamera()
	//---------------------------------------------
	void IcScene::initCamera(const TRect& viewRect)
	{
		ctl::TSize viewSize = viewRect.getSize();
        const auto& camCfg = m_cfg.m_camCfg;
        m_pCamera->setFrustum(viewSize, camCfg);
	}
	
    //-----------------------------------------
    //	drawLight
    //-----------------------------------------
    void IcScene::drawLights()
    {
        auto pEng = IcRenderEng::getInstance();
        auto pAdp = pEng->getCurRenderAdp();
        
        auto mat = m_pCamera->getViewMat();
        const size_t N = m_lights.size();
        for(auto pLight : m_lights.getAry())
        {
            TVec4 v0(pLight->getPos(),1); v0 = mat * v0;
            TVec4 v1(pLight->getDir(),0); v1 = mat * v1;
            TVec3 ecPos(v0.x, v0.y, v0.z);
            TVec3 ecDir(v1.x, v1.y, v1.z);
            pAdp->setLight(pLight->getPara(), ecPos, ecDir,
                           pLight->getLightId(), N);
        }
        
    }

	
	//-----------------------------------------
	//	onDraw
	//-----------------------------------------
	void IcScene::onDraw()
	{
        //-----------------
        //	Check Init
        //-----------------
        if(!m_hasInit)
        {
            onInit();
            m_hasInit = true;
            return;
        };
        
        
        //----------------------
		static int dbgFrmCnt=0;
		dbgFrmCnt ++;
		auto pRE= IcRenderEng::getInstance();
        pRE->setViewPort(m_cfg.m_viewRect);
		
		//-----------------
		//	Draw Light
		//-----------------
        drawLights();
        
		//-----------------
		//	Draw Obj
		//-----------------
		m_pCamera->drawObj(m_rootObj);
		m_frmCnt ++;
	}
} // namespace Ic3d
