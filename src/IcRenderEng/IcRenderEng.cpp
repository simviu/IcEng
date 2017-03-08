//
//  IcRenderEng.cpp
//  DevEng
//
//  Created by Sherman Chen on 9/28/16.
//  Copyright (c) 2016 Simviu Technology Inc.
//  All rights reserved.
//  http://www.simviu.com/dev
//



#include "IcRenderEng.h"
#include "IcRenderAdpStd.h"
#include "OpenGLinc.h"

#ifndef USE_OPENGL1

namespace Ic3d
{
	using namespace std;
	using namespace ctl;

    static const string K_sFileVert		= "IcShaderVert.vsh";
    static const string K_sFileFrag		= "IcShaderFrag.vsh";
    static const bool K_enCulling = false;
	//--------------------------------------------------------------
	//	IcRenderEng Singleton
	//--------------------------------------------------------------
    IcRenderEng* IcRenderEng::m_pInstance = nullptr;
    IcRenderEng* IcRenderEng::getInstance()
    {
        if (!m_pInstance)
            m_pInstance = new IcRenderEng;
        return m_pInstance;
    };
    
	
	//--------------------------------------------------------------
	//	clearScreen
	//--------------------------------------------------------------
	void IcRenderEng::readFramePixel(int W, int H, void* ptr)const
	{
		glReadPixels(0, 0, W, H, GL_RGBA, GL_UNSIGNED_BYTE, (GLubyte*)ptr);
	}
	
	//--------------------------------------------------------------
	//	clearScreen
	//--------------------------------------------------------------
	void IcRenderEng:: clearScreen(const TColor& c)
	{
        glClearColor(0,0,0,0);	// TODO: configuration of color
        glClear(GL_DEPTH_BUFFER_BIT);
		glClearColor(c.r, c.g, c.b, c.a);	// background color
		glClear(GL_COLOR_BUFFER_BIT);
		
	}
	//--------------------------------------------------------------
	//	getGLErrCode
	//--------------------------------------------------------------
	int IcRenderEng::getGLErrCode() const
	{
		
		int errNum = glGetError();
		return errNum;
	}
	//--------------------------------------------------------------
	//	clearDepthBuf
	//--------------------------------------------------------------
	void IcRenderEng::clearDepthBuf()
	{
		glClearColor(0,0,0,0);	// TODO: configuration of color
		glClear(GL_DEPTH_BUFFER_BIT);
	}
    //--------------------------------------------------------------
    //	onFrameStart/onFrameEnd
    //--------------------------------------------------------------
    void IcRenderEng::onFrameStart()
    {
        glEnable(GL_DEPTH_TEST);    // tmp for Android debugging
       
    }
    void IcRenderEng::onFrameEnd()
    {


 
    }
    
	//--------------------------------------------------------------
	//	initEng
	//--------------------------------------------------------------
	bool IcRenderEng::initEng(const std::string& sPath_shader)
	{
		logInfo("IcRenderEng::initEng()");
        string sVert = sPath_shader + K_sFileVert;
        string sFrag = sPath_shader + K_sFileFrag;
		auto pAdp = ctl::makeSp<IcRenderAdpStd>();
		bool isOK = pAdp->loadShader(sVert, sFrag);
		if (!isOK)
		{
 			logErr("IcRenderEng::initEng() failed");
			return false;
		}
		m_pCurRenderAdp = pAdp;
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE0);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
        //---- Culling
        if(K_enCulling)
        {
            glFrontFace(GL_CCW);
            glEnable(GL_CULL_FACE);
            glCullFace(GL_BACK);
        }
        
		TMaterial dfltMat;
		m_pCurRenderAdp->applyMaterial(dfltMat);
		logInfo("IcEng::initEng() Done OK.");
		return true;
	}
    //--------------------------------------------------------------
    //	releaseEng
    //--------------------------------------------------------------
    void IcRenderEng::releaseEng()
    {
        logInfo("IcRenderEng::releaseEng()");
		m_pCurRenderAdp =nullptr;
        logInfo("IcRenderEng::releaseEng() done.");
     
    }

	//--------------------------------------------------------------
	//	setViewPort
	//--------------------------------------------------------------
	void IcRenderEng::setViewPort(const ctl::TRect& rect)
	{
		const auto sz = rect.getSize();
		glViewport(rect.pos0.x, rect.pos0.y, sz.w, sz.h);
	}
	
	
} // namespace
#endif // #ifndef USE_OPENGL1
