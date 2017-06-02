//
//  IcMeshDataObj.cpp
//  IcEngLib
//
//  Created by Sherman Chen on 6/2/17.
//  Copyright (c) 2017 Simviu Technology Inc.
//  All rights reserved.
//  http://www.simviu.com/dev
//


#include "Ic3d.h"

namespace Ic3d{
	using namespace std;
	using namespace ctl;
	//-----------------------------------------------------
	//	IcMeshData
	//-----------------------------------------------------
	bool IcMeshData::saveFileObj(const std::string& sFile) const
	{
		ofstream ofs;
		ofs.open(sFile.c_str());
		if(!ofs.is_open())
		{ logFileErr(sFile); return false; }
		
		ofs << "#---- IcMeshData saveFileObj()" << endl;
		ofs << "o meshobj" << endl;
		
		//---- Verts
		for(const auto& v : m_verts.getAry())
			ofs <<"v "<< v.x << " "<< v.y <<" "<< v.z << endl;
		//---- Textures
		for(const auto& vt : m_texCords.getAry())
			ofs <<"vt "<< vt.x << " "<< vt.y  << endl;
		//---- Normals
		for(const auto& n : m_normals.getAry())
			ofs <<"vn "<< n.x << " "<< n.y <<" "<< n.z << endl;
		
		//---- Faces
		for(const auto& f : m_faces.getAry())
		{
			ofs << "f ";
			for(int i=0;i<3;i++)
			{
				const auto& v = f.m_verts[i];
				ofs << v.m_vi+1 << "/"<< v.m_ti+1 <<"/"<< v.m_ni+1 << " ";
			}
			ofs << endl;
		}
		ofs.close();
		return true;
	}

}
