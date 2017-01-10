//
//  DemoApp.hpp
//  IcEngDemo
//
//  Created by Sherman Chen on 12/24/16.
//  Copyright © 2016 Simviu Technology Inc. All rights reserved.
//

#ifndef DemoApp_hpp
#define DemoApp_hpp

#include "Ic3d.h"

class DemoWindow : public Ic3d::IcWindow
{
public:
    DemoWindow(const std::string& sPathRes);
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
protected:
	void createDemoScn(int sel);
	std::vector<TItem> m_demoAry;
    std::string m_sPathRes;
    int m_demoSel = 0;
};


#endif /* DemoApp_hpp */
