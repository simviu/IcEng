//
//  IcRenderAdpStd_Tex.cpp
//  DevEng
//
//  Created by Sherman Chen on 9/28/16.
//  Copyright (c) 2016 Simviu Technology Inc.
//  All rights reserved.
//  http://www.simviu.com/dev
//



#include "IcRenderAdpStd.h"
#include "OpenGLinc.h"

namespace Ic3d
{
	using namespace std;
	using namespace ctl;
	//----------------------------------------------------
	//	~CTexAdpStd
	//----------------------------------------------------
	CTexAdpStd::~CTexAdpStd()
	{
		//---- Release texture ID
		GLuint tid = m_texId;
		if(m_isValid)
			glDeleteTextures(1, &tid);
        if(m_R2T_cfg.m_isValid)
            glDeleteFramebuffers(1, &m_R2T_cfg.m_frmBufId);
	};
	//----------------------------------------------------
	//	IcTexture
	//----------------------------------------------------
	CTexAdpStd::CTexAdpStd(const ctl::IcImg& img)
	{
        auto sz0 = img.size();
        auto sz1 = calcValidSizeSquare(sz0);
        const auto& rImg = img;
        if(sz0!=sz1)
        {
            IcImg img1; img1.copy(img);
            importFromBuf(img1.size(), img1.getBuf());
        }
        importFromBuf(img.size(), img.getBuf());
		putTexIntoGL();
	}
	
	//----------------------------------------------------
	//	loadFile
	//----------------------------------------------------
	bool CTexAdpStd::loadFile(const string& fname)
	{
		string fext = ctl::sFileExt(fname);
		fext = ctl::s2lower(fext);
		bool isOK = false;
		if(fext=="pvrtc" || fext=="pvr")
		 isOK = loadFilePVR(fname);
		else isOK = loadFileOther(fname);
		
		m_isValid = isOK;
		if(!isOK)
		{
			ctl::logErr("Load Texture Failed:["+fname+"]");
			return false;
		}
		
		putTexIntoGL();
		m_sName = fname;
		return true;
	}
	//----------------------------------------------------
	//	loadFileOther
	//----------------------------------------------------
	bool CTexAdpStd::loadFileOther(const string& fname)
	{
		m_sName = sFileNoPath(fname);
		ctl::IcImg img;
		m_isValid = img.loadFile(fname);
		if(!m_isValid)
			return false;
        //---- Make sure size correct
        auto sz0 = img.size();
        auto sz1 = calcValidSizeSquare(sz0);
        if(sz0!=sz1)
            img.scaleTo(sz1);
        
		importFromBuf(img.size(), img.getBuf());
		return true;
		
	}
	
	//----------------------------------------------------
	//	loadFromBuf
	//----------------------------------------------------
	void CTexAdpStd::importFromBuf(const ctl::TSize& size,
								  const BinBuf& buf)
	{
		
		auto pData = ctl::makeSp<TMipMapData>(size, buf);
		m_aryMipData.add(pData);
		
	}
	
	//----------------------------------------------------
	bool CTexAdpStd::putTexIntoGL()
	{
		m_texId = genTexId();
		bool isOK = true;
		for (auto pData : m_aryMipData.getAry())
		{
			if(m_isTexPVR)
					isOK &= putIntoGL_Compressed(*pData);
			else	isOK &= putIntoGL(*pData);
			if(!isOK)
				return false;
		}
		return isOK;
	}
	
	
	
	//----------------------------------------------------
	//	genTexId
	//----------------------------------------------------
	unsigned int	CTexAdpStd::genTexId()
	{
		int gl_tex_id;
		glGenTextures(1, (GLuint*)(&gl_tex_id));
		return gl_tex_id;
	}
	
	//----------------------------------------------------
	//	putIntoGL
	//----------------------------------------------------
	bool CTexAdpStd::putIntoGL(const TMipMapData& rData)
	{
		glBindTexture(GL_TEXTURE_2D, m_texId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
					 rData.m_size.w, rData.m_size.h,
					 0, GL_RGBA, GL_UNSIGNED_BYTE,
					 rData.m_buf.getBuf());
		auto errId = glGetError();
		return (errId==GL_NO_ERROR);
	}
	
	//----------------------------------------------------
	//	draw
	//----------------------------------------------------
	void CTexAdpStd::render() const
	{

		glBindTexture(GL_TEXTURE_2D, m_texId);
		if(m_isRepeat)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		}
		else
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}
		
		//-----------------------------------------------
		if(m_isTexMipMap)
		{
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 1.0);	// TODO: could be 2.0
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
		else
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
		
	}
	
	//------------------------------------------------
	//	putIntoGL_Compressed
	//------------------------------------------------
	bool	CTexAdpStd::putIntoGL_Compressed(const TMipMapData& rData)
	{
#ifdef __APPLE__
		
		glBindTexture(GL_TEXTURE_2D, m_texId);
		auto errId = glGetError();
		glCompressedTexImage2D(GL_TEXTURE_2D, rData.m_level,
							   m_PVR_format,
							   rData.m_size.w, rData.m_size.h, 0,
							   rData.m_buf.size(),
							   rData.m_buf.getBuf());
		
		errId = glGetError();
		return (errId==GL_NO_ERROR);
		
		//----- Not suuported for none APPLE
#else // #ifdef __APPLE__
		
		return false;
#endif // #ifdef __APPLE__
		
		
	}
    //------------------------------------------------
    //	calcValidSizeSquare
    //------------------------------------------------
    ctl::TSize CTexAdpStd::calcValidSizeSquare(const ctl::TSize& sizeOri)
    {
        const static int N0 = 6;    // Min 64
        const static int N1 = 14;   // Max 16384
        
        TSize sz = sizeOri;
        int l = 16;
        for(int i=N0;i<=N1;i++)
        {
            l = pow(2,i);
            if((l>=sz.w)&&(l>=sz.h))
                break;
        }
        sz.w = sz.h = l;
        return sz;
    }

    //------------------------------------------------
    //	setAsRenderTarget
    //------------------------------------------------
    // Ref : http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-14-render-to-texture/
    bool CTexAdpStd::setAsRenderTarget(const ctl::TSize& sizeIn)
    {
        m_size = calcValidSizeSquare(sizeIn);
        //---- Save original frame buffer
        glGetIntegerv(GL_FRAMEBUFFER_BINDING, &m_R2T_cfg.m_frmBufIdOri);
        //---- Gen FrameBuf
        auto& cfg = m_R2T_cfg;
        cfg.m_frmBufId = 0;
        glGenFramebuffers(1, &cfg.m_frmBufId);
        glBindFramebuffer(GL_FRAMEBUFFER, cfg.m_frmBufId);
        
        //---- Normal Texture setup
        m_texId = genTexId();
        glBindTexture(GL_TEXTURE_2D, m_texId);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                     m_size.w, m_size.h,
                     0, GL_RGBA, GL_UNSIGNED_BYTE,
                     0);    // last para is empty buffer ptr
    //    glBindTexture(GL_TEXTURE_2D, 0);    // unbind it for safe
        // Poor filtering. Needed !
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        //----------------------
        // Also need depth buffer
        //-------------------------
        // The depth buffer
        glGenRenderbuffers(1, &cfg.m_depthBufId);
        glBindRenderbuffer(GL_RENDERBUFFER, cfg.m_depthBufId);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT,
                              m_size.w, m_size.h);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER,
                                  cfg.m_depthBufId);
        //-------------------------
        // Configure as render target
        //-------------------------
        // Set "renderedTexture" as our colour attachement #0
#if TARGET_OS_IPHONE
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texId, 0);
#else
        glFramebufferTextureEXT(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_texId, 0);
#endif
        // Set the list of draw buffers.
        GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
        glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers
        
         // Always check that our framebuffer is ok
        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            return false;
        m_R2T_cfg.m_isValid = true;
        return true;
    }
    //------------------------------------------------
    //	startRenderOn/finishRenderOn
    //------------------------------------------------
    void CTexAdpStd::startRenderOn()
    {
 
        //---- Note: this call should be at before glViewPort
        glBindFramebuffer(GL_FRAMEBUFFER, m_R2T_cfg.m_frmBufId);
    }
    void CTexAdpStd::finishRenderOn()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_R2T_cfg.m_frmBufIdOri);
      //glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

} // namespace Ic3d


