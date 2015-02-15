//
//  CMeshCommiter_ie3Dmesh.cpp
//  ie3D-Core
//
//  Created by Sergey Sergeev on 2/15/15.
//  Copyright (c) 2015 Sergey Sergeev. All rights reserved.
//

#include "CMeshCommiter_ie3Dmesh.h"
#include "CMesh.h"
#include "CVertexBuffer.h"
#include "CIndexBuffer.h"

CMeshCommiter_ie3Dmesh::CMeshCommiter_ie3Dmesh(const std::string& guid, ISharedResourceRef resource) :
IResourceCommiter(guid, resource)
{
    
}

CMeshCommiter_ie3Dmesh::~CMeshCommiter_ie3Dmesh(void)
{
    
}

void CMeshCommiter_ie3Dmesh::commit(void)
{
    m_status = E_COMMITER_STATUS_INPROGRESS;
    assert(m_resource != nullptr);
    
    CSharedMesh mesh = std::static_pointer_cast<CMesh>(m_resource);
    
    CSharedVertexBuffer vertexBuffer = std::make_shared<CVertexBuffer>(mesh->getNumVertices(),
                                                                       GL_STATIC_DRAW);
    SAttributeVertex* vertexData = vertexBuffer->lock();
    
    for(ui32 i = 0; i < mesh->getNumVertices(); ++i)
    {
        vertexData[i].m_position = mesh->getVertexData()[i].m_position;
        glm::vec2 texcoord = mesh->getVertexData()[i].m_texcoord;
        vertexData[i].m_texcoord = glm::packUnorm2x16(texcoord);
        glm::vec3 normal = mesh->getVertexData()[i].m_normal;
        vertexData[i].m_normal = glm::packSnorm4x8(glm::vec4(normal.x, normal.y, normal.z, 0.0));
        glm::vec3 tangent = mesh->getVertexData()[i].m_tangent;
        vertexData[i].m_tangent = glm::packSnorm4x8(glm::vec4(tangent.x, tangent.y, tangent.z, 0.0));
        
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
    
    GLenum error = glGetError();
    assert(error == GL_NO_ERROR);
    
    m_status = E_COMMITER_STATUS_SUCCESS;
    IResourceCommiter::onResourceDataCommitFinished(vertexBuffer);
    IResourceCommiter::onResourceDataCommitFinished(indexBuffer);
}