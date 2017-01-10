//
//  IcDemo.h
//  DevEng
//
//  Created by Sherman Chen on 3/3/16.
//  Copyright (c) 2016 Sherman Chen. All rights reserved.
//

#ifndef __DevEng__IcDemo__
#define __DevEng__IcDemo__

#include "DemoScene.h"

using namespace Ic3d;


//-----------------------------
//	DemoModels
//-----------------------------
class DemoModels : public DemoScene
{
public:
	DemoModels(){};
	virtual ~DemoModels(){};
	virtual void onUpdate(double deltaT) override;
    virtual void onInit() override;
	void onToggleModel();
    
protected:
	struct TModelItem
	{
		std::string m_sFile;
        TModelItem(const std::string& sFile);
		ctl::Sp<IcObject> m_pObj = nullptr;
	};
	ctl::SpAry<TModelItem>	m_modelList;
	int m_modelSel = 0;
	
	bool loadItem(TModelItem& rItem);
	ctl::Sp<IcObject> loadObj(const std::string& sFile);
	bool loadCfg();
	ctl::Sp<IcObject> m_pMainObj = ctl::makeSp<IcObject>();
	
    float m_t_modelToggle = 0;  // Every few seconds toggle model
    void updateInfo();
};


#endif /* defined(__DevEng__IcDemo__) */
