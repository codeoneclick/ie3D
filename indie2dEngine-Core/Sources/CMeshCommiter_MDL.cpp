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
    
    std::shared_ptr<CSVertexBuffer> softwareVertexBuffer = std::make_shared<CSVertexBuffer>(header->m_vertexData, header->m_numVertexes);

    std::shared_ptr<CSIndexBuffer> softwareIndexBuffer = std::make_shared<CSIndexBuffer>(header->m_indexData, header->m_numIndexes);
    mesh->_Set_Handlers(softwareVertexBuffer, softwareIndexBuffer);
    
    m_status = E_COMMITER_STATUS_SUCCESS;
}