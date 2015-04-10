//
//  CMesh.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/26/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CMesh.h"
#include "CVertexBuffer.h"
#include "CIndexBuffer.h"
#include "CBone.h"
#include "CAnimationSequence.h"
#include "CVertexArrayBuffer.h"

const ui32 kMaxBones = 32;

CMeshMetadata::CMeshMetadata(SVertexData* vertices, ui16* indices,
                             ui32 numVertices, ui32 numIndices,
                             const glm::vec3& minBound, const glm::vec3& maxBound) :
IResourceData(E_RESOURCE_DATA_CLASS_MESH_DATA),
m_vertices(vertices),
m_indices(indices),
m_numVertices(numVertices),
m_numIndices(numIndices),
m_minBound(minBound),
m_maxBound(maxBound)
{
    
}

CMeshMetadata::~CMeshMetadata(void)
{
    delete[] m_vertices;
    delete[] m_indices;
}

const SVertexData* CMeshMetadata::getVertices(void) const
{
    return m_vertices;
}

const ui16* CMeshMetadata::getIndices(void) const
{
    return m_indices;
}

ui32 CMeshMetadata::getNumVertices(void) const
{
    return m_numVertices;
}

ui32 CMeshMetadata::getNumIndices(void) const
{
    return m_numIndices;
}

const glm::vec3 CMeshMetadata::getMinBound(void) const
{
    return m_minBound;
}

const glm::vec3 CMeshMetadata::getMaxBound(void) const
{
    return m_maxBound;
}

CSkeletonData::CSkeletonData(ui32 numBones) :
IResourceData(E_RESOURCE_DATA_CLASS_SKELETON_DATA),
m_numBones(numBones)
{
    
}

CSkeletonData::~CSkeletonData(void)
{
    m_bonesRawData.clear();
}

void CSkeletonData::addBone(i32 id, i32 parentId)
{
    m_bonesRawData.push_back(std::make_shared<CBoneData>(id, parentId));
}

ui32 CSkeletonData::getNumBones(void) const
{
    return m_numBones;
}

const std::vector<CSharedBoneData> CSkeletonData::getBonesRawData(void) const
{
    return m_bonesRawData;
}

CMesh::CMesh(const std::string& guid) : IResource(E_RESOURCE_CLASS_MESH, guid),
m_vbo(nullptr),
m_ibo(nullptr),
m_meshMetadata(nullptr),
m_skeletonData(nullptr)
{

}

CSharedMesh CMesh::construct(const std::string& guid, CSharedVertexBufferRef vbo, CSharedIndexBufferRef ibo,
                             const glm::vec3& minBound,
                             const glm::vec3& maxBound)
{
    assert(vbo != nullptr);
    assert(ibo != nullptr);
    
    CSharedMesh mesh = CMesh::construct(guid, vbo, ibo);
    mesh->m_minBound = minBound;
    mesh->m_maxBound = maxBound;
    return mesh;
}

CSharedMesh CMesh::construct(const std::string &guid, CSharedVertexBufferRef vbo, CSharedIndexBufferRef ibo)
{
    assert(vbo != nullptr);
    assert(ibo != nullptr);
    
    CSharedMesh mesh = std::make_shared<CMesh>(guid);
    mesh->m_vbo = vbo;
    mesh->m_ibo = ibo;
    
    SAttributeVertex *vertices = vbo->lock();
    ui16* indices = ibo->lock();
    ui32 indicesCount = ibo->getUsedSize();
    
    glm::vec3 maxBound = glm::vec3(INT16_MIN);
    glm::vec3 minBound = glm::vec3(INT16_MAX);
    
    for(ui32 i = 0; i < indicesCount; ++i)
    {
        glm::vec3 point = vertices[indices[i]].m_position;
        maxBound = glm::max(point, maxBound);
        minBound = glm::min(point, minBound);
    }
    mesh->m_minBound = std::move(minBound);
    mesh->m_maxBound = std::move(maxBound);

    mesh->m_status |= E_RESOURCE_STATUS_LOADED;
    mesh->m_status |= E_RESOURCE_STATUS_COMMITED;
    return mesh;
}

CMesh::~CMesh(void)
{

}

void CMesh::onResourceDataSerializationFinished(ISharedResourceDataRef resourceData)
{
    assert(resourceData != nullptr);
    switch(resourceData->getResourceDataClass())
    {
        case E_RESOURCE_DATA_CLASS_MESH_DATA:
        {
            m_meshMetadata = std::static_pointer_cast<CMeshMetadata>(resourceData);
            m_minBound = m_meshMetadata->getMinBound();
            m_maxBound = m_meshMetadata->getMaxBound();
            
            m_status |= E_RESOURCE_STATUS_LOADED;
        }
            break;
            
        case E_RESOURCE_DATA_CLASS_SKELETON_DATA:
        {
            m_skeletonData = std::static_pointer_cast<CSkeletonData>(resourceData);
        }
            break;
            
        case E_RESOURCE_DATA_CLASS_SEQUENCE_DATA:
        {
            m_bindposeData = std::static_pointer_cast<CSequenceData>(resourceData);
        }
            break;
            
        default:
        {
            assert(false);
        }
            break;
    }
}

void CMesh::onResourceDataCommitFinished(ISharedResourceDataRef resourceData)
{
    assert(resourceData != nullptr);
    switch(resourceData->getResourceDataClass())
    {
        case E_RESOURCE_DATA_CLASS_VERTEX_BUFFER_DATA:
        {
            m_vbo = std::static_pointer_cast<CVertexBuffer>(resourceData);
        }
            break;
            
        case E_RESOURCE_DATA_CLASS_INDEX_BUFFER_DATA:
        {
            m_ibo = std::static_pointer_cast<CIndexBuffer>(resourceData);
        }
            break;
            
        default:
        {
            assert(false);
        }
            break;
    }
    if(m_vbo != nullptr &&
       m_ibo != nullptr)
    {
        m_meshMetadata = nullptr;
        m_status |= E_RESOURCE_STATUS_COMMITED;
    }
}

CSharedVertexBuffer CMesh::getVertexBuffer(void) const
{
    return IResource::isCommited() ? m_vbo : nullptr;
}

CSharedIndexBuffer CMesh::getIndexBuffer(void) const
{
    return IResource::isCommited() ? m_ibo : nullptr;
}

const SVertexData* CMesh::getRawVertices(void) const
{
    return IResource::isLoaded() ? m_meshMetadata->getVertices() : nullptr;
}

const ui16* CMesh::getRawIndices(void) const
{
    return IResource::isLoaded() ? m_meshMetadata->getIndices() : nullptr;
}

ui32 CMesh::getNumRawVertices(void) const
{
    return IResource::isLoaded() ? m_meshMetadata->getNumVertices() : 0;
}

ui32 CMesh::getNumRawIndices(void) const
{
    return IResource::isLoaded() ? m_meshMetadata->getNumIndices() : 0;
}

const glm::vec3 CMesh::getMinBound(void) const
{
    return IResource::isLoaded() ? m_minBound : std::move(glm::vec3(0.0f));
}

const glm::vec3 CMesh::getMaxBound(void) const
{
    return IResource::isLoaded() ? m_maxBound : std::move(glm::vec3(0.0f));
}

const std::tuple<glm::vec3, glm::vec3> CMesh::getBounds(void) const
{
    if(!IResource::isLoaded())
    {
        return std::make_tuple(glm::vec3(0.0), glm::vec3(0.0));
    }
    return std::make_tuple(m_minBound, m_maxBound);
}

const glm::vec3 CMesh::getMinBound(const glm::mat4& matrix) const
{
    if(!IResource::isLoaded())
    {
        return glm::vec3(0.0f);
    }
    return std::get<0>(CMesh::getBounds(matrix));
}

const glm::vec3 CMesh::getMaxBound(const glm::mat4& matrix) const
{
    if(!IResource::isLoaded())
    {
        return glm::vec3(0.0f);
    }
    return std::get<0>(CMesh::getBounds(matrix));
}

const std::tuple<glm::vec3, glm::vec3> CMesh::getBounds(const glm::mat4& matrix) const
{
    if(!IResource::isLoaded())
    {
        return std::make_tuple(glm::vec3(0.0), glm::vec3(0.0));
    }
    
    SAttributeVertex *vertices = m_vbo->lock();
    ui16* indices = m_ibo->lock();
    ui32 indicesCount = m_ibo->getUsedSize();
    
    glm::vec3 maxBound = glm::vec3(INT16_MIN);
    glm::vec3 minBound = glm::vec3(INT16_MAX);
    
    for(ui32 i = 0; i < indicesCount; ++i)
    {
        glm::vec3 point = vertices[indices[i]].m_position;
        glm::vec4 tranformedVertex = matrix * glm::vec4(point, 1.0f);
        
        maxBound = glm::max(glm::vec3(tranformedVertex.x, tranformedVertex.y, tranformedVertex.z), maxBound);
        minBound = glm::min(glm::vec3(tranformedVertex.x, tranformedVertex.y, tranformedVertex.z), minBound);
    }
    return std::make_tuple(minBound, maxBound);
}

const CSharedSkeletonData CMesh::getSkeletonData(void) const
{
    return IResource::isLoaded() ? m_skeletonData : nullptr;
}

const CSharedSequenceData CMesh::getBindposeData(void) const
{
    return IResource::isLoaded() ? m_bindposeData : nullptr;
}

void CMesh::bind(const std::string& attributesGUID, const std::array<i32, E_SHADER_ATTRIBUTE_MAX>& attributes)
{
    if(IResource::isLoaded() && IResource::isCommited())
    {
        assert(attributesGUID.length() != 0);
        CSharedVertexArrayBuffer vaoState = m_VAOstates[attributesGUID];
        if(!vaoState)
        {
            vaoState = std::make_shared<CVertexArrayBuffer>(m_vbo,
                                                            m_ibo);
            vaoState->init(attributes);
            m_VAOstates[attributesGUID] = vaoState;
        }
        CVertexArrayBuffer::bind(vaoState);
    }
}

void CMesh::draw(void) const
{
    if(IResource::isLoaded() && IResource::isCommited())
    {
        ieDrawElements(GL_TRIANGLES, m_ibo->getUsedSize(), GL_UNSIGNED_SHORT, NULL);
    }
}

void CMesh::draw(ui32 indices) const
{
    if(IResource::isLoaded() && IResource::isCommited())
    {
        ieDrawElements(GL_TRIANGLES, indices, GL_UNSIGNED_SHORT, NULL);
    }
}

void CMesh::unbind(const std::string& attributesGUID, const std::array<i32, E_SHADER_ATTRIBUTE_MAX>& attributes)
{
    if(IResource::isLoaded() && IResource::isCommited())
    {
        CVertexArrayBuffer::unbind();
    }
}
