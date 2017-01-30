//
//  IcRenderAdp.h
//  DevEng
//
//  Created by Sherman Chen on 9/28/16.
//  Copyright (c) 2016 Simviu Technology Inc.
//  All rights reserved.
//  http://www.simviu.com/dev
//
//

#ifndef IcRenderAdp_h
#define IcRenderAdp_h

#include "IcRenderAdp.h"


namespace Ic3d
{
    //---------------------------
    //  TShaderData
    //---------------------------
    // These are attribute/uniform locations of IcShaderStd
    struct TShaderData
    {
        typedef int T_glLocIdx;
        const static int K_invld = -1;
        const static int K_MAX_LIGHTS = 16;
        //---- Attribute Location
        struct
        {
            T_glLocIdx
            m_vert  =K_invld,
            m_norm  =K_invld,
            m_tex   =K_invld;
        }m_attrLoc;
        
        //---------------------------
        //---- Uniform location
        //---------------------------
        //---- For material
        struct
        {
            T_glLocIdx
            m_amb   = K_invld,
            m_dfs   = K_invld,
            m_spc   = K_invld,
            m_ems   = K_invld,
            m_shns  = K_invld,
            m_alpha = K_invld;
        }m_unfmLoc_mtl;
        
        //---- For Matrix
        struct
        {
            T_glLocIdx
            m_proj      = K_invld,
            m_modelView = K_invld,
            m_norm      = K_invld;
        }m_unfmLoc_mat;
        
        //---- For Fog
        struct
        {
            T_glLocIdx
            m_start= K_invld,
            m_k_linear= K_invld,
            m_k_exp= K_invld,
            m_color= K_invld;
        }m_unfmLoc_fog;
        
        //---- For Light
        struct
        {
            T_glLocIdx
            m_pos = K_invld,
            m_dir = K_invld,
            m_Kd2 = K_invld,
            m_isPara = K_invld,
            m_cosConeAngle = K_invld,
            m_cosCutOff = K_invld;
            
            //---- Light material
            struct
            {
                T_glLocIdx
                m_amb   = K_invld,
                m_dfs   = K_invld,
                m_spc   = K_invld,
                m_ems   = K_invld;
            }m_mat;
            
        }m_unfmLoc_lights[K_MAX_LIGHTS];
        T_glLocIdx m_unfmLoc_lightNum = K_invld;
    };
    
    
    //----------------------------
    //	CMeshAdpStd
    //----------------------------
    class CMeshAdpStd : public CRenderAdp::CMeshAdp
    {
    public:
        CMeshAdpStd(const TMeshData& rMeshData, const TShaderData& rShdrDt);
        virtual ~CMeshAdpStd();
        std::string m_sName;
        void render() const override;
        std::string	m_errMsg;
        bool isValid()const { return m_isValid; };
        
    protected:
        const TShaderData& m_rShaderData;
        //----------------------------
        struct TVertTN
        {
            float vx,vy,vz;		// Vertix
            float tx,ty;		// Texture coordinator
            float nx,ny,nz;		// normal vector
        };
        //----------------------------
        struct TTrianIdx{ unsigned short idx[3] = {0,0,0}; };//	// Triangle index for VBO
        //----------------------------
        size_t	m_vboPntNum =0;
        size_t	m_vboTrianIdxNum =0;
        ctl::BinBuf m_bufPnt;   // TODO: Local temp
        ctl::BinBuf	m_bufIdx;   // TODO: Local temp
        unsigned int	m_vboBufGLidx = 0;
        unsigned int	m_vboTrianBufGLidx = 0;
        bool	m_isValid = false;
        
        // TODO: Local temp
        std::vector <TVertTN>	m_aryPoint;
        std::vector <TTrianIdx>	m_aryTrianIdx;
        void	genVboBufData();
        void	sendBufToGL();
    };
    //-----------------------------------------
    //	CTexAdpStd
    //-----------------------------------------
    class CTexAdpStd : public CRenderAdp::CTexAdp
    {
    public:
        CTexAdpStd(const std::string& fname){ loadFile(fname); };
        CTexAdpStd(){};
        CTexAdpStd(const ctl::IcImg& img);
        virtual ~CTexAdpStd();
        std::string	m_sName;
        ctl::TSize	m_size;
        virtual void render() const override;
        void setTexRepeat(bool b){ m_isRepeat = b; };
        
    protected:
        
        void importFromBuf(const ctl::TSize& size,
                           const ctl::BinBuf& buf);
        //----- Store MipMap Data
        struct TMipMapData
        {
            ctl::BinBuf	m_buf;
            TMipMapData(const ctl::TSize& size, const ctl::BinBuf& buf)
            {	m_size = size; m_buf.copy(buf); };
            
            TMipMapData(const ctl::TSize& size) //:iBufData(sz){
            {	m_size = size;
                m_buf.setSize(size.w*size.h*4); };
            ctl::TSize	m_size;
            int		m_level = 0;
        };
        
        unsigned int genTexId();
        bool	putIntoGL(const TMipMapData& rData);
        bool	putIntoGL_Compressed(const TMipMapData& rData);
        
        unsigned int m_texId = 0;
        int		m_PVR_format = 0;
        bool	m_isTexPVR =false;
        bool	m_isTexMipMap =false;
        bool	m_isRepeat =true;
        
        ctl::SpAry<TMipMapData>	m_aryMipData;
        bool putTexIntoGL();
        bool loadFile(const std::string& fname);
        bool loadFilePVR(const std::string& fname);
        bool loadFileOther(const std::string& fname);
    };
    //-----------------------------------------
    //	IcRenderAdpStd
    //-----------------------------------------
    class IcRenderAdpStd : public CRenderAdp
    {
    protected:
        typedef ctl::Sp<IcShader> TShaderSp;
        TShaderSp m_pShader = nullptr;
        void sendUniform(TShaderData::T_glLocIdx loc, const TVec3& v) const;
        void sendUniform(TShaderData::T_glLocIdx loc, const TVec4& v) const;
        void sendUniform(TShaderData::T_glLocIdx loc, const float& d) const;
        void sendUniform(TShaderData::T_glLocIdx loc, const int& d) const;
        
    public:
		IcRenderAdpStd() {};
        
        virtual ctl::Sp<CMeshAdp> createMeshAdp(const TMeshData& rMshd) const override;
        virtual ctl::Sp<CTexAdp> createTextureAdp(const ctl::IcImg& rImg) const override;
        virtual ctl::Sp<CTexAdp> createTextureAdp(const std::string& sFile) const override;
        virtual void applyMaterial(const TMaterial& mat)  const override;
        TShaderSp getCurShader() const{ return m_pShader; };
        void setCurShader(TShaderSp p){ m_pShader = p; };
        bool loadShader(const std::string& sFileVert,
                        const std::string& sFileFrag);
        virtual void setLight(const TLight& light,
                              const TVec3& ecPos,   // Light pos in eye space
                              const TVec3& ecDir,   // Light dir in eye space
                              int lightIdx,
                              int totalLightNum) const override;
        virtual void setFog(const TFogPara& para) const override;
        virtual void applyMatrix(TRenderMatrix& rm) override;
    private:
        void fillShaderData();
        //---- Uniform and Attribute Location
        const TShaderData& getShaderData() const{ return m_shaderData; };
        TShaderData   m_shaderData;
    };
};// namespace Ic3d
#endif /* IcRenderAdp_h */
