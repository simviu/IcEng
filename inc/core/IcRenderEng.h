//
//  IcRenderEng.h
//  DevEng
//
//  Created by Sherman Chen on 6/4/13.
//  Copyright (c) 2016 Simviu Technology Inc.
//  All rights reserved.
//  http://www.simviu.com/dev
//
//

#ifndef __Warbirds__RanderEng__
#define __Warbirds__RanderEng__

#include "IcRenderAdp.h"

namespace Ic3d
{

    class IcShader;
	//----------------------------------------------------------
	//	IcRenderEng
	//----------------------------------------------------------
    // TODO: Rename render core
	class IcRenderEng
	{
	private:
		static IcRenderEng* m_pInstance;
        ctl::Sp<CRenderAdp> m_pCurRenderAdp = nullptr;
        
    public:
        IcRenderEng(){};
		virtual ~IcRenderEng(){};
        static IcRenderEng* getInstance();
        
		bool initEng(const std::string& sPath_shader);
        void releaseEng();
        
		void clearDepthBuf();
		void clearScreen(const TColor& color);
        void onFrameStart();
        void onFrameEnd();
		
		int getGLErrCode() const;
		void readFramePixel(int W, int H, void* ptr)const;
		
 		void setViewPort(const ctl::TRect& rect);
		void drawText(const std::string& str,
					  const TColor& c,
					  const TVec2& pos);

        auto getCurRenderAdp() const -> decltype(m_pCurRenderAdp)
            { return m_pCurRenderAdp; };
	protected:
	};

} // namespace Ic3d

#endif /* defined(__Warbirds__RanderEng__) */
