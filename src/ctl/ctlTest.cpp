//
//  ceTest_con.cpp
//  DevEng
//
//  Created by Sherman Chen on 2/15/16.
//  Copyright (c) 2016 Simviu Technology Inc.
//  All rights reserved.
//  http://www.simviu.com/dev
//


#include "ctl.h"

using namespace std;
using namespace ctl;

#define CTL_UT_CHK(val, expVal, bFlag) \
{ if(val!=expVal) \
{ ctl::logErr(std::string(#val)+" mismatch"); bFlag &= false; } }

//-------------------------------
//	Test Class
//-------------------------------
//---- A Car as base class
class TCar
{
public:
    TCar(){ onInit(); };
    TCar(const std::string& s):m_sName(s){ onInit(); };
    virtual void drive()
    {  logInfo("TCar ["+m_sName+"] drive"); };
    virtual ~TCar()
    { logInfo("TCar ["+m_sName+"] destructor"); };
protected:
    std::string	m_sName;
private:
    void onInit()
    { logInfo("TCar ["+ m_sName + "] constructor"); };
};
//------------------------------------------
//  UnitTest::runUnitTest
//------------------------------------------
bool UnitTest::runUnitTest()
{
    bool isPass = true;
    isPass &= runTestImg();
    isPass &= runTestSpAry();
    isPass &= runTestSpMap();
    return isPass;
}

//------------------------------------------
//  UnitTest::runTestSpAry
//------------------------------------------
bool UnitTest::runTestSpAry()
{
    bool isPass = true;
	//--------------------
	//	SpAry
	//--------------------
	logInfo("-- Test SpAry");
	SpAry<TCar> ary;
	ary.add(ctl::makeSp<TCar>("Honda Civic"));
	ary.add(ctl::makeSp<TCar>("Ford Mustang"));
	ary.add(ctl::makeSp<TCar>("BMW 370"));
	CTL_UT_CHK(ary.size(), 3, isPass);
	size_t cnt=0;
	ary.runAll([&](size_t i, Sp<TCar> p)
			   {	p->drive(); cnt++; });
	CTL_UT_CHK(cnt, 3, isPass);
	 
	logInfo("-- Test SpAry::runForIdx()");
	bool b0 = ary.run(2, [&](size_t i, Sp<TCar> p)
					{
						p->drive();
						CTL_UT_CHK(i, 2, isPass);
					});
	bool b1 = ary.run(3, [&](size_t i, Sp<TCar> p)
					{
						p->drive();
						CTL_UT_CHK("shouldNotRun0",
								   "shouldNotRun1", isPass);
					});
	CTL_UT_CHK(b0, true, isPass);
	CTL_UT_CHK(b1, false, isPass);
	//---- Check []
	Sp<TCar> p;
	p = ary[2]; CTL_UT_CHK(p!=nullptr, true, isPass);
	p->drive();
	p = ary[3]; CTL_UT_CHK(p==nullptr, true, isPass);
	//--------------------
	//	SpAry, use call back
	//--------------------
	//---- Local derived class for quick derivation
	struct CBK1:public SpAry<TCar>::CallBK
	{	virtual void run(size_t idx, Sp<TCar>p) override
		{	p->drive();
			m_cnt++;
		};
		//---- Exchange data use member vars
		size_t m_cnt=0;
	};
	CBK1 cbk1;
	ary.runAll(cbk1);
	CTL_UT_CHK(cbk1.m_cnt, 3, isPass);
	cbk1.m_cnt =0;
	CTL_UT_CHK(ary.run(2, cbk1), true, isPass);
	CTL_UT_CHK(cbk1.m_cnt, 1, isPass);
	//---- Should Not found item
	CTL_UT_CHK(ary.run(3, cbk1), false, isPass);
    return isPass;
}
//------------------------------------------
//  UnitTest::runTestSpMap
//------------------------------------------
bool UnitTest::runTestSpMap()
{
    bool isPass = true;
	//--------------------
	//	SpMap
	//--------------------
	logInfo("-- Test SpMap");
	SpMap<TCar>map1;
	map1.store("Tom", makeSp<TCar>("Benz"));
	map1.store("David", makeSp<TCar>("Chevy"));
	CTL_UT_CHK(map1.size(), 2, isPass);
	int cnt =0;
    Sp<TCar> p = nullptr;
	map1.runAll([&](const std::string& sKey, Sp<TCar> p)
	{
		logInfo(sKey + " is driving:");
		p->drive();
		cnt++;
	});
	CTL_UT_CHK(cnt, 2, isPass);
	//---- Run for sKey=="David", shoud found
	map1.run("David", [&](const string& sKey, Sp<TCar> p)
			 {
				 CTL_UT_CHK(sKey, "David", isPass);
				 p->drive();
			 });
	bool shouldNotFound =
		map1.run("No Body", [&](const string& sKey, Sp<TCar> p)
			 {
				 CTL_UT_CHK(sKey, "David", isPass);
				 p->drive();
			 });
	CTL_UT_CHK(shouldNotFound, false, isPass);
	//---- Check []
	p = map1.lookup("David"); CTL_UT_CHK(p!=nullptr, true, isPass);
	p->drive();
	p = map1.lookup("No Body"); CTL_UT_CHK(p==nullptr, true, isPass);
	//--------------------
	//	SpMap, use call back
	//--------------------
	//---- Local derived class for quick derivation
	struct CBK2:public SpMap<TCar>::CallBK
	{	virtual void run(const string& sKey, Sp<TCar>p) override
		{	p->drive();
			m_cnt++;
		};
		//---- Exchange data use member vars
		size_t m_cnt=0;
	};
	CBK2 cbk2;
	map1.runAll(cbk2);
	CTL_UT_CHK(cbk2.m_cnt, 2, isPass);
	cbk2.m_cnt =0;
	CTL_UT_CHK(map1.run("David", cbk2), true, isPass);
	CTL_UT_CHK(map1.run("No Body", cbk2), false, isPass);
	
	return isPass;
}
