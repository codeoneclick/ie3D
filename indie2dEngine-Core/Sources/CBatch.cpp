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
#include "CAnimationMixer.h"

CBatch::CBatch(const std::string& _mode, ui32 _renderQueuePosition, std::shared_ptr<CMaterial> _material, const std::function<void(std::shared_ptr<CMaterial>)>& _bind) :
m_material(_material),
m_renderQueuePosition(_renderQueuePosition),
m_bind(_bind),
m_mode(_mode),
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
        m_transformations.clear();
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
            ui32 numTransformations = 0;
            
            for(ui32 i = 0; i < m_meshes.size(); ++i)
            {
                std::shared_ptr<CMesh> mesh = std::get<0>(m_meshes[i]);
                std::shared_ptr<CAnimationMixer> mixer = std::get<1>(m_meshes[i]);
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
                    glm::vec3 position = glm::vec3(0.0f);
                    glm::vec3 normal = glm::vec3(0.0f);
                    glm::vec3 tangent = glm::vec3(0.0f);
                    
                    for(ui32 k = 0; k < 4; ++k)
                    {
                        f32 weight = static_cast<f32>(vertexData_02[j].m_extra[k]) / 255.0f;
                        position += glm::transform(vertexData_02[j].m_position, m_transformations[vertexData_02[j].m_color[k] + numTransformations]) * weight;
                        normal += glm::transform(CVertexBuffer::UncompressU8Vec4(vertexData_02[j].m_normal), m_transformations[vertexData_02[j].m_color[k] + numTransformations]) * weight;
                        tangent += glm::transform(CVertexBuffer::UncompressU8Vec4(vertexData_02[j].m_tangent), m_transformations[vertexData_02[j].m_color[k] + numTransformations]) * weight;
                    }
                    
                    vertexData_01[numVertices + j] = vertexData_02[j];
                    vertexData_01[numVertices + j].m_position = glm::transform(position, matrix);
                    vertexData_01[numVertices + j].m_normal = CVertexBuffer::CompressVec3(glm::transform(normal, matrix));
                    vertexData_01[numVertices + j].m_tangent = CVertexBuffer::CompressVec3(glm::transform(tangent, matrix));
                }
                
                numVertices += mesh->Get_NumVertexes();
                numIndices += mesh->Get_NumIndexes();
                numTransformations += mixer->Get_TransformationSize();
                assert(numTransformations < 255);
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

void CBatch::Batch(const std::tuple<std::shared_ptr<CMesh>, std::shared_ptr<CAnimationMixer>>& _mesh, const glm::mat4x4 &_matrix)
{
    if(!m_isLocked)
    {
        m_meshes.push_back(_mesh);
        m_matrices.push_back(_matrix);
        
        std::shared_ptr<CAnimationMixer> mixer = std::get<1>(_mesh);
        for(ui32 i = 0; i < mixer->Get_TransformationSize(); ++i)
        {
            m_transformations.push_back(mixer->Get_Transformations()[i]);
        }
    }
}

i32 CBatch::_OnQueuePosition(void)
{
    return m_renderQueuePosition;
}

bool CBatch::_OnOcclusion(void)
{
    return false;
}

ui32 CBatch::_OnGet_NumTriangles(void)
{
    return 0;
}

void CBatch::_OnBind(const std::string& _mode)
{
    
}

void CBatch::_OnDraw(const std::string& _mode)
{
    assert(m_mesh != nullptr);
    assert(m_material != nullptr);
    assert(m_material->Get_Shader() != nullptr);
    if(m_numIndices != 0 && m_numVertices != 0)
    {
        m_material->Bind();
        m_bind(m_material);
        
        m_material->Get_Shader()->Set_MatrixArray4x4(&m_transformations[0], m_transformations.size(), E_SHADER_UNIFORM_MATRIX_BONES);
        
        m_mesh->Bind(m_material->Get_Shader()->Get_Attributes());
        m_mesh->Draw(m_numIndices);
        m_mesh->Unbind(m_material->Get_Shader()->Get_Attributes());
        
        m_material->Unbind();
    }
}

void CBatch::_OnUnbind(const std::string& _mode)
{
    
}

void CBatch::_OnDebugDraw(const std::string& _mode)
{
    
}

void CBatch::_OnBatch(const std::string& _mode)
{
    
}
