//
//  CMeshCommiter_MDL.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/26/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CMeshCommiter_MDL.h"
#include "CMesh.h"

CMeshCommiter_MDL::CMeshCommiter_MDL(const std::string& _guid, std::shared_ptr<CMeshHeader> _header, std::shared_ptr<IResource> _resource) :
IResourceCommiter(_guid, _resource),
m_header(_header)
{
    
}

CMeshCommiter_MDL::~CMeshCommiter_MDL(void)
{
    
}

void CMeshCommiter_MDL::Commit(void)
{
    m_status = E_COMMITER_STATUS_INPROGRESS;
    assert(m_resource != nullptr);
    assert(m_header != nullptr);
    
    std::shared_ptr<CMesh> mesh = std::static_pointer_cast<CMesh >(m_resource);
    std::shared_ptr<CMeshHeader> header = mesh->_Get_Header();
    
    std::shared_ptr<CVertexBuffer> vertexBuffer = std::make_shared<CVertexBuffer>(header->Get_NumVertexes(), GL_DYNAMIC_DRAW);
    SVertex* vertexData = vertexBuffer->Lock();
	memcpy(vertexData, header->Get_VertexData(), sizeof(SVertex) * header->Get_NumVertexes());
    vertexBuffer->Unlock();
    
    std::shared_ptr<CIndexBuffer> indexBuffer = std::make_shared<CIndexBuffer>(header->Get_NumIndexes(), GL_STATIC_DRAW);
    ui16* indexData = indexBuffer->Lock();
	memcpy(indexData, header->Get_IndexData(), sizeof(ui16) * header->Get_NumIndexes());
    indexBuffer->Unlock();
    
    mesh->_Set_Handlers(vertexBuffer, indexBuffer);
    
    m_status = E_COMMITER_STATUS_SUCCESS;
}