/*
 *  icube_tex.cpp
 *  B_O_Britain
 *
 *  Created by X. Chen on 9/4/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */



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
	};
	//----------------------------------------------------
	//	IcTexture
	//----------------------------------------------------
	CTexAdpStd::CTexAdpStd(const ctl::IcImg& img)
	{
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
		static int prevTexId = -1;
		
		//---- Optimize for not draw if previously drawed.
        // TODO: move to IcTexture
		if(prevTexId==m_texId)
			return;
		prevTexId = m_texId;
		
		//-----------------------------------------------
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

} // namespace Ic3d


