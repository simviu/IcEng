//
//  Ic3dBase.h
//  DevEng
//
//  Created by Sherman Chen on 9/20/16.
//  Copyright © 2016 Sherman Chen. All rights reserved.
//

#ifndef Ic3dBase_h
#define Ic3dBase_h


#include "ctl.h"

//---- GLM : Matrix Math Lib
#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/euler_angles.hpp"

namespace Ic3d
{
	//-----------------------------------------------
	//	From vector vmath
	//-----------------------------------------------
	typedef glm::quat	TQuat;
	typedef glm::vec4	TVec4;
	typedef glm::vec3	TVec3;
	typedef glm::vec2	TVec2;
	typedef glm::mat3	TMat3;
	typedef glm::mat4	TMat4;
	typedef glm::vec4	TColor;
    //-----------------------------------------------
    //	TEuler
    //-----------------------------------------------
	struct TEuler
    {
        TEuler(){};
        TEuler(float pi, float ri, float yi):
        p(pi), r(ri), y(yi){};
        float p = 0;    // pitch
        float r = 0;    // roll
        float y = 0;    // yaw
    };
	
    //-----------------------------------------------
    //	TMaterial
    //-----------------------------------------------
	struct TMaterial
	{
        TMaterial(){};
        TMaterial(const TColor& c, float spcWeight, float ambWeight)
            { setColor(c, spcWeight, ambWeight); };
        virtual ~TMaterial(){};
		TColor	m_amb = TColor(0.9, 0.9, 0.9, 1);
		TColor	m_dfs = TColor(0.9, 0.9, 0.9, 1);
		TColor	m_spc = TColor(0.5, 0.5, 0.5, 1);
		TColor	m_ems = TColor(0,0,0,1);
		float	m_shns = 10;
		float	m_topAlpha = 1.0;
        void setColor(const TColor& c, float spcWeight, float ambWeight )
        {   m_amb=m_dfs=m_spc=m_amb=c;
            m_amb *= ambWeight; m_spc *= spcWeight; };
	};

    //----------------------------
    //	TLight
    //----------------------------
    struct TLight
    {
        TMaterial m_mat = TMaterial(TColor(1,1,1,1),1,0.1);
        bool    m_isPara = true;
        
        //---- Spot Light Para
        float   m_cosConeAngle = -2;    // Cone Angle in Cos()
        float   m_cosCutOff = -2;       // Complete cut off angle in Cos()
        float   m_Kd2 = 0;                  // Attenuation : atn = 1/(1 + Kd2 * dist^2);
        // Note : The pos/dir here is in eye space ( Model/View Matrix )
    };
    //----------------------------
	//	TMeshData
	//----------------------------
	struct TMeshData
	{
		std::string m_sName;
		struct TFaceIdx
		{
			struct TVertIdx
			{
                TVertIdx(size_t vi, size_t ti, size_t ni):
                    m_vi(vi), m_ti(ti), m_ni(ni){};
                TVertIdx(){};
				size_t	m_vi = 0;
				size_t	m_ti = 0;
				size_t	m_ni = 0;
			};
			TVertIdx m_verts[4];	// Triangle or Quad
            //--- Note for quad
            // 0---1
            // | / |
            // 2---3
            void triangulate(TFaceIdx& trn0, TFaceIdx& trn1) const
            {
                std::vector<int> idx[2] = {{ 0,2,1}, { 1,2,3}};
                int j=0;          // j=0,1,2 for triangle
                j = 0; for(const auto k : idx[0]) trn0.m_verts[j++]= m_verts[k];
                j = 0; for(const auto k : idx[1]) trn1.m_verts[j++]= m_verts[k];
            };
            
		};
		void addVert(const TVec3& v){ m_verts.add(v); };
		void addNorm(const TVec3& n){ m_normals.add(n); };
		void addTexCo(const TVec2& t){ m_texCords.add(t); };
		void addTrian(const TFaceIdx& f){ m_faces.add(f); };
        void addQuad(const TFaceIdx& f){
            TFaceIdx trn[2];  f.triangulate(trn[0], trn[1]);
            addTrian(trn[0]); addTrian(trn[1]);
        }
		ctl::SafeAry<TVec3>		m_verts;
		ctl::SafeAry<TVec2>		m_texCords;
		ctl::SafeAry<TVec3>		m_normals;
		ctl::SafeAry<TFaceIdx>	m_faces;
        
 	};
	//-----------------------------------------
	//	IcShader
	//-----------------------------------------
	class IcShader
	{

	protected:
		bool m_isValid = false;

	public:
		typedef int T_glLoc;    // GL location of attribute/uniform
		IcShader() { };
		IcShader(const std::string& sFileVert,
			const std::string& sFileFrag);

		virtual ~IcShader();
		bool loadFile(const std::string& sFileVert,
			const std::string& sFileFrag);


		virtual bool isValid()const { return m_isValid; };
		int GetAttribLocation(const std::string sName) const;
		int GetUniformLocation(const std::string sName) const;
		void bindShader() const;
		void unbindShader() const;

	private:
		void loadShaderFile(const char* sVertexSource, const char* sFragmentSource);
		void buildShader();
		void compileShader();
		int m_uVertexShader = -1;
		int m_uFragmentShader = -1;
		int m_uProgram = 1;
		std::string mVertexShaderStr;
		std::string mFragmentShaderStr;
	};


	//----------------------------
	//	Render Adapter
	//----------------------------
	class CRenderAdp
	{
    public:
        virtual void applyMaterial(const TMaterial& mat) const = 0;
        virtual ~CRenderAdp(){};
		//---- Mesh Adp
		struct CMeshAdp { virtual void render() const{}; };
		//---- Texture Adp
		struct CTexAdp {
			virtual void render() const{};
			bool m_isRepeat = false;
			bool m_isValid = false;
		};
        
        //---- Factory
        virtual ctl::Sp<CMeshAdp> createMeshAdp(const TMeshData& rMshd) const = 0;
        virtual ctl::Sp<CTexAdp> createTextureAdp(const ctl::IcImg& rImg) const = 0;
        virtual ctl::Sp<CTexAdp> createTextureAdp(const std::string& sFile) const = 0;
        
        //---- Lighting
        virtual void setLight(const TLight& light,
                              const TVec3& ecPos,   // Light pos in eye space
                              const TVec3& ecDir,   // Light dir in eye space
                              int lightIdx,
                              int totalLightNum) const = 0;
        //---- Render Matrix
        struct TRenderMatrix
        {
            TMat4	m_matModel;
            TMat4	m_matView;
            TMat4	m_matProj;
        };
        virtual void applyMatrix(TRenderMatrix& rm) =0;
	};
	
} // namespace Ic3d

#endif /* Ic3dBase_h */
