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
    
    m_batchMesh = CMesh::constructCustomMesh(m_guid, vertexBuffer, indexBuffer,
                                             glm::vec3(4096.0), glm::vec3(-4096));
    assert(m_batchMesh != nullptr);
}

CBatch::~CBatch(void)
{
    
}

std::string CBatch::getGuid(void) const
{
    return m_material->getShader()->getGuid();
}

std::string CBatch::getTechniqueName(void) const
{
    return m_mode;
}

ui32 CBatch::getNumUnlockedVertices(void) const
{
    return m_numUnlockedVertices;
}

ui32 CBatch::getNumUnlockedIndices(void) const
{
    return m_numUnlockedIndices;
}

ui32 CBatch::getNumUnlockedTransformations(void) const
{
    return m_numUnlockedTransformations;
}

bool CBatch::isAnimated(void) const
{
    return m_animationMixers.size() != 0;
}

void CBatch::lock(void)
{
    m_meshes.clear();
    m_animationMixers.clear();
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
    
    if(CBatch::isAnimated())
    {
        for(ui32 i = 0; i < m_meshes.size(); ++i)
        {
            CSharedMesh mesh = m_meshes.at(i);
            CSharedAnimationMixer animationMixer = m_animationMixers.at(i);
            glm::mat4x4& matrix = m_matrices.at(i);
            
            ui16* indexData_01 = m_batchMesh->getIndexBuffer()->lock();
            ui16* indexData_02 = mesh->getIndexBuffer()->lock();
            
            for(ui32 j = 0; j < mesh->getNumIndices(); ++j)
            {
                indexData_01[m_numLockedIndices + j] = indexData_02[j] + m_numLockedVertices;
            }
            
            SAttributeVertex* vertexData_01 = m_batchMesh->getVertexBuffer()->lock();
            SAttributeVertex* vertexData_02 = mesh->getVertexBuffer()->lock();
            
            glm::vec4 originalNormal = glm::vec4(0.0);
            glm::vec4 originalTangent = glm::vec4(0.0);
            
            for(ui32 j = 0; j < mesh->getNumVertices(); ++j)
            {
                glm::vec3 position = glm::vec3(0.0);
                glm::vec4 normal = glm::vec4(0.0);
                glm::vec4 tangent = glm::vec4(0.0);
                
                originalNormal = glm::unpackSnorm4x8(vertexData_02[j].m_normal);
                originalTangent = glm::unpackSnorm4x8(vertexData_02[j].m_tangent);
                
                for(ui32 k = 0; k < 4; ++k)
                {
                    f32 weight = static_cast<f32>(vertexData_02[j].m_extra[k]) / 255.0f;
                    position += glm::transform(vertexData_02[j].m_position, m_transformations[vertexData_02[j].m_color[k] + m_numLockedTransformations]) * weight;
                    normal += glm::transform(originalNormal, m_transformations[vertexData_02[j].m_color[k] + m_numLockedTransformations]) * weight;
                    tangent += glm::transform(originalTangent, m_transformations[vertexData_02[j].m_color[k] + m_numLockedTransformations]) * weight;
                }
                
                vertexData_01[m_numLockedVertices + j] = vertexData_02[j];
                vertexData_01[m_numLockedVertices + j].m_position = glm::transform(position, matrix);
                vertexData_01[m_numLockedVertices + j].m_normal = glm::packSnorm4x8(glm::transform(normal, matrix));
                vertexData_01[m_numLockedVertices + j].m_tangent = glm::packSnorm4x8(glm::transform(tangent, matrix));
            }
            
            m_numLockedVertices += mesh->getNumVertices();
            m_numLockedIndices += mesh->getNumIndices();
            m_numLockedTransformations += animationMixer->getTransformationSize();
            assert(m_numLockedTransformations < k_MAX_NUM_TRANSFORMATION);
        }
    }
    else
    {
        for(ui32 i = 0; i < m_meshes.size(); ++i)
        {
            CSharedMesh mesh = m_meshes.at(i);
            glm::mat4x4& matrix = m_matrices.at(i);
            
            ui16* indexData_01 = m_batchMesh->getIndexBuffer()->lock();
            ui16* indexData_02 = mesh->getIndexBuffer()->lock();
            
            for(ui32 j = 0; j < mesh->getNumIndices(); ++j)
            {
                indexData_01[m_numLockedIndices + j] = indexData_02[j] + m_numLockedVertices;
            }
            
            SAttributeVertex* vertexData_01 = m_batchMesh->getVertexBuffer()->lock();
            SAttributeVertex* vertexData_02 = mesh->getVertexBuffer()->lock();
            
            glm::vec3 position = glm::vec3(0.0);
            glm::vec4 normal = glm::vec4(0.0);
            glm::vec4 tangent = glm::vec4(0.0);
            
            for(ui32 j = 0; j < mesh->getNumVertices(); ++j)
            {
                glm::vec3 position = vertexData_02[j].m_position;
                glm::vec4 normal = glm::unpackSnorm4x8(vertexData_02[j].m_normal);
                glm::vec4 tangent = glm::unpackSnorm4x8(vertexData_02[j].m_tangent);
                
                vertexData_01[m_numLockedVertices + j] = vertexData_02[j];
                vertexData_01[m_numLockedVertices + j].m_position = glm::transform(position, matrix);
                vertexData_01[m_numLockedVertices + j].m_normal = glm::packSnorm4x8(glm::transform(normal, matrix));
                vertexData_01[m_numLockedVertices + j].m_tangent = glm::packSnorm4x8(glm::transform(tangent, matrix));
            }
            
            m_numLockedVertices += mesh->getNumVertices();
            m_numLockedIndices += mesh->getNumIndices();
        }
    }
}

void CBatch::batch(CSharedMeshRef mesh,
                   const glm::mat4x4& matrix)
{
    m_meshes.push_back(mesh);
    m_matrices.push_back(matrix);
    
    m_numUnlockedVertices += mesh->getNumVertices();
    m_numUnlockedIndices += mesh->getNumIndices();
}

void CBatch::batch(CSharedMeshRef mesh,
                   CSharedAnimationMixerRef animationMixer,
                   const glm::mat4x4& matrix)
{
    CBatch::batch(mesh, matrix);
    for(ui32 i = 0; i < animationMixer->getTransformationSize(); ++i)
    {
        m_transformations.push_back(animationMixer->getTransformations()[i]);
    }
    m_numUnlockedTransformations += animationMixer->getTransformationSize();
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

void CBatch::onDrawBoundingBox(void)
{
    
}

void CBatch::onBind(const std::string& techniqueName)
{
    
}

void CBatch::onDraw(const std::string& techniqueName)
{
    assert(m_batchMesh != nullptr);
    assert(m_material != nullptr);
    assert(m_material->getShader() != nullptr);
    
    m_batchMesh->getVertexBuffer()->unlock(m_numLockedVertices);
    m_batchMesh->getIndexBuffer()->unlock(m_numLockedIndices);

    if(m_numLockedIndices != 0 &&
       m_numLockedVertices != 0)
    {
        m_material->bind();
		m_materialBindImposer(m_material);
        
        m_batchMesh->bind(m_material->getShader()->getAttributes());
        m_batchMesh->draw(m_numLockedIndices);
        m_batchMesh->unbind(m_material->getShader()->getAttributes());
        
        m_material->unbind();
    }
}

void CBatch::onUnbind(const std::string& techniqueName)
{
    
}
void CBatch::onBatch(const std::string& techniqueName)
{
    
}
