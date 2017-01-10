//
//  DemoAnimation.cpp
//  DevEng
//
//  Created by Sherman Chen on 10/26/16.
//  Copyright © 2016 Sherman Chen. All rights reserved.
//

#include "DemoAnimation.hpp"
#include <cstdlib>
using namespace std;
using namespace ctl;
using namespace Ic3d;


//----------------------------------------------
//  CCarObj::drive
//----------------------------------------------
const static float K_speed = 5; // speed is 1 meter/second
const static float K_T_steer = 2; // Every few seconds change direction
const static float K_squareRange = 20.0;// If hit range, drive back to center
const static float K_turnRateRange = 50;
void CCarObj::drive(float deltaT)
{
    auto& h = m_heading;
    h += m_turnRate * deltaT;
    if(h > 360) h -= 360;
    if(h < 0)   h += 360;
    
    //---- Calculate Angle (Quaternion)
    TQuat q(TVec3(0,deg2rad(h),0));
    setQuat(q);
    
    //---- Calculate vector of speed
    TVec3 dv(0,0,1);
    dv = glm::rotateY(dv, deg2rad(h));
    dv *= K_speed * deltaT;
    
    //--- Calc new position
    TVec3 pos = getPos();
    pos += dv;
    setPos(pos);
    
    //--- Randomly change direction
    m_t_steer += deltaT;
    if(m_t_steer > K_T_steer)
    {
        m_t_steer = 0;
        int sel = (std::rand()%3)-1;
        m_turnRate = K_turnRateRange * ((float)sel);
    }
    
    //---- Check boundry, if hit, back to center
    if(fabs(pos.x) > K_squareRange ||
       fabs(pos.z) > K_squareRange)
        setPos(TVec3(0,0,0));
}

//----------------------------------------------
//  DemoAnimation::onInit
//----------------------------------------------
void DemoAnimation::onInit()
{
    //--- Always call parent class onInit()
    IcScene::onInit();
    
    //---- Create Bottom Plate
    float r = K_squareRange;
    IcMeshData mshd;
    mshd.createPlaneXZ(TRect(-r,-r, r*2, r*2), TRect());
    auto pMsh = makeSp<IcMesh>(mshd);
    auto pModel0 = makeSp<IcModel>(pMsh);
    auto pObj0 = makeSp<IcObject>(pModel0);
    addObj(pObj0);
    
    //--- Load Object
 //   auto pModel1 = IcModel::createCube(TVec3(2,1,1), TVec3(0,0,0), TColor(0.9,0.1,0.1,1));
    string sPath = "";
    auto pModel1 = makeSp<IcModel>(sPath + "IcDemo/Jeep/Jeep.obj");
    auto pObj1   = ctl::makeSp<CCarObj>();
    pObj1->setScale(TVec3(0.5, 0.5, 0.5));
    pObj1->setModel(pModel1);
    m_pCarObj = pObj1;
    // Add this obj to Scene, will be rendered
    addObj(pObj1);
    
    //---- Set Camera
    auto& cam = *getCamera();
    
    // Put Camera somewhere apart.
    cam.setPos(TVec3(0, 20, 60));
    
    // Set camera look at obj, up vector is +y.
    // Our obj is at (0,0,0) position by default.
    cam.lookAt(TVec3(0,0,0), TVec3(0,1,0));
 }

//----------------------------------------------
//  DemoAnimation::onInit
//----------------------------------------------
void DemoAnimation::onUpdate(double deltaT)
{
    //---- Always call parent
    IcScene::onUpdate(deltaT);
    
    //---- Drive my car
    if(m_pCarObj!=nullptr)
        m_pCarObj->drive(deltaT);
    
}
