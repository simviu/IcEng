//
//  IcText.cpp
//  IcEngLib
//
//  Created by Sherman Chen on 30/01/2017.
//  Copyright (c) 2017 Simviu Technology Inc.
//  All rights reserved.
//  http://www.simviu.com/dev
//
//

#include "Ic3d.h"
#include "IcRenderEng.h"

namespace Ic3d
{
    using namespace std;
    using namespace ctl;
    //--------------------------------------
    //	IcText
    //--------------------------------------
    void IcText::onDraw() const
    {
        auto pEng = IcRenderEng::getInstance();
        auto pAdp = pEng->getCurRenderAdp();
        pEng->drawText(m_sText, m_color, m_pos);
    }
   
}
