//
//  CeCG.cpp
//  EngDemo
//
//  Created by Sherman Chen on 2/24/16.
//  Copyright (c) 2016 Simviu Technology Inc.
//  All rights reserved.
//  http://www.simviu.com/dev
//
//


#include "ctl.h"
inline namespace ctl {
    using namespace std;

    //------------------------------------------
	//	def
	//------------------------------------------
	static const string K_sDelimeterIn = ", ;\n\r\t";
	static const string K_sDelimeterOut = ",";

	//------------------------------------------
	//	s2v
	//------------------------------------------
	extern bool s2v2d(const string& s, TSize& sz)
	{
		auto tkns = s2tokens(s, K_sDelimeterIn);
		if(tkns.size()<2) return false;
		return
		ctl::s2v(tkns[0], sz.w) &&
		ctl::s2v(tkns[1], sz.h);
	}
	extern bool s2v2d(const string& s, TPos& pos)
	{
		auto tkns = s2tokens(s, K_sDelimeterIn);
		if(tkns.size()<2) return false;
		return
		ctl::s2v(tkns[0], pos.x) &&
		ctl::s2v(tkns[1], pos.y);
	}
	extern bool s2v2d(const string& s, TRect& r)
	{
		auto tkns = s2tokens(s, K_sDelimeterIn);
		if(tkns.size()<4) return false;
		return
		ctl::s2v(tkns[0], r.pos0.x) &&
		ctl::s2v(tkns[1], r.pos0.x) &&
		ctl::s2v(tkns[2], r.pos1.x) &&
		ctl::s2v(tkns[3], r.pos1.y);
		
	}
	//------------------------------------------
	//	v2s
	//------------------------------------------
	string v2s2d(const TSize& sz){ return
		ctl::v2s(sz.w)+K_sDelimeterOut + ctl::v2s(sz.h); }
	string v2s2d(const TPos& pos){ return
		ctl::v2s(pos.x)+K_sDelimeterOut + ctl::v2s(pos.y); }
	string v2s2d(const TRect& r){ return
		v2s2d(r.pos0)+K_sDelimeterOut + v2s2d(r.pos1); }
	
	


}
