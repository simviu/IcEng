//
//  IcWindow.cpp
//  DevEng
//
//  Created by Sherman Chen on 10/31/16.
//  Copyright (c) 2016 Simviu Technology Inc.
//  All rights reserved.
//  http://www.simviu.com/dev
//
//

#include "Ic3d.h"

namespace Ic3d
{
    using namespace ctl;
    using namespace std;
 
    //-------------------------------------------
    //  def
    //-------------------------------------------
    const static string K_sSubPath_shader = "IcShader/";
    //-------------------------------------------
    //	addScene/removeScene
    //-------------------------------------------
    void IcWindow::addScene(ctl::Sp<IcScene> pScn)
    {
        logInfo("IcWindow::addScene() ["+v2s(m_scnAry.size())+"]");
        m_scnAry.add(pScn);
        pScn->onWindowSize(m_cfg.m_size);
    };
    
    void IcWindow::removeAllScene()
    {
        logInfo("IcWindow::removeAllScene() -- "+
                        v2s(m_scnAry.size())+" scenes");
        m_scnAry.clear();
    }
    
    //-------------------------------------------
    //	onInit
    //-------------------------------------------
    void IcWindow::onInit()
    {
        logInfo("IcWindow::onInit()");
        //--- Init Eng
        auto pEng = IcEng::getInstance();
        string sPathRes = IcApp::getInstance()->m_cfg.m_sPathRes;
        pEng->initEng(sPathRes + K_sSubPath_shader);
        
    }
    //-------------------------------------------
    //	onRelease
    //-------------------------------------------
    void IcWindow::onRelease()
    {
        logInfo("IcWindow::onRelease()");
        removeAllScene();
    }
    //-------------------------------------------
    //	onWindowSize
    //-------------------------------------------
    void IcWindow::onWindowSize(const ctl::TSize& size)
    {
        logInfo("IcWindow::onWindowSize() ["+
            v2s(size.w) + "x"+ v2s(size.h) +"]");
        // In default, call onWindowSize().
        // TODO:
        //    Consider add option to fit window into screen
        //    based on normalized coordinator, etc.
        m_cfg.m_size = size;
        for(auto pScn : m_scnAry.getAry())
            pScn->onWindowSize(size);
    }
    //-------------------------------------------
    //	initWindow()/releaseWindow()
    //-------------------------------------------
    void IcWindow::initWindow()
    {
        //---- Request onInit(), will be called inside drawUpdate()
        m_hasInit = false;
    }
    void IcWindow::releaseWindow()
    {
        onRelease();
        //---- Release Eng
        auto pEng = IcEng::getInstance();
        pEng->releaseEng();
    }

    //-------------------------------------------
    //	onDrawUpdate
    //-------------------------------------------
    void IcWindow::onDrawUpdate(float deltaT)
    {
        //---- Thread protection
        std::unique_lock<std::mutex> lock(m_mtx_draw);
		if(m_isDrawing) return;
		m_isDrawing = true;
        //-------------
        // Check Init
        //-------------
        if(!m_hasInit)
        {
            onInit();
            m_hasInit = true;
            m_isDrawing = false;
            return;
        }
        //-------------
        // Frame Start
        //-------------
        auto pEng = IcEng::getInstance();
        pEng->onFrameStart();
        pEng->clearScreen(m_cfg.m_bkColor); // TODO: cfg of not clearScreen

        //-------------
        // Draw Update Scene
        //-------------
        for(auto pScn : m_scnAry.getAry())
            pScn->onUpdate(deltaT);

        for(auto pScn : m_scnAry.getAry())
            pScn->onDraw();

        pEng->onFrameEnd();
		m_isDrawing = false;

    }
    //-------------------------------------------
    //	onKeyboard
    //-------------------------------------------
    void IcWindow::onKeyboard(unsigned char key)
    {
        switch (key) {
                break;
            default:
                break;
        }
    }
 
}
