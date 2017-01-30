//
//  IcModelLoader.cpp
//  DevEng
//
//  Created by Sherman Chen on 3/11/16.
//  Copyright (c) 2016 Simviu Technology Inc.
//  All rights reserved.
//  http://www.simviu.com/dev
//

#include "Ic3d.h"

#include <fstream>
#include <string.h>

namespace Ic3d {
    using namespace std;
	using namespace ctl;

    
	static const bool K_dbgEnLog = false;
	
	//---- Quad face :4, Triangle Only : 3
	static const int K_N_faceVertNumSupported = 3;
	//-----------------------------------------
	//	chkParaNum
	//-----------------------------------------
	bool IcModelLoader::chkParaNum(size_t lnCnt,
								   size_t paraNum,
								   size_t paraNumExp)
	{
		if(paraNum>=paraNumExp) return true;
		string sLine = "[Ln "+v2s(lnCnt)+"]";
		logErr(sLine + "Insufficient para num, "+
			   "got "+v2s(paraNum)+", "+
			   "expect "+v2s(paraNumExp));
		return false;
	}
	//-----------------------------------------
	//	loadFileObj
	//-----------------------------------------
	bool IcModelLoader::loadFile(const string& sFile)
	{
		//---- So far obj only
		m_sPath = ctl::sFilePath(sFile);
		return loadFileObj(sFile);
	}
	//-----------------------------------------
	//	createAddMesh
	//-----------------------------------------
	void IcModelLoader::createAddModel(const std::string& sName,
									   const TMtlItem& mtl)
	{
		size_t curFaceNum = m_meshData.m_faces.size();
		if(curFaceNum==0) return;
		size_t faceNum = curFaceNum - m_subMeshFaceStart;
		
		//---- Create sub-meshData by current data
		IcMeshData mshd;
		mshd.m_sName = sName;
		m_meshData.getSubMesh(mshd, m_subMeshFaceStart, faceNum);
		if(K_dbgEnLog)
		{
			logDbg("==== SubMesh:====");
			mshd.dbgPrint();
		}
		//---- Create IcMesh, set RenderMesh and Material
		auto pMesh = makeSp<IcMesh>(mshd);
        
		//---- Create Model
		auto pModel = ctl::makeSp<IcModel>();
		pModel->setMesh(pMesh);
		pModel->setMaterial(mtl.m_pMat);
		pModel->setTexture(mtl.m_pTex);
		m_models.add(pModel);
		
		m_subMeshFaceStart = curFaceNum;
		m_info.m_N_face += curFaceNum;
	}
	//-----------------------------------------
	//	checkLoadTex
	//-----------------------------------------
	ctl::Sp<IcTexture> IcModelLoader::checkLoadTex(const std::string& sFile)
	{
		//---- Check re-use
		auto p = m_texLib.lookup(sFile);
		if(p!=nullptr) return p;
		
		//---- Load Texture from file
		string sPath = m_sPath + sFile;
		p = makeSp<IcTexture>(sPath);
		m_texLib.store(sFile, p);
		return p;
	}

	//-----------------------------------------
	//	loadFileObj
	//-----------------------------------------
	bool IcModelLoader::loadFileObj(const string& sFile)
	{
		bool isOK = true;
		ifstream ifs(sFile);
		if(!ifs.is_open())
		{
			logFileErr(sFile);
			return false;
		}
		
		//-------------------------------
		//	main loop
		//-------------------------------
		size_t lnCnt=0;
		string sLine;
		ctl::Sp<TMtlItem> pCurMtl = ctl::makeSp<TMtlItem>();
		string m_sNameMesh;

		while (getline(ifs, sLine))
		{
			lnCnt++;
			//---- Line check
			if(sLine.size()==0) continue;
			if(sLine[0]=='#') continue;
			auto tkns = ctl::s2tokens(sLine, " \t\r\n");
			if(tkns.size()==0) continue;
			
			string sCmd = tkns[0];
			string sLnCnt = "[Ln "+v2s(lnCnt)+"]:";
			//---- Pack Rest of Args
			size_t N = tkns.size();
			string sArgs;
			for(int i=1;i<N;i++) sArgs += tkns[i]+",";

			//---------------------------
			//	v
			//---------------------------
			if(sCmd == "v")
			{
				if(!chkParaNum(lnCnt, N, 4))
					return false;
				TVec3 v;
				isOK &= s2vec(sArgs, v);
				m_meshData.addVert(v);
			}
			//---------------------------
			//	vt
			//---------------------------
			else if(sCmd == "vt")
			{
				if(!chkParaNum(lnCnt, tkns.size(),3))
					return false;
				TVec2 v;
				isOK &= s2vec(sArgs, v);
				v.y = 1.0 - v.y;
				//---- png file y reverse
				m_meshData.addTexCo(v);
				
			}
			//---------------------------
			//	vn
			//---------------------------
			else if(sCmd == "vn")
			{
				if(!chkParaNum(lnCnt, tkns.size(),4))
					return false;
				TVec3 n;
				isOK &= s2vec(sArgs, n);
				m_meshData.addNorm(n);
			}
			//---------------------------
			//	f
			//---------------------------
			else if(sCmd == "f")
			{
				TMeshData::TFaceIdx face;
				bool hasTex = pCurMtl->m_pTex !=nullptr;
			
				//---- At least 3 (Triangle)
				if(!chkParaNum(lnCnt, tkns.size(),4))
					return false;
				//---- Check if it's Quad face
				if(tkns.size()>K_N_faceVertNumSupported+1)
				{
					logErr(sLnCnt + "Too many vert num for a face.(Triangle:3, Quad:4...)");
					return false;
				}
				
				//---- Loop points
				for(int k=0;k<3;k++)
				{
					string sPoint = tkns[k+1];
					sReplace(sPoint, "//", "/1/");
					auto tkni = ctl::s2tokens(sPoint, "/");
					if(!chkParaNum(lnCnt, tkni.size(),3))
					{
						logErr("Face index incorrect (v/t/n or v//n)");
						// TODO: support OBJ without n or t
						return false;
					}
					auto& rVert = face.m_verts[k];
					ctl::s2v(tkni[0], rVert.m_vi);
					ctl::s2v(tkni[1], rVert.m_ti);
					ctl::s2v(tkni[2], rVert.m_ni);
					//---- OBJ file index start from 1, instead of 0
					rVert.m_vi --;
					rVert.m_ti --;
					rVert.m_ni --;
				}
				m_meshData.addTrian(face);
			}
			//------------------------------------
			else if(sCmd == "mtllib")
			{
				if(!chkParaNum(lnCnt, tkns.size(),2))
					return false;
				loadFileMtl(m_sPath + tkns[1]);
			}
			//------------------------------------
			else if(sCmd == "usemtl")
			{
				if(!chkParaNum(lnCnt, tkns.size(),2))
					return false;
				auto pMtl = m_mtlLib.lookup(tkns[1]);
				if(pMtl!=nullptr) pCurMtl = pMtl;
			}
			//------------------------------------
			else if(sCmd == "s")
			{
				//	Syst::logStr("'s' To be implemented \n");
			}
			//------------------------------------
			else if(sCmd == "g")
			{
				//	Syst::logStr("'g' To be implemented \n");
			}
			//------------------------------------
			else if(sCmd == "o")
			{
				createAddModel(m_sNameMesh, *pCurMtl);
				
				//---- Note Mesh Name is for next
				if(tkns.size()>1)
					m_sNameMesh = tkns[1];
			}
			//------------------------------------
			else
			{
				//	Syst::logStr("Unkown token "+tok_str+"\n");
				//	return 0;
				// just skip
			}

		}
		//---- Add Last one
		createAddModel(m_sNameMesh, *pCurMtl);
		if(K_dbgEnLog)
		{
			logDbg("==== Total Mesh:====");
			m_meshData.dbgPrint();
		}
		
		return isOK;
	}
	//-----------------------------------------
	//	loadFileMtl
	//-----------------------------------------
	bool IcModelLoader::loadFileMtl(const string& sFile)
	{
		bool isOK = true;
		//---- Open File
		ifstream ifs(sFile);
		if(!ifs.is_open())
		{
			ctl::logFileErr(sFile);
			return false;
		}
		
		//---- Read file
		auto pMtl = makeSp<TMtlItem>();
		auto pCurMat = makeSp<IcMaterial>();
		
		string sMatName;
		string sLine;
		size_t lnCnt=0;
		while (getline(ifs, sLine))
		{
			lnCnt ++;
			if(sLine.size()==0) continue;
			if(sLine[0]=='#') continue;
			auto tkns = ctl::s2tokens(sLine, " \t\r\n");
			if(tkns.size()==0) continue;
			//-----------------------
			//	newmtl (New Material)
			//-----------------------
			string sCmd = tkns[0];
			size_t N = tkns.size();
			
			//---- Pack Rest of Args
			string sArgs;
			for(int i=1;i<N;i++) sArgs += tkns[i]+",";
			
			//-----------------------
			//	newmtl
			//-----------------------
			if(sCmd=="newmtl")
			{
				//---- Create New One
				sMatName = (N>=2)? tkns[1] :"";
				pMtl = makeSp<TMtlItem>();
				pCurMat = makeSp<IcMaterial>();
				pMtl->m_pMat = pCurMat;
				pCurMat->m_sName = sMatName;
				if(sMatName!="")
					m_mtlLib.store(sMatName, pMtl);
			}
			//-----------------------
			//	Ns (shiness)
			//-----------------------
			else if(sCmd=="Ns" )
			{	if(N>=2) s2v(sArgs, pCurMat->m_shns);
            }
			//-----------------------
			//	Ka (ambient)
			//-----------------------
			else if(sCmd=="Ka" )
			{	if(N>=4) s2color(sArgs, pCurMat->m_amb); }
			//-----------------------
			//	Kd (diffuse)
			//-----------------------
			else if(sCmd=="Kd" )
			{	if(N>=4) s2color(sArgs, pCurMat->m_dfs); }
			//-----------------------
			//	Ks (specular)
			//-----------------------
			else if(sCmd=="Ks" )
			{	if(N>=4) s2color(sArgs, pCurMat->m_spc); }
            //-----------------------
            //	Ke (emission)
            //-----------------------
            else if(sCmd=="Ke" )
            {	if(N>=4) s2color(sArgs, pCurMat->m_ems); }
			//-----------------------
			//	d (Dissolve/Transparency)
			//-----------------------
			else if(sCmd=="d" )
			{	if(N>=2) s2v(sArgs, pCurMat->m_topAlpha); }
			//-----------------------
			//	map_Kd  (Texture
			//-----------------------
			else if(sCmd=="map_Kd" )
			{
				chkParaNum(lnCnt, N, 2);
				if(N>=2)
				{
					string sFile = tkns[1];
					auto pTex = checkLoadTex(sFile);
					pMtl->m_pTex = pTex;
				}
			}
			
		}
		//---- Save Lat mat
	//	if(sMatName!="")
	//		m_matLib.store(sMatName, pMat);
	
		return isOK;
	}

}
