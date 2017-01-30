//
//  IcRenderAdpStd_TexPVR.cpp
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

    //-----------------------------------------
	//	PVRTC
	//-----------------------------------------
	
#define PVR_TEX_FLAG_TYPE_MASK		0xff
#define	PVR_TEX_FLAG_TYPE_PVRTC_2	24
#define	PVR_TEX_FLAG_TYPE_PVRTC_4	25
	
	static char PVR_TEX_IDENT_TAG[10] = "PVR!";
	//-----------------------------------------
	//	PVRTC header
	//-----------------------------------------
	typedef struct
	{
		unsigned int headerLength;
		unsigned int height;
		unsigned int width;
		unsigned int numMipmaps;
		unsigned int flags;
		unsigned int dataLength;
		unsigned int bpp;
		unsigned int bitmaskRed;
		unsigned int bitmaskGreen;
		unsigned int bitmaskBlue;
		unsigned int bitmaskAlpha;
		unsigned int pvrTag;
		unsigned int numSurfs;
	} t_PVR_tex_header;
	
	//------------------------------------------------
	//	endian swap
	//------------------------------------------------
	//#define	ENDIAN_SWAP32(din)	endian_swap32(din)	//---- swap
#define		ENDIAN_SWAP32(din)	din					//---- no sawp
	
	static unsigned int endian_swap32(unsigned int din)
	{
		unsigned int c0,c1,c2,c3;
		c0 = (din>>0 )&0xff;
		c1 = (din>>8 )&0xff;
		c2 = (din>>16 )&0xff;
		c3 = (din>>24 )&0xff;
		unsigned int dout =
		(c0<<24) |
		(c1<<16) |
		(c2<<8) |
		(c3<<0) ;
		return dout;
		
	}
	//------------------------------------------------
	//	Load PVR texture file
	//------------------------------------------------
#define MAX(d0,d1) (d0>d1?d0:d1)
	bool CTexAdpStd::loadFilePVR(const string& fname)
	{
#if TARGET_OS_IPHONE
		FILE *fp = fopen(fname.c_str(), "rb");
		if(!fp)
		{
			logErr("Fail to read file '"+fname+"'");
			m_isValid = false;
			return false;
		}
		
		t_PVR_tex_header header;
		fread((void*)(&header), sizeof(header), 1, fp);
		
		unsigned w,h;
		unsigned flags, pvrTag;
		unsigned dataLength = 0, dataOffset = 0, dataSize = 0;
		unsigned blockSize = 0, widthBlocks = 0, heightBlocks = 0;
		unsigned bpp = 4;
		unsigned formatFlags;
		
		
		pvrTag = ENDIAN_SWAP32(header.pvrTag);
		
		if (PVR_TEX_IDENT_TAG[0] != ((pvrTag >>  0) & 0xff) ||
			PVR_TEX_IDENT_TAG[1] != ((pvrTag >>  8) & 0xff) ||
			PVR_TEX_IDENT_TAG[2] != ((pvrTag >> 16) & 0xff) ||
			PVR_TEX_IDENT_TAG[3] != ((pvrTag >> 24) & 0xff))
		{
			string s = string(__FUNCTION__)+fname+" is not a PVRTC file\n";
			logErr(s);
			m_isValid = false;
			return false;
		}
		
		flags = ENDIAN_SWAP32(header.flags);
		formatFlags = flags & PVR_TEX_FLAG_TYPE_MASK;
		
		
		//-------------------------------------------------------------
		//---- check flag
		if (formatFlags == PVR_TEX_FLAG_TYPE_PVRTC_4)
			m_PVR_format = GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG;
		else if (formatFlags == PVR_TEX_FLAG_TYPE_PVRTC_2)
			m_PVR_format = GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG;
		else
		{
			string s = string(__FUNCTION__)+fname+" PVR format flags unkown\n";
			logErr(s);
			m_isValid = false;
			return false;
		}
		
		//-------------------------------------------------------------
		
		//---- get size
		w=m_size.w = ENDIAN_SWAP32(header.width);
		h=m_size.h = ENDIAN_SWAP32(header.height);
		
		//---- get has_alpha
		/*
		 if (ENDIAN_SWAP32(header.bitmaskAlpha))
			PVR_has_alpha = true;
		 else	PVR_has_alpha = false;
		 */
		
		//---- data length
		dataLength = ENDIAN_SWAP32(header.dataLength);
		
		int lvl = 0; // TODO: may change initial level
		//---- read each level
		while (dataOffset < dataLength)
		{
			//---- calc size
			if (formatFlags == PVR_TEX_FLAG_TYPE_PVRTC_4)
			{
				blockSize = 4 * 4; // Pixel by pixel block size for 4bpp
				widthBlocks = w / 4;
				heightBlocks = h / 4;
				bpp = 4;
			}
			else
			{
				blockSize = 8 * 4; // Pixel by pixel block size for 2bpp
				widthBlocks = w / 8;
				heightBlocks = h / 4;
				bpp = 2;
			}
			
			// Clamp to minimum number of blocks
			if (widthBlocks < 2)
				widthBlocks = 2;
			if (heightBlocks < 2)
				heightBlocks = 2;
			
			dataSize = widthBlocks * heightBlocks * ((blockSize  * bpp) / 8);
			
			//--- create data and read from file
			auto pData = ctl::makeSp<TMipMapData>(TSize(w,h),dataSize);
			fread(pData->m_buf.getBuf(), pData->m_buf.size(), 1, fp);
			
			pData->m_level = lvl;
			m_aryMipData.add(pData);
			
			//---- Set new width/height for next level
			w = MAX(w >> 1, 1);
			h = MAX(h >> 1, 1);
			lvl++;
			dataOffset += dataSize;
			
		}
		
		m_isTexPVR = true;
		if(lvl>0) m_isTexMipMap = true;
		fclose(fp);
		return true;
#else // TARGET_OS_IPHONE
		return false;	// PVR only support IOS?
		
#endif	// TARGET_OS_IPHONE
		
	}

}//namespace Ic3d
