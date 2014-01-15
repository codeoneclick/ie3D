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
    
    std::shared_ptr<CVertexBuffer> vertexBuffer = std::make_shared<CVertexBuffer>(header->_Get_NumVerticies(), GL_STATIC_DRAW);
    SHardwareVertex* vertexData = vertexBuffer->Lock();
    
    for(ui32 i = 0; i < header->_Get_NumVerticies(); ++i)
    {
        vertexData[i].m_position = header->_Get_VertexData()[i].m_position;
        vertexData[i].m_texcoord = CVertexBuffer::CompressVec2(header->_Get_VertexData()[i].m_texcoord);
        vertexData[i].m_normal = CVertexBuffer::CompressVec3(header->_Get_VertexData()[i].m_normal);
        vertexData[i].m_tangent = CVertexBuffer::CompressVec3(header->_Get_VertexData()[i].m_tangent);
        
        assert(header->_Get_VertexData()[i].m_bones.size() <= 4);
        vertexData[i].m_color = glm::u8vec4(header->_Get_VertexData()[i].m_bones.size() >= 1 ? static_cast<ui8>(header->_Get_VertexData()[i].m_bones[0].m_id) : 0,
                                              header->_Get_VertexData()[i].m_bones.size() >= 2 ? static_cast<ui8>(header->_Get_VertexData()[i].m_bones[1].m_id) : 0,
                                              header->_Get_VertexData()[i].m_bones.size() >= 3 ? static_cast<ui8>(header->_Get_VertexData()[i].m_bones[2].m_id) : 0,
                                              header->_Get_VertexData()[i].m_bones.size() == 4 ? static_cast<ui8>(header->_Get_VertexData()[i].m_bones[3].m_id) : 0);
        
        vertexData[i].m_extra = glm::u8vec4(header->_Get_VertexData()[i].m_bones.size() >= 1 ? static_cast<ui8>(header->_Get_VertexData()[i].m_bones[0].m_weigth * 255.0f) : 0,
                                            header->_Get_VertexData()[i].m_bones.size() >= 2 ? static_cast<ui8>(header->_Get_VertexData()[i].m_bones[1].m_weigth * 255.0f) : 0,
                                            header->_Get_VertexData()[i].m_bones.size() >= 3 ? static_cast<ui8>(header->_Get_VertexData()[i].m_bones[2].m_weigth * 255.0f) : 0,
                                            header->_Get_VertexData()[i].m_bones.size() == 4 ? static_cast<ui8>(header->_Get_VertexData()[i].m_bones[3].m_weigth * 255.0f) : 0);
    }
    vertexBuffer->Unlock();
    
    
    std::shared_ptr<CIndexBuffer> indexBuffer = std::make_shared<CIndexBuffer>(header->_Get_NumIndices(), GL_STATIC_DRAW);
    ui16* indexData = indexBuffer->Lock();
	memcpy(indexData, header->_Get_IndexData(), sizeof(ui16) * header->_Get_NumIndices());
    indexBuffer->Unlock();

    mesh->_Set_Handlers(vertexBuffer, indexBuffer);
    
    m_status = E_COMMITER_STATUS_SUCCESS;
}