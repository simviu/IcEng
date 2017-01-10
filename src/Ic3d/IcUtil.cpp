//
//  IcUtil.cpp
//  DevEng
//
//  Created by Sherman Chen on 9/30/16.
//  Copyright © 2016 Sherman Chen. All rights reserved.
//


#include "Ic3d.h"
#include "IcRenderEng.h"

namespace Ic3d
{
    using namespace std;
    using namespace ctl;

    //--------------------------------------
    //	IcUtil
    //--------------------------------------
    TQuat quatFromVecs(const TVec3& v0, const TVec3& v1)
    {
        TVec3 vAxis = glm::cross(v0, v1);
        float a = acos(glm::dot(v0, v1));
        TQuat q(glm::rotate(a, vAxis));
        return q;
        
    }

}

