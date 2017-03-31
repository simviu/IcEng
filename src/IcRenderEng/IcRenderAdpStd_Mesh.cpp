//
//  IcRenderAdpStd_Mesh.cpp
//  DevEng
//
//  Created by Sherman Chen on 9/28/16.
//  Copyright (c) 2016 Simviu Technology Inc.
//  All rights reserved.
//  http://www.simviu.com/dev
//



#include "IcRenderAdpStd.h"
#include "OpenGLinc.h"

namespace Ic3d
{
	using namespace std;
	using namespace ctl;
    const static bool K_enDupVertCheck = false;

	//---------------------------------------------
	//	CImportHelper
	//---------------------------------------------
	typedef TMeshData::TFaceIdx TTFaceIdx;
	typedef TTFaceIdx::TVertIdx TTVertIdx;
	struct CImportHelper
	{
		//---- Will check index, if equal, use the saved vert.
		size_t checkAddPoint(const TTVertIdx& vNew)
		{
			auto& rAry = m_ary.getAry();
            
            if(!K_enDupVertCheck)
            {
                m_ary.add(vNew);
                return m_ary.size()-1;
            }
            
			size_t i = 0;
			for(auto& v : rAry)
			{
				if(v.m_vi == vNew.m_vi &&
				   v.m_ti == vNew.m_ti &&
				   v.m_ni == vNew.m_ni )
					return i;
				i++;
			}
			
			m_ary.add(vNew);
			return i;
		};
		SafeAry<TTVertIdx>	m_ary;
	protected:
	};
	
	//---------------------------------------------
	//	CMeshAdpStd::CMeshAdpStd
	//---------------------------------------------
	CMeshAdpStd::CMeshAdpStd(const TMeshData&  rMeshData,
                             const TShaderData& rShdrDt)
    :m_rShaderData(rShdrDt)
	{
		m_sName = rMeshData.m_sName;
		size_t N = rMeshData.m_faces.size();
		CImportHelper helper;
		for(size_t i=0;i<N;i++)
		{
			TMeshData::TFaceIdx face;
			rMeshData.m_faces.getAt(i, face);
			TTrianIdx trianIdx;
			for(int k=0;k<3;k++)	// TODO: 4?
			{
				auto& v = face.m_verts[k];
				size_t idx = helper.checkAddPoint(v);
				trianIdx.idx[k] = idx;
			}
			m_aryTrianIdx.push_back(trianIdx);
            if(i>= N-2)
            { int k=0; }
		}
		//---- vbo point
        size_t i=0; // dbg
		for(const auto& vIdx : helper.m_ary.getAry())
		{
			TVec3 v; TVec2 t; TVec3 n;
			rMeshData.m_verts.getAt(vIdx.m_vi, v);
			rMeshData.m_texCords.getAt(vIdx.m_ti, t);
			rMeshData.m_normals.getAt(vIdx.m_ni, n);
			TVertTN vTN = {v.x, v.y, v.z,
				t.x, t.y, n.x, n.y, n.z};
			m_aryPoint.push_back(vTN);
            //---- dbg
            if(i++ > N*3 - 10)
            { int k=0; }
            
        }
		
		genVboBufData();
		sendBufToGL();
	}
	
	//---------------------------------------------
	CMeshAdpStd::~CMeshAdpStd	()
	{
		if(m_isValid)
		{
			glDeleteBuffers(1, &m_vboBufGLidx);
			glDeleteBuffers(1, &m_vboTrianBufGLidx);
		}
	};
	
	//---------------------------------------------
	// genVboBufData
	//---------------------------------------------
	
	void CMeshAdpStd::genVboBufData()
	{
		
		size_t i=0;
		m_vboPntNum = m_aryPoint.size();
		m_vboTrianIdxNum = m_aryTrianIdx.size();
		m_bufPnt.setSize(sizeof(TVertTN)*m_vboPntNum);
		m_bufIdx.setSize(sizeof(TTrianIdx)*m_vboTrianIdxNum);
		
		//-----------------
		//	vbo buf point data
		//-----------------
		size_t sz_pnt = sizeof(TVertTN);
		for(i=0;i<m_vboPntNum;i++)
		{
			const ctl::TByte* p =
			reinterpret_cast<ctl::TByte*>(&m_aryPoint[i]);
			m_bufPnt.fill(p, i*sz_pnt, sz_pnt);
		}
		
		//-----------------
		//	vbo index data
		//-----------------
		size_t sz_idx = sizeof(TTrianIdx);
		for(i=0;i<m_vboTrianIdxNum;i++)
		{
			const ctl::TByte* p =
			reinterpret_cast<ctl::TByte*>(&m_aryTrianIdx[i]);
			m_bufIdx.fill(p, i*sz_idx, sz_idx);
		}
		
		//--------------------------
		// OK to delete src data
		//--------------------------
		m_aryPoint.clear();
		m_aryTrianIdx.clear();
	}
	
#ifndef USE_OPENGL1
#define VBO_BUF_OFST(bytes) ((GLubyte*)NULL+(bytes))
	
	
	//---------------------------------------------
	//	draw()
	//---------------------------------------------
	void CMeshAdpStd::render() const
	{
		//----------------------------------------------
		// applyMatrix
		//----------------------------------------------
        const auto& glLoc = m_rShaderData.m_attrLoc;
		
		//----------------------------------------------
		//---- vbo point data
		glBindBuffer(GL_ARRAY_BUFFER, m_vboBufGLidx);
		
		//------- Tell shader
		glEnableVertexAttribArray(glLoc.m_vert);
		glVertexAttribPointer(glLoc.m_vert, 3, GL_FLOAT, GL_FALSE, sizeof(TVertTN),0);
		
		glEnableVertexAttribArray(glLoc.m_tex);
		glVertexAttribPointer(glLoc.m_tex, 2, GL_FLOAT, GL_FALSE, sizeof(TVertTN),VBO_BUF_OFST(3*sizeof(float)));
		
		glEnableVertexAttribArray(glLoc.m_norm);
		glVertexAttribPointer(glLoc.m_norm, 3, GL_FLOAT, GL_FALSE, sizeof(TVertTN),VBO_BUF_OFST(5*sizeof(float)));
		
		
		//----------------------------------------------
		//---- vbo idx data
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vboTrianBufGLidx);
		glDrawElements(GL_TRIANGLES, ((m_vboTrianIdxNum) * 3), GL_UNSIGNED_INT,0);
        
        //---- Check Error
#if DEBUG
        GLenum err = glGetError();
        if(err!=GL_NO_ERROR)
        {
            int e = 0;
        }
#endif
	}
	
	
	//---------------------------------------------
	//	sendBufToGL()
	//---------------------------------------------
	void	CMeshAdpStd::sendBufToGL()
	{
		//------------------------------
		//---- VBO buf point data
		//------------------------------
		glGenBuffers(1, &m_vboBufGLidx);
		glBindBuffer(GL_ARRAY_BUFFER, m_vboBufGLidx);
		glBufferData(GL_ARRAY_BUFFER,
					 m_bufPnt.size(), m_bufPnt.getBuf(),
					 GL_STATIC_DRAW);
		//------------------------------
		//---- VBO buf triangle index data
		//------------------------------
		glGenBuffers(1, &m_vboTrianBufGLidx);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vboTrianBufGLidx);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
					 m_bufIdx.size(), m_bufIdx.getBuf(),
					 GL_STATIC_DRAW);
		m_isValid  = true;
		
		m_bufPnt.clear();
		m_bufIdx.clear();
	}
#endif// #ifndef USE_OPENGL1
	
	
}	//namespace Ic3d
