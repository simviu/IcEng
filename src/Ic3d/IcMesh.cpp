//
//  icube_mesh_new.cpp
//  Warbirds
//
//  Created by Sherman Chen on 6/4/13.
//
//

#include "Ic3d.h"
#include "IcRenderEng.h"


namespace Ic3d{

	//---------------------------------------------
	//	IcMesh::draw
	//---------------------------------------------
	void IcMesh::draw() const
	{
		if(m_renderAdp!=NULL)
			m_renderAdp->render();
		
	}
	//---------------------------------------------
	//	IcMesh::IcMesh
	//---------------------------------------------
	IcMesh::IcMesh(const TMeshData& rMshd)
	{
		//---- Use Mesh Data to create Render Mesh
		auto pRE = IcRenderEng::getInstance();
        auto pAdp = pRE->getCurRenderAdp();
		m_renderAdp = pAdp->createMeshAdp(rMshd);
	}

} // namespace Ic3d
