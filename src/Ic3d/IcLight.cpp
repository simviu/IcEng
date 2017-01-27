//
//  IcLight.cpp
//  DevEng
//
//  Created by Sherman Chen on 10/28/16.
//  Copyright (c) 2016 Simviu Technology Inc.
//  All rights reserved.
//  http://www.simviu.com/dev
//


#include "Ic3d.h"
#include "IcRenderEng.h"

namespace Ic3d
{
    using namespace std;
    using namespace ctl;

    
    const static TVec3 K_vecFront(0,0,-1);    // Front point at -z
    //--------------------------------------
    //	getDir
    //--------------------------------------
    TVec3 IcLight::getDir() const    // Assume -z as front direction
    {
        return getQuat()*K_vecFront;
    };
    //--------------------------------------
    //	pointAt
    //--------------------------------------
    void IcLight::pointAt(const TVec3& pos)
    {
        TVec3 v0 = K_vecFront;
        TVec3 v1 = glm::normalize(pos - getPos());
        TQuat q = quatFromVecs(v0, v1);
        setQuat(q);
    }
   
    //--------------------------------------
    //	setSpotPara
    //--------------------------------------
    void IcLight::setSpotPara(float Kd2, float coneAngle, float cutOff)
    {
        m_para.m_isPara = false; m_para.m_Kd2 = Kd2;
        m_para.m_cosConeAngle = cos(deg2rad(coneAngle));
        m_para.m_cosCutOff = cos(deg2rad(cutOff));
    };
    //--------------------------------------
    //	setLightColor
    //--------------------------------------
    void IcLight::setLightColor(const TColor& c)
    {
        m_para.m_mat.setColor(c,1,1);
    };
}
