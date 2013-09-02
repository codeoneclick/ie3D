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
m_numIndices(0),
m_numUsingVertices(0),
m_numUsingIndices(0),
m_isLocked(false)
{
    assert(m_material != nullptr);
    assert(m_material->Get_Shader() != nullptr);
    m_guid = m_material->Get_Shader()->Get_Guid() + ".batch";
    
    std::shared_ptr<CVertexBuffer> vertexBuffer = std::make_shared<CVertexBuffer>(std::numeric_limits<ui16>::max(), GL_DYNAMIC_DRAW);
    SHardwareVertex* vertexData = vertexBuffer->Lock();
    memset(vertexData, 0x0, std::numeric_limits<ui16>::max() * sizeof(SHardwareVertex));
    vertexBuffer->Unlock();
   
    std::shared_ptr<CIndexBuffer> indexBuffer = std::make_shared<CIndexBuffer>(std::numeric_limits<ui16>::max(), GL_DYNAMIC_DRAW);
    ui16* indexData = indexBuffer->Lock();
    memset(indexData, 0x0, std::numeric_limits<ui16>::max() * sizeof(ui16));
    indexBuffer->Unlock();
    
    m_mesh = std::make_shared<CMesh>(m_guid, vertexBuffer, indexBuffer);
    assert(m_mesh != nullptr);
}

CBatch::~CBatch(void)
{
    
}

void CBatch::Lock(void)
{
    if(!m_isLocked)
    {
        m_meshes.clear();
        m_matrices.clear();
    }
}

void CBatch::Unlock(void)
{
    if(!m_isLocked)
    {
        m_isLocked = true;
        assert(m_matrices.size() == m_meshes.size());
        std::function<void(void)> function = [this]()
        {
            ui32 numVertices = 0;
            ui32 numIndices = 0;
            
            for(ui32 i = 0; i < m_meshes.size(); ++i)
            {
                std::shared_ptr<CMesh> mesh = m_meshes[i];
                
                glm::mat4x4& matrix = m_matrices[i];
                
                ui16* indexData_01 = m_mesh->Get_IndexBuffer()->Lock();
                ui16* indexData_02 = mesh->Get_IndexBuffer()->Lock();
                
                for(ui32 j = 0; j < mesh->Get_NumIndexes(); ++j)
                {
                    indexData_01[numIndices + j] = indexData_02[j] + numVertices;
                }
                
                SHardwareVertex* vertexData_01 = m_mesh->Get_VertexBuffer()->Lock();
                SHardwareVertex* vertexData_02 = mesh->Get_VertexBuffer()->Lock();
                for(ui32 j = 0; j < mesh->Get_NumVertexes(); ++j)
                {
                    vertexData_01[numVertices + j] = vertexData_02[j];
                    vertexData_01[numVertices + j].m_position = glm::transform(vertexData_02[numVertices + j].m_position, matrix);
                    vertexData_01[numVertices + j].m_normal = CVertexBuffer::CompressVec3(glm::transform(CVertexBuffer::UncompressU8Vec4(vertexData_02[numVertices + j].m_normal), matrix));
                }
                
                numVertices += mesh->Get_NumVertexes();
                numIndices += mesh->Get_NumIndexes();
            }
            
            std::function<void(void)> main = [this, numVertices, numIndices]()
            {
                m_mesh->Get_VertexBuffer()->Unlock(numVertices);
                m_mesh->Get_IndexBuffer()->Unlock(numIndices);
                m_numVertices = numVertices;
                m_numIndices = numIndices;
                m_isLocked = false;
            };
            gcdpp::impl::DispatchAsync(gcdpp::queue::GetMainQueue(), main);
        };
        gcdpp::impl::DispatchAsync(gcdpp::queue::GetGlobalQueue(gcdpp::queue::GCDPP_DISPATCH_QUEUE_PRIORITY_LOW), function);
    }
}

void CBatch::Batch(const std::shared_ptr<CMesh>& _mesh, const glm::mat4x4 &_matrix)
{
    if(!m_isLocked)
    {
        m_meshes.push_back(_mesh);
        m_matrices.push_back(_matrix);
    }
}

void CBatch::Draw(void)
{
    assert(m_mesh != nullptr);
    assert(m_material != nullptr);
    assert(m_material->Get_Shader() != nullptr);
    if(m_numIndices != 0 && m_numVertices != 0)
    {
        m_material->Bind();
        
        m_mesh->Bind(m_material->Get_Shader()->Get_Attributes());
        m_mesh->Draw(m_numIndices);
        m_mesh->Unbind(m_material->Get_Shader()->Get_Attributes());
        
        m_material->Unbind();
    }
}