//
//  CMeshCommiter_MDL_01.cpp
//  ie3D-Core
//
//  Created by Sergey Sergeev on 7/19/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#include "CMeshCommiter_MDL_01.h"
#include "CMesh.h"
#include "CVertexBuffer.h"
#include "CIndexBuffer.h"

CMeshCommiter_MDL_01::CMeshCommiter_MDL_01(const std::string& guid, ISharedResourceRef resource) :
IResourceCommiter(guid, resource)
{
    
}

CMeshCommiter_MDL_01::~CMeshCommiter_MDL_01(void)
{
    
}

void CMeshCommiter_MDL_01::commit(void)
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