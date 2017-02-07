//
//  IcRenderEngText.cpp
//  DevEng
//
//  Created by Sherman Chen on 9/28/16.
//  Copyright (c) 2016 Simviu Technology Inc.
//  All rights reserved.
//  http://www.simviu.com/dev
//


#include "IcRenderEng.h"
#define GLUT_STATIC
#if __APPLE__
#include "TargetConditionals.h"

#if TARGET_OS_IPHONE
#else
    #include "GLUT/glut.h"
#endif //

#endif	// __APPLE__


namespace Ic3d
{
	using namespace std;
	using namespace ctl;
    //--------------------------------------------------------------
    //	drawText
    //--------------------------------------------------------------
    void IcRenderEng::drawText(const string& str,
                               const TColor& c,
                               const TVec2& pos)
    {
		
#if TARGET_OS_IPHONE	//---- Not implemented in iOS,
		// TODO: Text Adapter needed.
#else // TARGET_OS_IPHONE
        // TODO: Re-implement
        glColor3d(c.r, c.g, c.b);
        glColor4f(c.r, c.g, c.b, c.a);

        //---- TODO: none of these works
        TMaterial mtl;
        mtl.m_dfs = c;
        mtl.m_ems = c;
        mtl.m_amb = c;
        mtl.m_spc = c;
     //   applyMaterial(mtl);

        glRasterPos2f(pos.x, pos.y);
        for(auto c : str)
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, c);
#endif // #if TARGET_IPHONE_SIMULATOR
    }
}
