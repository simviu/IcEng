//
//  IcMeshDataSubMsh.cpp
//  DevEng
//
//  Created by Sherman Chen on 9/30/16.
//  Copyright (c) 2016 Simviu Technology Inc.
//  All rights reserved.
//  http://www.simviu.com/dev
//

#include "Ic3d.h"

namespace Ic3d{
    using namespace std;
    using namespace ctl;
    
    //-----------------------------------------------------
    //	CSubMeshHelper
    //-----------------------------------------------------
    typedef  TMeshData::TFaceIdx TTFaceIdx;
    
    struct CSubMeshHelper
    {
        void addTranslateFace(const TTFaceIdx& faceOri,
                              TTFaceIdx& faceNew)
        {
            for(int i=0;i<3;i++)
            {
                const auto& vOri = faceOri.m_verts[i];
                auto& vNew = faceNew.m_verts[i];
                vNew.m_vi = m_vTab.checkAddLnk(vOri.m_vi);
                vNew.m_ti = m_tTab.checkAddLnk(vOri.m_ti);
                vNew.m_ni = m_nTab.checkAddLnk(vOri.m_ni);
            }
        };
        //---- When generate new mesh,
        // index mapping changed.
        // Use this TIdxLnk to map ori idx to new idx.
        struct TLnk
        {	size_t m_idxOri=0;
            size_t m_idxNew=0; };
        //----------------
        struct TIdxLnkTable
        {
            size_t checkAddLnk(const size_t idxOri)
            {
                auto iter = m_map.find(idxOri);
                
                //---- Check existance
                if(iter!=m_map.end())
                    return iter->second.m_idxNew;
                size_t idxNew = m_ary.size();
                
                //---- Create/Add new lnk
                TLnk lnk;
                lnk.m_idxOri = idxOri;
                lnk.m_idxNew = idxNew;
                m_ary.add(lnk);
                m_map[idxOri] = lnk;
                return idxNew;
            };
            SafeAry<TLnk>	m_ary;	//[idxNew] = idxOri
        protected:
            map<size_t, TLnk>	m_map;	//<idxOri, idxNew>
        };
        //------------------------
        
        TIdxLnkTable m_vTab;
        TIdxLnkTable m_nTab;
        TIdxLnkTable m_tTab;
    protected:
        
        
    };
    //-----------------------------------------------------
    //	TMeshData
    //-----------------------------------------------------
    void IcMeshData::getSubMesh(TMeshData& mesh,
                               size_t faceStrt,
                               size_t N) const
    {
        CSubMeshHelper m_helper;
        //--------------------
        // Loop face
        //--------------------
        for(size_t i=0; i<N; i++)
        {
            TFaceIdx faceOri;
            if(!m_faces.getAt(i+faceStrt, faceOri))
                continue;
            TFaceIdx faceNew;
            m_helper.addTranslateFace(faceOri, faceNew);
            mesh.addTrian(faceNew);
        }
        //--------------------
        // Add Vert/Texture/Normal
        //--------------------
        const auto& rAry_v = m_helper.m_vTab.m_ary.getAry();
        const auto& rAry_t = m_helper.m_tTab.m_ary.getAry();
        const auto& rAry_n = m_helper.m_nTab.m_ary.getAry();
        
        //----------------
        for(const auto& lnk : rAry_v)
        {
            TVec3 v; m_verts.getAt(lnk.m_idxOri, v);
            mesh.addVert(v);
        };
        //----------------
        for(const auto& lnk : rAry_t)
        {
            TVec2 t; m_texCords.getAt(lnk.m_idxOri, t);
            mesh.addTexCo(t);
        };
        //----------------
        for(const auto& lnk : rAry_n)
        {
            TVec3 n; m_normals.getAt(lnk.m_idxOri, n);
            mesh.addNorm(n);
        };
        
    }
    //-----------------------------------------------------
    //	dbgPrint
    //-----------------------------------------------------
    void IcMeshData::dbgPrint() const
    {
        //----------------
        logDbg("---- Verts");
        for(const auto& v : m_verts.getAry())
            logDbg(toStr(v));
        logDbg("---- TexCords");
        for(const auto& v : m_texCords.getAry())
            logDbg(toStr(v));
        logDbg("---- Normals");
        for(const auto& v : m_normals.getAry())
            logDbg(toStr(v));
        
    }
    
    
    
}
