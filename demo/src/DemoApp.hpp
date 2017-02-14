//
//  DemoApp.hpp
//  IcEngDemo
//
//  Created by Sherman Chen on 12/24/16.
//  Copyright (c) 2016 Simviu Technology Inc.
//  All rights reserved.
//  http://www.simviu.com/dev
//

#ifndef DemoApp_hpp
#define DemoApp_hpp

#include "Ic3d.h"

class DemoApp : public Ic3d::IcApp
{
public:
    virtual void onInit() override;

    //-----------------------------------
    //  Demo List
    //-----------------------------------
    struct TItem
    {
        TItem(const std::string& sTitle):m_sTitle(sTitle){};
        std::string  m_sTitle;
    };
    size_t getDemoNum()const{ return m_demoAry.size(); };
    const TItem& getDemoItem(int idx){ return m_demoAry[idx]; };
    void setDemoSel(int sel){ m_demoSel = sel; };
    virtual std::string onCmd(const std::string& sCmd) override;
     
protected:
    ctl::Sp<Ic3d::IcScene> createDemoScn(int sel);
	std::vector<TItem> m_demoAry;
    int m_demoSel = 0;
};


#endif /* DemoApp_hpp */
