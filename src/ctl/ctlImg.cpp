//
//  ctlImg.cpp
//  DevEng
//
//  Created by Sherman Chen on 2/24/16.
//  Copyright (c) 2016 Simviu Technology Inc.
//  All rights reserved.
//  http://www.simviu.com/dev
//
//


#include "ctl.h"
#include "lodepng.h"

namespace ctl {
    using namespace std;

	//------------------------------------------
	//	Extension Adapter
	//------------------------------------------
	struct CExtFormatAdpMng
	{
		Sp<IcImg::CFormatAdp> lookup(const string& sExt)
		{ return m_map.lookup(sExt); };
		void addAdp(const string& sExtList, Sp<IcImg::CFormatAdp> p)
		{
			auto tkns = ctl::s2tokens(sExtList, " ,;\t");
			for(auto sExt : tkns)
				m_map.store(sExt, p);
		}
	private:
		ctl::SpMap<IcImg::CFormatAdp> m_map;	// keyed with file ext
	};
	static CExtFormatAdpMng l_extAdpMng;
	//------------------------------------------
	//	Util Macro
	//------------------------------------------
	#define CE_LIMIT(d, dmin, dmax) ((d<dmin)?dmin:(d>dmax)?dmax:d)
	//---------------------------------------------
	//	IcImg::TPixel to/from str
	//---------------------------------------------
	std::string IcImg::TPixel::toStr() const
	{
		return v2s(r)+","+v2s(g)+","+v2s(b)+","+v2s(a);
	}
	bool IcImg::TPixel::fromStr(const string& s)
	{
		auto tkns = s2tokens(s);
		if(tkns.size()<3) return false;
		a = 255;
		bool isOK = true;
		unsigned int ri=0, gi=0, bi=0, ai=255;
		isOK &=s2v(tkns[0], ri);
		isOK &=s2v(tkns[1], gi);
		isOK &=s2v(tkns[2], bi);
		if(tkns.size()>3)
			isOK &=s2v(tkns[3],ai);
		if(isOK)
			{ r=ri; g=gi; b=bi; a=ai; }
		return isOK;
	}
	//------------------------------------------
	//	loadFile
	//------------------------------------------
	bool IcImg::loadFile(const string& sFile)
	{
		checkSysInit();
		logInfo("Load IcImg ["+sFile+"]...");
		string sExt = s2lower(sFileExt(sFile));
		bool isOK = true;
		
		auto pAdp = l_extAdpMng.lookup(sExt);
		if(pAdp==nullptr) pAdp = l_extAdpMng.lookup("*");
		if(pAdp==nullptr)
		{
			ctl::logFileErr(sFile);
			ctl::logErr("Image format not supported for '"+sExt+"'");
			return false;
		}
		isOK = pAdp->loadFile(*this, sFile);

		
		
		string sR = (isOK?"Done.":"Failed.");
		logInfo("Load IcImg "+ sR);
		return isOK;
	}
	//------------------------------------------
	//	loadFile
	//------------------------------------------
	bool IcImg::saveFile(const string& sFile) const
	{
		checkSysInit();
		logInfo("Save IcImg ["+sFile+"]...");
		string sExt = s2lower(sFileExt(sFile));
		bool isOK = true;
		
		auto pAdp = l_extAdpMng.lookup(sExt);
		if(pAdp==nullptr) pAdp = l_extAdpMng.lookup("*");
		if(pAdp==nullptr)
		{
			ctl::logFileErr(sFile);
			ctl::logErr("Image format not supported for '"+sExt+"'");
			return false;
		}
		isOK = pAdp->saveFile(*this, sFile);
		
		string sR = (isOK?"Done.":"Failed.");
		logInfo("Save IcImg "+ sR);
		return isOK;
	}

    //------------------------------------------
    //	copy
    //------------------------------------------
    void IcImg::copy(const IcImg& img)
    {
        setSize(img.size());
        m_buf.copy(img.getBuf());
    }

	//------------------------------------------
	//	ImgHelper
	//------------------------------------------
	struct ImgHelper
	{
		//---- Bi-Cubic interpolate
		static IcImg::TPixel
			interpolateColorBicubic(const IcImg::TPixel& c0,
									const IcImg::TPixel& c1,
									const IcImg::TPixel& c2,
									const IcImg::TPixel& c3,
									float t)
		{
			IcImg::TPixel c;
			c.r = BiCubic::interpByte(t, c0.r, c1.r, c2.r, c3.r);
			c.g = BiCubic::interpByte(t, c0.g, c1.g, c2.g, c3.g);
			c.b = BiCubic::interpByte(t, c0.b, c1.b, c2.b, c3.b);
			c.a = BiCubic::interpByte(t, c0.a, c1.a, c2.a, c3.a);
			return c;
			
		}

		//---- c = c0*t + (1-t)c1
		static IcImg::TPixel
			interpolateColor(const IcImg::TPixel& c0,
							 const IcImg::TPixel& c1,
							 float t)
		{
			IcImg::TPixel c;
			c.r = interpolateColorByte(c0.r, c1.r, t);
			c.g = interpolateColorByte(c0.g, c1.g, t);
			c.b = interpolateColorByte(c0.b, c1.b, t);
			c.a = interpolateColorByte(c0.a, c1.a, t);
			return c;
		};
		//---- c = c0*t + (1-t)c1
		static TByte interpolateColorByte(TByte c0, TByte c1,
									float t)
		{
			TByte c = (((float)c0)*(1-t)) +
				(((float)c1)*t);
			return c;
		};
		//--------------------------------
		//----- Interpolation Bi-cubic
		//--------------------------------
		// Ref : https://en.wikipedia.org/wiki/Bicubic_interpolation
		class BiCubic
		{
		public:
			//--------------
			static TByte interpByte(float t, TByte d0, TByte d1, TByte d2, TByte d3)
			{
				float d = interpFloat(t, (float)d0/255.0, (float)d1/255.0,
										 (float)d2/255.0, (float)d3/255.0);
				return d*255.0;
			};
			//--------------
			static float interpFloat(float t, float d0, float d1, float d2, float d3)
			{
				/* The biCubic coef
				 M =
					{ 0,  2,  0,  0},
					{-1,  0,  1,  0},
					{ 2, -5,  4, -1},
					{-1,  3, -3,  1}
				 
				 p(t) = (1/2)*[1 t t*t t*t*t] * M * [ d0 d1 d2 d3]'
				 */
				 
				float t2 = t*t;
				float t3 = t2*t;
				float a0 = 0 -1*t +2*t2 -1*t3;
				float a1 = 2 +0   -5*t2 +3*t3;
				float a2 = 0 +1*t +4*t2 -3*t3;
				float a3 = 0 +0   -1*t2 +1*t3;
				return (a0*d0 + a1*d1 + a2*d2 + a3*d3)*0.5;
			};
		};
	};
	
	//------------------------------------------
	//	IcImg
	//------------------------------------------
	bool IcImg::setSize(const TSize& size)
	{
		size_t N = size.w * size.h * sizeof(TPixel) * 4;
		bool isOK = m_buf.setSize(N);
		if(isOK)
			m_size = size;
		return isOK;
	}
	//------------------------------------------
	//	setPx
	//------------------------------------------
	bool IcImg::setPx(const TPos& pos,
					  const TPixel& c)
	{
		if(pos.x<0 || pos.x>=m_size.w) return false;
		if(pos.y<0 || pos.y>=m_size.h) return false;
		size_t i = (pos.y*m_size.w + pos.x)*
						sizeof(TPixel);
		const TByte* p = reinterpret_cast<const TByte*>(&c);
		size_t r = m_buf.fill(p, i, sizeof(c));
		return (r==sizeof(c));
	}
	//------------------------------------------
	//	getPx
	//------------------------------------------
	bool IcImg::getPx(const TPos& pos, TPixel& c) const
	{
		TPos pos1 = pos;
        if(pos.x<0 || pos.x>=m_size.w) return false;
        if(pos.y<0 || pos.y>=m_size.h) return false;
		size_t i = (pos1.y*m_size.w + pos1.x)*
						sizeof(TPixel);
		c.r = m_buf[i];
		c.g = m_buf[i+1];
		c.b = m_buf[i+2];
		c.a = m_buf[i+3];
		
		return true;
	}

	//---------------------------------------------
	//	fillColor
	//---------------------------------------------
	void IcImg::fillColor(const TPixel& c)
	{
		size_t N = m_buf.size()/sizeof(TPixel);
		TPixel* p = reinterpret_cast<TPixel*>(m_buf.getBuf());
		for(size_t i=0;i<N;i++)
			*(p++) = c;
			
	}

	//---------------------------------------------
	//	scaleTo
	//---------------------------------------------
	// Bi-Cubic scale
	void IcImg::scaleTo(const TSize& size)
	{
		if(m_size.w==0 ||m_size.h==0)
			return;
		int w = m_size.w;
		int h = m_size.h;
		int wn = size.w;
		int hn = size.h;
		//-------------------------
		//	scale horizontal
		//-------------------------
		IcImg img1(TSize(wn, h));
		// In case of scale down, scl <1.0
		float scl = ((float)wn)/((float)w);
		if(scl<=0) return;
		for(int x=0;x<wn;x++)
		{
			float xf = ((float)x)/scl;
			int x0 = (int)xf;
			int x1 = x0+1;
			int x2 = x0+2;
			int xm1 = x0 -1;	// x[-1]
			float t = xf-x0;
			for(int y=0;y<h;y++)
			{
				TPixel c0,c1,c2,c3;
				getPx(TPos(xm1,y), c0);
				getPx(TPos(x0, y), c1);
				getPx(TPos(x1, y), c2);
				getPx(TPos(x2, y), c3);
				TPixel c = ImgHelper::interpolateColorBicubic(c0,c1,c2,c3,t);
				img1.setPx(TPos(x, y), c);
			}
		}
		//-------------------------
		//	scale vertical
		//-------------------------
		setSize(size);
		// In case of scale down, scl <1.0
		scl = ((float)hn)/((float)h);
		if(scl<=0) return;
		for(int y=0;y<hn;y++)
		{
			float yf = ((float)y)/scl;
			int y0 = (int)yf;
			int y1 = y0+1;
			int y2 = y0+2;
			int ym1 = y0-1; // y[-1]
			float t = yf-y0;
			for(int x=0;x<wn;x++)
			{
				TPixel c0,c1,c2,c3;
				img1.getPx(TPos(x,ym1), c0);
				img1.getPx(TPos(x,y0),  c1);
				img1.getPx(TPos(x,y1),  c2);
				img1.getPx(TPos(x,y2),  c3);
				TPixel c = ImgHelper::interpolateColorBicubic(c0, c1, c2, c3, t);
				setPx(TPos(x, y), c);
			}
		}
		
	}
	//---------------------------------------------
	//	scaleTo
	//---------------------------------------------
	// Bi-Linear scale
	void IcImg::scaleTo_bilinear(const TSize& size)
	{
		if(m_size.w==0 ||m_size.h==0)
			return;
		int w = m_size.w;
		int h = m_size.h;
		int wn = size.w;
		int hn = size.h;
		//-------------------------
		//	scale horizontal
		//-------------------------
		IcImg img1(TSize(wn, h));
		// In case of scale down, scl <1.0
		float scl = ((float)wn)/((float)w);
		if(scl<=0) return;
		for(int x=0;x<wn;x++)
		{
			float xf = ((float)x)/scl;
			int x0 = (int)xf;
			int x1 = x0+1;
			float t = xf-x0;
			for(int y=0;y<h;y++)
			{
				TPixel c0,c1;
				getPx(TPos(x0,y), c0);
				getPx(TPos(x1,y), c1);
				TPixel c = ImgHelper::interpolateColor(c0, c1, t);
				img1.setPx(TPos(x, y), c);
			}
		}
		//-------------------------
		//	scale vertical
		//-------------------------
		setSize(size);
		// In case of scale down, scl <1.0
		scl = ((float)hn)/((float)h);
		if(scl<=0) return;
		for(int y=0;y<hn;y++)
		{
			float yf = ((float)y)/scl;
			int y0 = (int)yf;
			int y1 = y0+1;
			float t = yf-y0;
			for(int x=0;x<wn;x++)
			{
				TPixel c0,c1;
				img1.getPx(TPos(x,y0), c0);
				img1.getPx(TPos(x,y1), c1);
				TPixel c = ImgHelper::interpolateColor(c0, c1, t);
				setPx(TPos(x, y), c);
			}
		}
	}

	//---------------------------------------------
	//	CFormatAdp_png
	//---------------------------------------------
	class CFormatAdp_png : public IcImg::CFormatAdp
	{
		//---------------------------------
		virtual bool loadFile(IcImg& img, const string& sFile)
		{
			std::vector<unsigned char> buffer, image;
			lodepng::load_file(buffer, sFile.c_str()); //load the image file with given filename
			unsigned w=0, h=0;
			unsigned error = lodepng::decode(image,w, h, buffer); //decode the png
			//stop if there is an error
			if(error)
			{
				string sErr = lodepng_error_text(error);
				logErr("Load IcImg Failed: --"+sErr );
				return false;
			}
			
			TSize sz(w,h);
			//get width, height and pixels
			img.setSize(sz);
			size_t L=w*h*4;
			auto& buf = img.getBuf();
			for(size_t i=0;i<L;i++)
				buf.setAt(i, image[i]);
			logInfo("Load IcImg Done.(size =["+
					sz.toStr()+"]");
			return true;
			
		};
		//---------------------------------
		virtual bool saveFile(const IcImg& img, const string& sFile) const
		{
			std::vector<unsigned char> image;
			auto sz = img.size();
			int w = sz.w;
			int h = sz.h;
			auto& buf = img.getBuf();
			image.resize(w * h * 4);
			int p=0;
			for(int y = 0; y < h; y++)
				for(int x = 0; x < w; x++)
				{
					int k = y*w+x;
					image[k*4+0] = buf[p++];
					image[k*4+1] = buf[p++];
					image[k*4+2] = buf[p++];
					image[k*4+3] = buf[p++];
				}
			
			//----------------------------------------------
			
			//encode and save
			std::vector<unsigned char> buffer;
			unsigned error = lodepng::encode(buffer, image.empty() ? 0 : &image[0], w, h);
			if(!error)
				error = lodepng::save_file(buffer, sFile.c_str());
			else
			{
				string sErr = lodepng_error_text(error);
				logErr("Save IcImg Failed: --"+sErr );
				return false;
			}
			
			return true;
			
		};
		
	};
	//---------------------------------------------
	//	addFormatAdp
	//---------------------------------------------
	void IcImg::addFormatAdp(const string& sExtList,
							 ctl::Sp<CFormatAdp> pAdp)
	{
		l_extAdpMng.addAdp(sExtList, pAdp);
	}
	
	//---------------------------------------------
	//	checkSysInit
	//---------------------------------------------
	void IcImg::checkSysInit()
	{
		static bool m_hasInit = false;
		if(m_hasInit) return;
		m_hasInit = true;
		addFormatAdp("png", ctl::makeSp<CFormatAdp_png>());
	}
	
}	// namespace ctl
