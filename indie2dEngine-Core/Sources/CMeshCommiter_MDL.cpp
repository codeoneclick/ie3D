//
//  CMeshCommiter_MDL.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/26/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CMeshCommiter_MDL.h"
#include "CMesh.h"
#include "CVertexBuffer.h"
#include "CIndexBuffer.h"

CMeshCommiter_MDL::CMeshCommiter_MDL(const std::string& guid, ISharedResourceRef resource) :
IResourceCommiter(guid, resource)
{
    
}

CMeshCommiter_MDL::~CMeshCommiter_MDL(void)
{
    
}

void CMeshCommiter_MDL::commit(void)
{
    IResourceCommiter::onResourceDataCommitStatusChanged(nullptr, E_RESOURCE_DATA_STATUS_STARTED);
    m_status = E_COMMITER_STATUS_INPROGRESS;
    assert(m_resource != nullptr);
    
    std::shared_ptr<CMesh> mesh = std::static_pointer_cast<CMesh >(m_resource);
    
    std::shared_ptr<CVertexBuffer> vertexBuffer = std::make_shared<CVertexBuffer>(mesh->getNumVertices(),
                                                                                  GL_STATIC_DRAW);
    SAttributeVertex* vertexData = vertexBuffer->lock();
    
    for(ui32 i = 0; i < mesh->getNumVertices(); ++i)
    {
        vertexData[i].m_position = mesh->getVertexData()[i].m_position;
        vertexData[i].m_texcoord = CVertexBuffer::compressVec2(mesh->getVertexData()[i].m_texcoord);
        vertexData[i].m_normal = CVertexBuffer::compressVec3(mesh->getVertexData()[i].m_normal);
        vertexData[i].m_tangent = CVertexBuffer::compressVec3(mesh->getVertexData()[i].m_tangent);
        
        assert(mesh->getVertexData()[i].m_bones.size() <= 4);
        vertexData[i].m_color = glm::u8vec4(
                                            mesh->getVertexData()[i].m_bones.size() >= 1 ? static_cast<ui8>(mesh->getVertexData()[i].m_bones[0].m_id) : 0,
                                            mesh->getVertexData()[i].m_bones.size() >= 2 ? static_cast<ui8>(mesh->getVertexData()[i].m_bones[1].m_id) : 0,
                                            mesh->getVertexData()[i].m_bones.size() >= 3 ? static_cast<ui8>(mesh->getVertexData()[i].m_bones[2].m_id) : 0,
                                            mesh->getVertexData()[i].m_bones.size() == 4 ? static_cast<ui8>(mesh->getVertexData()[i].m_bones[3].m_id) : 0);
        
        vertexData[i].m_extra = glm::u8vec4(
                                            mesh->getVertexData()[i].m_bones.size() >= 1 ? static_cast<ui8>(mesh->getVertexData()[i].m_bones[0].m_weigth * 255.0f) : 0,
                                            mesh->getVertexData()[i].m_bones.size() >= 2 ? static_cast<ui8>(mesh->getVertexData()[i].m_bones[1].m_weigth * 255.0f) : 0,
                                            mesh->getVertexData()[i].m_bones.size() >= 3 ? static_cast<ui8>(mesh->getVertexData()[i].m_bones[2].m_weigth * 255.0f) : 0,
                                            mesh->getVertexData()[i].m_bones.size() == 4 ? static_cast<ui8>(mesh->getVertexData()[i].m_bones[3].m_weigth * 255.0f) : 0);
    }
    vertexBuffer->unlock();
    
    std::shared_ptr<CIndexBuffer> indexBuffer = std::make_shared<CIndexBuffer>(mesh->getNumIndices(), GL_STATIC_DRAW);
    ui16* indexData = indexBuffer->lock();
	memcpy(indexData, &mesh->getIndexData()[0], sizeof(ui16) * mesh->getNumIndices());
    indexBuffer->unlock();

    IResourceCommiter::onResourceDataCommitStatusChanged(vertexBuffer, E_RESOURCE_DATA_STATUS_PROGRESS);
    IResourceCommiter::onResourceDataCommitStatusChanged(indexBuffer, E_RESOURCE_DATA_STATUS_PROGRESS);
    m_status = E_COMMITER_STATUS_SUCCESS;
    IResourceCommiter::onResourceDataCommitStatusChanged(nullptr, E_RESOURCE_DATA_STATUS_FINISHED);
}