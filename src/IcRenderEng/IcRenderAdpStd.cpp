//
//  IcRenderAdpStd.cpp
//  DevEng
//
//  Created by Sherman Chen on 9/28/16.
//  Copyright © 2016 Sherman Chen. All rights reserved.
//

#include "IcRenderAdpStd.h"
#include "OpenGLinc.h"
#include "glm/gtc/type_ptr.hpp"


namespace Ic3d {
    using namespace ctl;
    using namespace std;
    

	//--------------------------------------------------------------
    //	fillShaderData
    //--------------------------------------------------------------
    void IcRenderAdpStd::fillShaderData()
    {
        auto pShdr = m_pShader;
        if(pShdr==nullptr) return;
        
        //---- Attribute Location
        auto& attrLoc = m_shaderData.m_attrLoc;
        attrLoc.m_vert  = pShdr->GetAttribLocation("a_vert");
        attrLoc.m_tex   = pShdr->GetAttribLocation("a_texCo");
        attrLoc.m_norm  = pShdr->GetAttribLocation("a_normal");
        
        //---- Uniform
        auto& unfmMtl = m_shaderData.m_unfmLoc_mtl;
        unfmMtl.m_amb  = pShdr->GetUniformLocation("u_material.amb");
        unfmMtl.m_dfs  = pShdr->GetUniformLocation("u_material.dfs");
        unfmMtl.m_spc  = pShdr->GetUniformLocation("u_material.spc");
        unfmMtl.m_ems  = pShdr->GetUniformLocation("u_material.ems");
        unfmMtl.m_shns = pShdr->GetUniformLocation("u_material.shns");
        unfmMtl.m_alpha= pShdr->GetUniformLocation("u_material.alpha");
 
        //---- Matrix
        auto& unfmMat = m_shaderData.m_unfmLoc_mat;
        unfmMat.m_proj      = pShdr->GetUniformLocation("u_matProj");
        unfmMat.m_modelView = pShdr->GetUniformLocation("u_matModelView");
        unfmMat.m_norm      = pShdr->GetUniformLocation("u_matNormal");
        
        //---- Light
        for(int i=0;i<TShaderData::K_MAX_LIGHTS;i++)
        {
            auto& unfm = m_shaderData.m_unfmLoc_lights[i];
            string sLoc = "u_lightAry[" + v2s(i) +"].";
            unfm.m_dir = pShdr->GetUniformLocation(sLoc + "dir");
            unfm.m_pos = pShdr->GetUniformLocation(sLoc + "pos");
            unfm.m_Kd2 = pShdr->GetUniformLocation(sLoc + "Kd2");
            unfm.m_isPara       = pShdr->GetUniformLocation(sLoc + "isPara");
            unfm.m_cosConeAngle = pShdr->GetUniformLocation(sLoc + "cosConeAngle");
            unfm.m_cosCutOff    = pShdr->GetUniformLocation(sLoc + "cosCutOff");
            
            //---- Light Mat
            unfm.m_mat.m_amb = pShdr->GetUniformLocation(sLoc + "mat.amb");
            unfm.m_mat.m_dfs = pShdr->GetUniformLocation(sLoc + "mat.dfs");
            unfm.m_mat.m_spc = pShdr->GetUniformLocation(sLoc + "mat.spc");
            unfm.m_mat.m_ems = pShdr->GetUniformLocation(sLoc + "mat.ems");
        }
        m_shaderData.m_unfmLoc_lightNum = pShdr->GetUniformLocation("u_lightNum");
      
   }
   //----------------------------------------------------
    //	Factory of Adapter
    //----------------------------------------------------
     ctl::Sp<CRenderAdp::CMeshAdp> IcRenderAdpStd::createMeshAdp(const TMeshData& rMshd) const
    {
        auto p = ctl::makeSp<CMeshAdpStd>(rMshd, m_shaderData);
        return p;
    }
    ctl::Sp<CRenderAdp::CTexAdp> IcRenderAdpStd::createTextureAdp(const ctl::IcImg& rImg) const
    {
        return ctl::makeSp<CTexAdpStd>(rImg);
    }
    ctl::Sp<CRenderAdp::CTexAdp> IcRenderAdpStd::createTextureAdp(const std::string& sFile) const
    {
        return ctl::makeSp<CTexAdpStd>(sFile);
    }
    //--------------------------------------------------------------
    //	sendUniform
    //--------------------------------------------------------------
    void IcRenderAdpStd::sendUniform(TShaderData::T_glLocIdx loc, const TVec3& v) const
    {
        glUniform3f(loc, v.x, v.y, v.z);
    }
    void IcRenderAdpStd::sendUniform(TShaderData::T_glLocIdx loc, const TVec4& v) const
    {
        glUniform4f(loc, v.x, v.y, v.z, v.w);
    }
    
    //--------------------------------------------------------------
    //	loadShader
    //--------------------------------------------------------------
    bool IcRenderAdpStd::loadShader(const string& sFileVert,
                                    const string& sFileFrag)
    {
        m_pShader = ctl::makeSp<IcShader>(sFileVert, sFileFrag);
        if(!m_pShader->isValid())
            return false;
        fillShaderData();
        return true;
    }
    //--------------------------------------------------------------
    //	setLight
    //--------------------------------------------------------------
    void IcRenderAdpStd::setLight(const TLight& light,
                                  const TVec3& ecPos,   // Light pos in eye space
                                  const TVec3& ecDir,   // Light dir in eye space
                                  int lightIdx,
                                  int totalLightNum) const
    {
        if(lightIdx >= TShaderData::K_MAX_LIGHTS) return;
        const auto& unfm = m_shaderData.m_unfmLoc_lights[lightIdx];

        sendUniform(unfm.m_dir,             ecDir);
        sendUniform(unfm.m_pos,             ecPos);
        glUniform1f(unfm.m_Kd2,             light.m_Kd2);
        glUniform1f(unfm.m_isPara,          (light.m_isPara)?1.0 : 0.0);
        glUniform1f(unfm.m_cosCutOff,       light.m_cosCutOff);
        glUniform1f(unfm.m_cosConeAngle,    light.m_cosConeAngle);
        
        sendUniform(unfm.m_mat.m_amb,   light.m_mat.m_amb);
        sendUniform(unfm.m_mat.m_dfs,   light.m_mat.m_dfs);
        sendUniform(unfm.m_mat.m_spc,   light.m_mat.m_spc);
        sendUniform(unfm.m_mat.m_ems,   light.m_mat.m_ems);
        
        glUniform1i(m_shaderData.m_unfmLoc_lightNum, totalLightNum);
    }
    //--------------------------------------------------------------
    //	applyMaterial
    //--------------------------------------------------------------
    void IcRenderAdpStd::applyMaterial(const TMaterial &mat) const
    {
        if(m_pShader==nullptr)
            return;
        
        //----- Apply Material
        const TColor& amb = mat.m_amb;
        const TColor& dfs = mat.m_dfs;
        const TColor& spc = mat.m_spc;
        const TColor& ems = mat.m_ems;
        
        GLfloat ambf[4] = {(GLfloat)(amb.r), (GLfloat)(amb.g), (GLfloat)(amb.b), (GLfloat)(amb.a)};
        GLfloat dfsf[4] = {(GLfloat)(dfs.r), (GLfloat)(dfs.g), (GLfloat)(dfs.b), (GLfloat)(dfs.a)};
        GLfloat spef[4] = {(GLfloat)(spc.r), (GLfloat)(spc.g), (GLfloat)(spc.b), (GLfloat)(spc.a)};
        GLfloat emif[4] = {(GLfloat)(ems.r), (GLfloat)(ems.g), (GLfloat)(ems.b), (GLfloat)(ems.a)};
        GLfloat shif=mat.m_shns;
        
        const auto& loc = m_shaderData.m_unfmLoc_mtl;

        glUniform4fv(loc.m_amb, 1, ambf);
        glUniform4fv(loc.m_dfs, 1, dfsf);
        glUniform4fv(loc.m_spc, 1, spef);
        glUniform4fv(loc.m_ems, 1, emif);
        glUniform1f(loc.m_shns, shif+1);
        glUniform1f(loc.m_alpha, mat.m_topAlpha);
       
    }
    
    //--------------------------------------------------------------
    //	applyMatrix
    //--------------------------------------------------------------
    void IcRenderAdpStd::applyMatrix(TRenderMatrix& rm)
    {
        TMat4 modelViewMat = rm.m_matView * rm.m_matModel;
        
        //---- Also apply normal matrix
        // ( TODO: Move this to high level )
        TMat4 matI = glm::inverse(modelViewMat);
        TMat3 matN(glm::transpose(matI));
        
        //---- Send to uniform
        const auto& loc = m_shaderData.m_unfmLoc_mat;
        //---- TODO: wrap GL uniform transfer
        glUniformMatrix4fv(loc.m_proj,      1, GL_FALSE, glm::value_ptr(rm.m_matProj));
        glUniformMatrix4fv(loc.m_modelView, 1, GL_FALSE, glm::value_ptr(modelViewMat));
        glUniformMatrix3fv(loc.m_norm,      1, GL_FALSE, glm::value_ptr(matN));

    }
 }
