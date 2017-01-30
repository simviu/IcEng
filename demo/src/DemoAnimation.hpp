//
//  DemoAnimation.hpp
//  DevEng
//
//  Created by Sherman Chen on 10/26/16.
//  Copyright (c) 2016 Simviu Technology Inc.
//  All rights reserved.
//  http://www.simviu.com/dev
//

#ifndef DemoAnimation_hpp
#define DemoAnimation_hpp


#include "DemoScene.h"
//-----------------------------
//	CCarObj
//-----------------------------
class CCarObj : public Ic3d::IcObject
{
public:
    void drive(float deltaT);
protected:
    float m_heading = 0;
    float m_turnRate = 0;  // Turn Left, Right or go strait
    float m_t_steer = 0;
};

//-----------------------------
//	DemoAnimation
//-----------------------------
class DemoAnimation : public DemoScene
{
public:
    virtual void onInit() override;
    ctl::Sp<CCarObj> m_pCarObj = nullptr;
    virtual void onUpdate(double deltaT) override;
};

#endif /* DemoAnimation_hpp */
