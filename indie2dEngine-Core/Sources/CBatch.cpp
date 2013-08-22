//
//  CBatch.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 8/22/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CBatch.h"
#include "CMaterial.h"
#include "CMesh.h"
#include "CShader.h"

CBatch::CBatch(std::shared_ptr<CMaterial> _material) :
m_material(_material),
m_numVertices(0),
m_numIndices(0)
{
    assert(m_material != nullptr);
    assert(m_material->Get_Shader() != nullptr);
    m_guid = m_material->Get_Shader()->Get_Guid() + ".batch";
    std::shared_ptr<CVertexBuffer> vertexBuffer = std::make_shared<CVertexBuffer>(std::numeric_limits<ui16>::max() * 3, GL_DYNAMIC_DRAW);
    SVertex* vertexData = vertexBuffer->Lock();
    memset(vertexData, 0x0, std::numeric_limits<ui16>::max() * 3 * sizeof(SVertex));
    
    std::shared_ptr<CIndexBuffer> indexBuffer = std::make_shared<CIndexBuffer>(std::numeric_limits<ui16>::max(), GL_DYNAMIC_DRAW);
    ui16* indexData = indexBuffer->Lock();
    memset(indexData, 0x0, std::numeric_limits<ui16>::max() * sizeof(ui16));
    
    m_mesh = std::make_shared<CMesh>(m_guid, vertexBuffer, indexBuffer);
    assert(m_mesh != nullptr);
}

CBatch::~CBatch(void)
{
    
}

void CBatch::Erase(void)
{
    m_numVertices = 0;
    m_numIndices = 0;
}

void CBatch::Batch(std::shared_ptr<CMesh> _mesh, const glm::mat4x4 &_matrix)
{
    assert((m_numVertices + _mesh->Get_NumVertexes()) <= m_mesh->Get_NumVertexes());
    assert((m_numIndices + _mesh->Get_NumIndexes()) <= m_mesh->Get_NumIndexes());
    
    ui16* indexData_01 = m_mesh->Get_IndexBuffer()->Lock();
    ui16* indexData_02 = _mesh->Get_IndexBuffer()->Lock();
    
    for(ui32 i = 0; i < _mesh->Get_NumIndexes(); ++i)
    {
        indexData_01[m_numIndices + i] = indexData_02[i] + m_numVertices;
    }
    m_numIndices += _mesh->Get_NumIndexes();
    
    SVertex* vertexData_01 = m_mesh->Get_VertexBuffer()->Lock();
    SVertex* vertexData_02 = _mesh->Get_VertexBuffer()->Lock();
    for(ui32 i = 0; i < _mesh->Get_NumVertexes(); ++i)
    {
        vertexData_01[m_numVertices + i] = vertexData_02[i];
        vertexData_01[m_numVertices + i].m_position = glm::transform(vertexData_01[m_numVertices + i].m_position, _matrix);
        vertexData_01[m_numVertices + i].m_normal = CVertexBuffer::CompressVec3(glm::transform(CVertexBuffer::UncompressU8Vec4(vertexData_01[m_numVertices + i].m_normal), _matrix));
    }
    m_numVertices += _mesh->Get_NumVertexes();
}

void CBatch::Draw(void)
{
    assert(m_mesh != nullptr);
    assert(m_material != nullptr);
    assert(m_material->Get_Shader() != nullptr);
    if(m_numIndices != 0 && m_numVertices != 0)
    {
        m_mesh->Get_VertexBuffer()->Unlock(m_numVertices);
        m_mesh->Get_IndexBuffer()->Unlock(m_numIndices);
        
        m_material->Bind();
        
        m_mesh->Bind(m_material->Get_Shader()->Get_Guid(), m_material->Get_Shader()->Get_Attributes());
        m_mesh->Draw(m_numIndices);
        m_mesh->Unbind(m_material->Get_Shader()->Get_Guid(), m_material->Get_Shader()->Get_Attributes());
        
        m_material->Unbind();
    }
    GLenum error = glGetError();
    assert(error == GL_NO_ERROR);
}