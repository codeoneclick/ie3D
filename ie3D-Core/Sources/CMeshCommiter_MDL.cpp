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
    m_status = E_COMMITER_STATUS_INPROGRESS;
    assert(m_resource != nullptr);
    
    CSharedMesh mesh = std::static_pointer_cast<CMesh>(m_resource);
    
    CSharedVertexBuffer vbo = std::make_shared<CVertexBuffer>(mesh->getNumRawVertices(),
                                                              GL_STATIC_DRAW);
    SAttributeVertex* vertices = vbo->lock();
    
    for(ui32 i = 0; i < mesh->getNumRawVertices(); ++i)
    {
        vertices[i].m_position = mesh->getRawVertices()[i].m_position;
        glm::vec2 texcoord = mesh->getRawVertices()[i].m_texcoord;
        vertices[i].m_texcoord = glm::packUnorm2x16(texcoord);
        glm::vec3 normal = mesh->getRawVertices()[i].m_normal;
        vertices[i].m_normal = glm::packSnorm4x8(glm::vec4(normal.x, normal.y, normal.z, 0.0));
        glm::vec3 tangent = mesh->getRawVertices()[i].m_tangent;
        vertices[i].m_tangent = glm::packSnorm4x8(glm::vec4(tangent.x, tangent.y, tangent.z, 0.0));
        
        assert(mesh->getRawVertices()[i].m_bones.size() <= 4);
        vertices[i].m_color = glm::u8vec4(mesh->getRawVertices()[i].m_bones.size() >= 1 ? static_cast<ui8>(mesh->getRawVertices()[i].m_bones[0].m_id) : 0,
                                          mesh->getRawVertices()[i].m_bones.size() >= 2 ? static_cast<ui8>(mesh->getRawVertices()[i].m_bones[1].m_id) : 0,
                                          mesh->getRawVertices()[i].m_bones.size() >= 3 ? static_cast<ui8>(mesh->getRawVertices()[i].m_bones[2].m_id) : 0,
                                          mesh->getRawVertices()[i].m_bones.size() == 4 ? static_cast<ui8>(mesh->getRawVertices()[i].m_bones[3].m_id) : 0);
        
        vertices[i].m_extra = glm::u8vec4(mesh->getRawVertices()[i].m_bones.size() >= 1 ? static_cast<ui8>(mesh->getRawVertices()[i].m_bones[0].m_weigth * 255.0f) : 0,
                                          mesh->getRawVertices()[i].m_bones.size() >= 2 ? static_cast<ui8>(mesh->getRawVertices()[i].m_bones[1].m_weigth * 255.0f) : 0,
                                          mesh->getRawVertices()[i].m_bones.size() >= 3 ? static_cast<ui8>(mesh->getRawVertices()[i].m_bones[2].m_weigth * 255.0f) : 0,
                                          mesh->getRawVertices()[i].m_bones.size() == 4 ? static_cast<ui8>(mesh->getRawVertices()[i].m_bones[3].m_weigth * 255.0f) : 0);
    }
    vbo->unlock();
    
    std::shared_ptr<CIndexBuffer> ibo = std::make_shared<CIndexBuffer>(mesh->getNumRawIndices(), GL_STATIC_DRAW);
    ui16* indices = ibo->lock();
    memcpy(indices, &mesh->getRawIndices()[0], sizeof(ui16) * mesh->getNumRawIndices());
    ibo->unlock();
    
    GLenum error = glGetError();
    assert(error == GL_NO_ERROR);
    
    m_status = E_COMMITER_STATUS_SUCCESS;
    IResourceCommiter::onResourceDataCommitFinished(vbo);
    IResourceCommiter::onResourceDataCommitFinished(ibo);
}