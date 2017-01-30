//
//  IcDemo.cpp
//  DevEng
//
//  Created by Sherman Chen on 3/14/16.
//  Copyright (c) 2016 Simviu Technology Inc.
//  All rights reserved.
//  http://www.simviu.com/dev
//


#include "DemoModels.h"

using namespace std;
using namespace ctl;
using namespace Ic3d;

//---------------------------------------------
//	def
//---------------------------------------------
static const string K_sPath_demo    = "IcDemo/";
static const string K_sFileDemoCfg  = "model_list.txt";

static const TVec3 K_dfltCamLookAtPos(0, 3, 0);
static const TVec3 K_dfltObjPos(0, 0, 0);
static const TVec3 K_dfltObjEuler(deg2rad(20), deg2rad(15), 0);
static const float K_dfltRotateSpeed = 50;

static const float K_T_modelToggle = 5.0;   // toggle model every 5 seconds
//-----------------------------------------
//	onInit
//-----------------------------------------
DemoModels::TModelItem::TModelItem(const string& sFile)
:m_sFile(sFile)
{
};
//-----------------------------------------
//	onInit
//-----------------------------------------
void DemoModels::onInit()
{
	IcScene::onInit();
	loadCfg();
	
	if(m_modelList.size()==0) return;
	auto pItem = m_modelList.getAry()[0];
	loadItem(*pItem);

    //---- Set Camera
    auto& cam = *getCamera();
    cam.setPos(TVec3(5, 5, 10));
    cam.lookAt(K_dfltCamLookAtPos, TVec3(0,1,0));

}
//---------------------------------
//	loadObj()
//----------------------------------
Sp<IcObject> DemoModels::loadObj(const string& sFile)
{
	logInfo("LoadObj : "+ sFile);
//	string sPath = ctl::SysMng::getPathRes();
	string sPath = DemoScene::m_cfg.m_sPathRes;
	sPath += K_sPath_demo;
	auto pModel = makeSp<IcModel>(sPath + sFile);
	auto pObj = makeSp<IcObject>(pModel);
	return pObj;
	
}

//---------------------------------
//	loadObj()
//----------------------------------
bool DemoModels::loadItem(TModelItem& rItem)
{
	auto pObj = rItem.m_pObj;
	if(pObj==nullptr)
		pObj = loadObj(rItem.m_sFile);

	pObj->setQuat(TQuat(K_dfltObjEuler));
	pObj->setScale(TVec3(1,1,1));
	pObj->setPos(K_dfltObjPos);

	clearObjs();
    addObj(pObj);
    
	m_pMainObj = pObj;
	rItem.m_pObj = pObj;
	return true;
}

//-----------------------------------------
//	onUpdate
//-----------------------------------------
void DemoModels::onUpdate(double deltaT)
{
	//--- Must call super
	IcScene::onUpdate(deltaT);
	
	if(m_pMainObj==nullptr) return;
	static float _dgr = 0;
	_dgr += K_dfltRotateSpeed*deltaT;
	if(_dgr>360) _dgr -= 360;
	auto e = K_dfltObjEuler;
	e.y = deg2rad(_dgr);
	m_pMainObj->setQuat(TQuat(e));
    
    //---- Check toggle model
    m_t_modelToggle += deltaT;
    if(m_t_modelToggle > K_T_modelToggle)
    {
        m_t_modelToggle = 0;
        onToggleModel();
    }
}

//-----------------------------------------
//	onToggleModel
//-----------------------------------------
void DemoModels::onToggleModel()
{
	size_t N = m_modelList.size();
	if(N==0) return;
	m_modelSel ++;
	if(m_modelSel>=N)
		m_modelSel = 0;
	auto pItem = m_modelList.getAry()[m_modelSel];
	if(pItem==nullptr) return;
	loadItem(*pItem);
}

//-----------------------------------------
//	onInit
//-----------------------------------------
bool DemoModels::loadCfg()
{
	string sPath = DemoScene::m_cfg.m_sPathRes;
	sPath += K_sPath_demo;
    bool isOK = true;
    string sListFile = ctl::sFromFile(sPath + K_sFileDemoCfg, isOK);
    if(!isOK) return false;
    auto tknList = s2tokens(sListFile, "\n");
   	for(auto sFile : tknList)
	{
		auto pItem = ctl::makeSp<TModelItem>(sFile);
		m_modelList.add(pItem);
	}
    return true;
}

