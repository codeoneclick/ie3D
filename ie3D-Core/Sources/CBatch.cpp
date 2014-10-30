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
#include "CQuad.h"
#include "CShader.h"
#include "CAnimationMixer.h"
#include "CVertexBuffer.h"
#include "CIndexBuffer.h"

const ui32 CBatch::k_MAX_NUM_VERTICES = 65535 / 4; // 16k vertices
const ui32 CBatch::k_MAX_NUM_INDICES = 65535 / 2;  // 32k indices
const ui32 CBatch::k_MAX_NUM_TRANSFORMATION = 255;

CBatch::CBatch(const std::string& mode,
               ui32 renderQueuePosition,
               CSharedMaterialRef material,
               const std::function<void(CSharedMaterialRef)>& materialBindImposer) :
m_material(material),
m_zOrder(renderQueuePosition),
m_materialBindImposer(materialBindImposer),
m_mode(mode),
m_numLockedVertices(0),
m_numLockedIndices(0),
m_numLockedTransformations(0),
m_numUnlockedVertices(0),
m_numUnlockedIndices(0),
m_numUnlockedTransformations(0)
{
    assert(m_material != nullptr);
    assert(m_material->getShader() != nullptr);
    m_guid = m_material->getShader()->getGuid() + ".batch";
    
    CSharedVertexBuffer vertexBuffer = std::make_shared<CVertexBuffer>(k_MAX_NUM_VERTICES, GL_STREAM_DRAW);
    SAttributeVertex* vertexData = vertexBuffer->lock();
    memset(vertexData, 0x0, k_MAX_NUM_VERTICES * sizeof(SAttributeVertex));
    vertexBuffer->unlock();
   
    CSharedIndexBuffer indexBuffer = std::make_shared<CIndexBuffer>(k_MAX_NUM_INDICES, GL_STREAM_DRAW);
    ui16* indexData = indexBuffer->lock();
    memset(indexData, 0x0, k_MAX_NUM_INDICES * sizeof(ui16));
    indexBuffer->unlock();
    
    m_mesh = CMesh::constructCustomMesh(m_guid, vertexBuffer, indexBuffer,
                                     glm::vec3(4096.0), glm::vec3(-4096));
    assert(m_mesh != nullptr);
}

CBatch::~CBatch(void)
{
    
}

std::string CBatch::getGuid(void) const
{
    return m_material->getShader()->getGuid();
}

std::string CBatch::getMode(void) const
{
    return m_mode;
}

ui32 CBatch::getNumUnlockedNumVertices(void) const
{
    return m_numUnlockedVertices;
}

ui32 CBatch::getNumUnlockedNumIndices(void) const
{
    return m_numUnlockedIndices;
}

ui32 CBatch::getNumUnlockedNumTransformations(void) const
{
    return m_numUnlockedTransformations;
}

void CBatch::lock(void)
{
    m_models.clear();
    m_matrices.clear();
    m_transformations.clear();
    
    m_numUnlockedVertices = 0;
    m_numUnlockedIndices = 0;
    m_numUnlockedTransformations = 0;
}

void CBatch::unlock(void)
{
    m_numLockedVertices = 0;
    m_numLockedIndices = 0;
    m_numLockedTransformations = 0;
    
    for(ui32 i = 0; i < m_models.size(); ++i)
    {
        CSharedMesh mesh = std::get<0>(m_models[i]);
        CSharedAnimationMixer mixer = std::get<1>(m_models[i]);
        glm::mat4x4& matrix = m_matrices[i];
        
        ui16* indexData_01 = m_mesh->getIndexBuffer()->lock();
        ui16* indexData_02 = mesh->getIndexBuffer()->lock();
        
        for(ui32 j = 0; j < mesh->getNumIndices(); ++j)
        {
            indexData_01[m_numLockedIndices + j] = indexData_02[j] + m_numLockedVertices;
        }
        
        SAttributeVertex* vertexData_01 = m_mesh->getVertexBuffer()->lock();
        SAttributeVertex* vertexData_02 = mesh->getVertexBuffer()->lock();
        for(ui32 j = 0; j < mesh->getNumVertices(); ++j)
        {
            glm::vec3 position = glm::vec3(0.0f);
            glm::vec3 normal = glm::vec3(0.0f);
            glm::vec3 tangent = glm::vec3(0.0f);
            
            for(ui32 k = 0; k < 4; ++k)
            {
                f32 weight = static_cast<f32>(vertexData_02[j].m_extra[k]) / 255.0f;
                position += glm::transform(vertexData_02[j].m_position, m_transformations[vertexData_02[j].m_color[k] + m_numLockedTransformations]) * weight;
                normal += glm::transform(CVertexBuffer::uncompressU8Vec4(vertexData_02[j].m_normal), m_transformations[vertexData_02[j].m_color[k] + m_numLockedTransformations]) * weight;
                tangent += glm::transform(CVertexBuffer::uncompressU8Vec4(vertexData_02[j].m_tangent), m_transformations[vertexData_02[j].m_color[k] + m_numLockedTransformations]) * weight;
            }
            
            vertexData_01[m_numLockedVertices + j] = vertexData_02[j];
            vertexData_01[m_numLockedVertices + j].m_position = glm::transform(position, matrix);
            vertexData_01[m_numLockedVertices + j].m_normal = CVertexBuffer::compressVec3(glm::transform(normal, matrix));
            vertexData_01[m_numLockedVertices + j].m_tangent = CVertexBuffer::compressVec3(glm::transform(tangent, matrix));
        }
        
        m_numLockedVertices += mesh->getNumVertices();
        m_numLockedIndices += mesh->getNumIndices();
        //m_numLockedTransformations += mixer->Get_TransformationSize();
        assert(m_numLockedTransformations < k_MAX_NUM_TRANSFORMATION);
    }
}

void CBatch::batch(const std::tuple<CSharedMesh, CSharedAnimationMixer>& model,
                   const glm::mat4x4 &matrix)
{
    m_models.push_back(model);
    m_matrices.push_back(matrix);
    
    m_numUnlockedVertices += std::get<0>(model)->getNumVertices();
    m_numUnlockedIndices += std::get<0>(model)->getNumIndices();
    //m_numUnlockedTransformations += std::get<1>(model)->Get_TransformationSize();
    
    std::shared_ptr<CAnimationMixer> mixer = std::get<1>(model);
    /*for(ui32 i = 0; i < mixer->Get_TransformationSize(); ++i)
     {
     m_transformations.push_back(mixer->Get_Transformations()[i]);
     }*/
}

i32 CBatch::zOrder(void)
{
    return m_zOrder;
}

bool CBatch::checkOcclusion(void)
{
    return false;
}

ui32 CBatch::numTriangles(void)
{
    return 0;
}

void CBatch::onBind(const std::string& mode)
{
    
}

void CBatch::onDraw(const std::string& mode)
{
    assert(m_mesh != nullptr);
    assert(m_material != nullptr);
    assert(m_material->getShader() != nullptr);
    
    m_mesh->getVertexBuffer()->unlock(m_numLockedVertices);
    m_mesh->getIndexBuffer()->unlock(m_numLockedIndices);

    if(m_numLockedIndices != 0 &&
       m_numLockedVertices != 0)
    {
        m_material->bind();
		m_materialBindImposer(m_material);
        
        m_mesh->bind(m_material->getShader()->getAttributesRef());
        m_mesh->draw(m_numLockedIndices);
        m_mesh->unbind(m_material->getShader()->getAttributesRef());
        
        m_material->unbind();
    }
}

void CBatch::onUnbind(const std::string& mode)
{
    
}
void CBatch::onBatch(const std::string& mode)
{
    
}
