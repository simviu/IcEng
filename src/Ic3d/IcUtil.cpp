//
//  IcUtil.cpp
//  DevEng
//
//  Created by Sherman Chen on 9/30/16.
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
    //-------------------------------------------
    //	utils
    //-------------------------------------------
    extern bool s2color(const string& s, TColor& c)
    {
        // TODO: decode HTML color
        bool isOK = true;
        auto tkns = s2tokens(s, " ,\t");
        if(tkns.size()<3)return false;
        isOK &= ctl::s2v(tkns[0],c.r);
        isOK &= ctl::s2v(tkns[1],c.g);
        isOK &= ctl::s2v(tkns[2],c.b);
        c.a = 1.0;
        if(tkns.size()>3)
            isOK &= ctl::s2v(tkns[3],c.a);
        return isOK;
    }
    //-------------------------------
    extern bool s2vec(const string& s, TVec2& d)
    {
        auto tkns = s2tokens(s, " ,\t");
        if(tkns.size()<2) return false;
        return
        ctl::s2v(tkns[0],d.x) &&
        ctl::s2v(tkns[1],d.y);
    }
    //-------------------------------
    extern bool s2vec(const string& s, TVec3& d)
    {
        auto tkns = s2tokens(s, " ,\t");
        if(tkns.size()<3) return false;
        return
        ctl::s2v(tkns[0],d.x) &&
        ctl::s2v(tkns[1],d.y) &&
        ctl::s2v(tkns[2],d.z);
    }
    //-------------------------------
    extern bool s2vec(const string& s, TVec4& d)
    {
        auto tkns = s2tokens(s, " ,\t");
        if(tkns.size()<4) return false;
        return
        ctl::s2v(tkns[0],d.x) &&
        ctl::s2v(tkns[1],d.y) &&
        ctl::s2v(tkns[2],d.z) &&
        ctl::s2v(tkns[3],d.w);
    }
    //-------------------------------
    string toStr(const TVec2& v)
    {
        return
        ctl::v2s(v.x) + "," +
        ctl::v2s(v.y);
    }
    //-------------------------------
    string toStr(const TVec3& v)
    {
        return ctl::v2s(v.x) + "," +
        ctl::v2s(v.y) + "," +
        ctl::v2s(v.z);
    }
    //-------------------------------
    string toStr(const TVec4& v)
    {
        return ctl::v2s(v.x) + "," +
        ctl::v2s(v.y) + "," +
        ctl::v2s(v.z) + "," +
        ctl::v2s(v.w);
    }
    //-------------------------------
    string toStr(const TQuat& q)
    {
        return ctl::v2s(q.x) + "," +
        ctl::v2s(q.y) + "," +
        ctl::v2s(q.z) + "," +
        ctl::v2s(q.w);
    }

}

