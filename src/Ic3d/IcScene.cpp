//
//  IcScene.cpp
//  DevEng
//
//  Created by Sherman Chen on 5/10/16.
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

    
    const static TVec3 K_camDfltPos(1,3,5);
    const static TVec3 K_camLookAt(0,0,0);
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
	//	onWindowSize()
	//---------------------------------------------
	void IcScene::onWindowSize(const ctl::TSize& winSize)
    {
        if(!m_cfg.m_enAutoResize) return;
        //---- Recalc Scene Rect
        const auto& w = winSize.w;
        const auto& h = winSize.h;
        const auto& rn = m_cfg.m_viewRectNorm;
        TPos npos0 = TPos(rn.pos0.x * w, rn.pos0.y * h);
        TPos npos1 = TPos(rn.pos1.x * w, rn.pos1.y * h);
        TRect r{npos0, npos1};
        setViewRect(r);
        //---- Recursive call sub scenes
        for(auto pScn : m_subScns.getAry())
            pScn->onWindowSize(winSize);
    }
    //---------------------------------------------
    //	setViewRect
    //---------------------------------------------
    void IcScene::setViewRect(const ctl::TRect& r)
	{
        m_cfg.m_viewRect = r;
   //     const auto& camCfg = m_cfg.m_camCfg;
        
        //---- TODO: move to drawUpdate()?
	//	m_pCamera->setFrustum(r.getSize(), camCfg);
        m_pCamera->m_cfg.m_viewSize = r.getSize();
        
	}
    //---------------------------------------------
    //	setRenderToTexture
    //---------------------------------------------
    void IcScene::setRenderToTexture(ctl::Sp<IcTexture> pTex)
    {
        if(pTex==nullptr) return;
        auto sz = pTex->getSize();
        setViewRect(TRect(TPos(0,0), sz));
        m_pTargetTex = pTex;
    //  pTex->setAsRenderTarget();    // call this outside
        m_cfg.m_enAutoResize = false; // Disable auto resize, set outside
        m_cfg.m_enClrScrn = true;
    };

    /*
	//---------------------------------------------
	//	initCamera()
	//---------------------------------------------
	void IcScene::initCamera(const TRect& viewRect)
	{
		ctl::TSize viewSize = viewRect.getSize();
        const auto& camCfg = m_cfg.m_camCfg;
        m_pCamera->setFrustum(viewSize, camCfg);
        
        
	}
     */
	
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

    //-------------------------------------------
    //	renderObjRecur
    //-------------------------------------------
    void IcScene::renderObjRecur(const IcCamera& cam,
                                 const IcObject& obj,
                                 const TMat4& matModelParent) const
    {
        auto pRIF = IcRenderEng::getInstance();
        auto pAdp = pRIF->getCurRenderAdp();
        //----- Draw this
        TMat4 matModel = matModelParent * obj.calcMat();
        CRenderAdp::TRenderMatrix rm;
        rm.m_matModel = matModel;
        rm.m_matView  = cam.getViewMat();
        rm.m_matProj  = cam.getProjMat();
        pAdp->applyMatrix(rm);
        obj.draw();
        
        //---- Draw Childs
        auto& childs = obj.getChildObjs();
        for(auto pObj : childs.all())
            renderObjRecur(cam, *pObj, matModel);
    }
    //-----------------------------------------
    //  addSubScn
    //-----------------------------------------
    void IcScene::addSubScn(ctl::Sp<IcScene> pScn)
    {
        pScn->onWindowSize(m_cfg.m_viewRect.getSize());
        m_subScns.add(pScn);
    };
    
    //-----------------------------------------
    //	renderSubScns
    //-----------------------------------------
    // Called by onDraw()
    void IcScene::renderSubScns()
    {
        for(auto pScn : m_subScns.getAry())
            pScn->onDraw();
    }
  
    //-----------------------------------------
    //	renderThis
    //-----------------------------------------
    // Called by onDraw()
    void IcScene::renderThis()
    {
        
        auto pRE= IcRenderEng::getInstance();
        pRE->setViewPort(m_cfg.m_viewRect);
        //----------------------
        // Check clear Screen
        //----------------------
        if(m_cfg.m_enClrScrn)
            pRE->clearScreen(m_cfg.m_bkColor);
        
        //----------------------
        //---- Set fog
        //----------------------
        auto pAdp = pRE->getCurRenderAdp();
        pAdp->setFog(m_cfg.m_fogPara);
        
        //-----------------
        //	Draw Light
        //-----------------
        drawLights();
        
        //-----------------
        //	Draw Obj
        //-----------------
        //	m_pCamera->drawObj(m_rootObj);
        renderObjRecur(*m_pCamera, m_rootObj, TMat4());
        //-----------------
        //	Draw Text
        //-----------------
        for(auto pText : m_texts.getAry())
            pText->onDraw();
        
        
    }
	
	//-----------------------------------------
	//	onDraw
	//-----------------------------------------
	void IcScene::onDraw()
	{
        //----------------------
        // Check IcEng
        auto pEng = IcEng::getInstance();
        if(!pEng->isEnabled()) return;
        //----------------------
        // Check onInit()
        //----------------------
        if(!m_hasInit)
        {
            onInit();
            m_hasInit = true;
            return;
        }
 
        //----------------------
        // Check Render To texture
        //----------------------
        if(m_pTargetTex!=nullptr)
            m_pTargetTex->startRenderOn();

        //----------------------
        // Render
        //----------------------
        if(m_subScns.size()>0)
                renderSubScns();
        else    renderThis();
        //----------------------
        // Check Render To texture (finish)
        //----------------------
        if(m_pTargetTex!=nullptr)
            m_pTargetTex->finishRenderOn();
        
        m_frmCnt ++;
	}
} // namespace Ic3d
