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

CMeshData::CMeshData(SVertexData* vertexData,
                     ui16* indexData,
                     ui32 numVertices,
                     ui32 numIndices,
                     const glm::vec3& maxBound,
                     const glm::vec3& minBound) :
IResourceData(E_RESOURCE_DATA_CLASS_MESH_DATA),
m_vertexData(vertexData),
m_indexData(indexData),
m_numVertices(numVertices),
m_numIndices(numIndices),
m_maxBound(maxBound),
m_minBound(minBound)
{
    
}

CMeshData::~CMeshData(void)
{
    std::cout<<"[CMeshData::~CMeshData]"<<std::endl;
    delete[] m_vertexData;
    delete[] m_indexData;
}

const SVertexData* CMeshData::getVertexData(void) const
{
    return m_vertexData;
}

const ui16* CMeshData::getIndexData(void) const
{
    return m_indexData;
}

ui32 CMeshData::getNumVertices(void) const
{
    return m_numVertices;
}

ui32 CMeshData::getNumIndices(void) const
{
    return m_numIndices;
}

glm::vec3 CMeshData::calculateMaxBound(const glm::vec3& point_01, const glm::vec3& point_02)
{
    glm::vec3 result = point_02;
    if(point_01.x > point_02.x)
    {
        result.x = point_01.x;
    }
    if(point_01.y > point_02.y)
    {
        result.y = point_01.y;
    }
    if(point_01.z > point_02.z)
    {
        result.z = point_01.z;
    }
    return result;
}

glm::vec3 CMeshData::calculateMinBound(const glm::vec3& point_01, const glm::vec3& point_02)
{
    glm::vec3 result = point_02;
    if(point_01.x < point_02.x)
    {
        result.x = point_01.x;
    }
    if(point_01.y < point_02.y)
    {
        result.y = point_01.y;
    }
    if(point_01.z < point_02.z)
    {
        result.z = point_01.z;
    }
    return result;
}

const glm::vec3& CMeshData::getMaxBound(void) const
{
    return m_maxBound;
}

const glm::vec3& CMeshData::getMinBound(void) const
{
    return m_minBound;
}

void CMeshData::updateBounds(SAttributeVertex *data, ui32 numVertices)
{
    m_maxBound = glm::vec3( -4096.0f, -4096.0f, -4096.0f );
    m_minBound = glm::vec3( 4096.0f, 4096.0f, 4096.0f );
    assert(data != nullptr);
    for(ui32 i = 0; i < numVertices; ++i)
    {
        glm::vec3 point = data[i].m_position;
        m_maxBound = CMeshData::calculateMaxBound(point, m_maxBound);
        m_minBound = CMeshData::calculateMinBound(point, m_minBound);
    }
}

void CMeshData::removeData(void)
{
    delete[] m_vertexData;
    delete[] m_indexData;
    
    m_vertexData = nullptr;
    m_indexData = nullptr;
    
    m_numVertices = 0;
    m_numIndices = 0;
}

CFrameData::CFrameData(const std::vector<glm::quat>& rotations,
                       const std::vector<glm::vec3>& positions,
                       const std::vector<glm::vec3>& scales) :
m_rotations(rotations),
m_positions(positions),
m_scales(scales)
{
    
}

CFrameData::~CFrameData(void)
{
    m_rotations.clear();
    m_positions.clear();
    m_scales.clear();
}

const glm::quat& CFrameData::getRotation(ui32 index) const
{
    assert(m_rotations.size() > index);
    return m_rotations.at(index);
}

const glm::vec3& CFrameData::getPosition(ui32 index) const
{
    assert(m_positions.size() > index);
    return m_positions.at(index);
}

const glm::vec3& CFrameData::getScale(ui32 index) const
{
    assert(m_scales.size() > index);
    return m_scales.at(index);
}

CSequenceData::CSequenceData(const std::string& animationName,
                             ui32 animationFPS,
                             const std::vector<CSharedFrameData>& frames) :
IResourceData(E_RESOURCE_DATA_CLASS_SEQUENCE_DATA),
m_animationName(animationName),
m_animationFPS(animationFPS),
m_frames(frames)
{
    
}

CSequenceData::~CSequenceData(void)
{
    m_frames.clear();
}

const ui32 CSequenceData::getNumFrames(void) const
{
    return m_frames.size();
}

const ui32 CSequenceData::getAnimationFPS(void) const
{
    return m_animationFPS;
}

const std::string CSequenceData::getAnimationName(void) const
{
    return m_animationName;
}

CSharedFrameData CSequenceData::getFrame(ui32 index) const
{
    assert(m_frames.size() > index);
    return m_frames.at(index);
}

CSkeletonData::CSkeletonData(ui32 numBones) :
IResourceData(E_RESOURCE_DATA_CLASS_SKELETON_DATA),
m_numBones(numBones)
{
    
}

CSkeletonData::~CSkeletonData(void)
{
    // TODO#: remove all bones
}

void CSkeletonData::addBone(CSharedBoneRef bone)
{
    if(bone == nullptr)
    {
        return;
    }
    
    if (bone->getParentId() == -1)
    {
        m_roots.insert(bone);
        return;
    }
    
    CSharedBone parent = CSkeletonData::getBone(bone->getParentId());
    if (parent != nullptr)
    {
        parent->addChild(bone);
        return;
    }
    assert(false);
}

CSharedBone CSkeletonData::getBone(ui32 index) const
{
    for(const auto& root : m_roots)
    {
        if (root->getId() == index)
        {
            return root;
        }
        else
        {
            CSharedBone child = root->findChild(index);
            if(child != nullptr)
            {
                return child;
            }
        }
    }
    return nullptr;
}

ui32 CSkeletonData::getNumBones(void) const
{
    return m_numBones;
}

CMesh::CMesh(const std::string& guid) : IResource(E_RESOURCE_CLASS_MESH, guid),
m_vertexBuffer(nullptr),
m_indexBuffer(nullptr),
m_meshData(nullptr),
m_skeletonData(nullptr),
m_sequenceData(nullptr)
{
    
}

CSharedMesh CMesh::constructCustomMesh(const std::string& guid,
                                    CSharedVertexBufferRef vertexBuffer,
                                    CSharedIndexBufferRef indexBuffer,
                                    const glm::vec3& maxBound,
                                    const glm::vec3& minBound)
{
    assert(vertexBuffer != nullptr);
    assert(indexBuffer != nullptr);
    
    CSharedMesh mesh = std::make_shared<CMesh>(guid);
    mesh->m_vertexBuffer = vertexBuffer;
    mesh->m_indexBuffer = indexBuffer;
    
    mesh->m_meshData = std::make_shared<CMeshData>(nullptr, nullptr, 0, 0,
                                                   maxBound,
                                                   minBound);
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
            m_meshData = std::static_pointer_cast<CMeshData>(resourceData);
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
            m_sequenceData = std::static_pointer_cast<CSequenceData>(resourceData);
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
            m_vertexBuffer = std::static_pointer_cast<CVertexBuffer>(resourceData);
        }
            break;
            
        case E_RESOURCE_DATA_CLASS_SKELETON_DATA:
        {
            m_indexBuffer = std::static_pointer_cast<CIndexBuffer>(resourceData);
        }
            break;
            
        default:
        {
            assert(false);
        }
            break;
    }
    if(m_vertexBuffer != nullptr && m_indexBuffer != nullptr)
    {
        m_status |= E_RESOURCE_STATUS_COMMITED;
    }
}

CSharedVertexBuffer CMesh::getVertexBuffer(void) const
{
    return IResource::isCommited() ? m_vertexBuffer : nullptr;
}

CSharedIndexBuffer CMesh::getIndexBuffer(void) const
{
    return IResource::isCommited() ? m_indexBuffer : nullptr;
}

const SVertexData* CMesh::getVertexData(void) const
{
    return IResource::isLoaded() ? m_meshData->getVertexData() : nullptr;
}

const ui16* CMesh::getIndexData(void) const
{
    return IResource::isLoaded() ? m_meshData->getIndexData() : nullptr;
}

const ui32 CMesh::getNumVertices(void) const
{
    return IResource::isCommited() ? m_vertexBuffer->getSize() : 0;
}

const ui32 CMesh::getNumIndices(void) const
{
    return IResource::isCommited() ? m_indexBuffer->getSize() : 0;
}

const glm::vec3 CMesh::getMaxBound(void) const
{
    return IResource::isLoaded() ? m_meshData->getMaxBound() : glm::vec3(0.0, 0.0, 0.0);
}

const glm::vec3 CMesh::getMinBound(void) const
{
    return IResource::isLoaded() ? m_meshData->getMinBound() : glm::vec3(0.0, 0.0, 0.0);
}

void CMesh::updateBounds(void)
{
    if(IResource::isLoaded())
    {
        assert(m_meshData != nullptr);
        assert(m_vertexBuffer != nullptr);
        m_meshData->updateBounds(m_vertexBuffer->lock(), m_vertexBuffer->getSize());
    }
}

const ui32 CMesh::getNumFrames(void) const
{
    return IResource::isLoaded() && m_sequenceData != nullptr ? m_sequenceData->getNumFrames() : 0;
}

const ui32 CMesh::getAnimationFPS(void) const
{
    return IResource::isLoaded() && m_sequenceData != nullptr ? m_sequenceData->getAnimationFPS() : 0;
}

const std::string CMesh::getAnimationName(void) const
{
    return IResource::isLoaded() && m_sequenceData != nullptr ? m_sequenceData->getAnimationName() : 0;
}

CSharedFrameData CMesh::getFrame(ui32 index) const
{
    return IResource::isLoaded() && m_sequenceData != nullptr ? m_sequenceData->getFrame(index) : nullptr;
}

CSharedBone CMesh::getBone(ui32 index) const
{
    return IResource::isLoaded() && m_skeletonData != nullptr ? m_skeletonData->getBone(index) : nullptr;
}

ui32 CMesh::getNumBones(void) const
{
    return IResource::isLoaded() && m_skeletonData != nullptr ? m_skeletonData->getNumBones() : 0;
}

void CMesh::bind(const i32* attributes) const
{
    if(IResource::isLoaded() && IResource::isCommited())
    {
        assert(m_vertexBuffer != nullptr);
        assert(m_indexBuffer != nullptr);
        m_vertexBuffer->bind(attributes);
        m_indexBuffer->bind();
    }
}

void CMesh::draw(void) const
{
    if(IResource::isLoaded() && IResource::isCommited())
    {
        assert(m_vertexBuffer != nullptr);
        assert(m_indexBuffer != nullptr);
        glDrawElements(GL_TRIANGLES, m_indexBuffer->getSize(), GL_UNSIGNED_SHORT, NULL);
    }
}

void CMesh::draw(ui32 indices) const
{
    if(IResource::isLoaded() && IResource::isCommited())
    {
        assert(m_vertexBuffer != nullptr);
        assert(m_indexBuffer != nullptr);
        glDrawElements(GL_TRIANGLES, indices, GL_UNSIGNED_SHORT, NULL);
    }
}

void CMesh::unbind(const i32* attributes) const
{
    if(IResource::isLoaded() && IResource::isCommited())
    {
        assert(m_vertexBuffer != nullptr);
        assert(m_indexBuffer != nullptr);
        m_indexBuffer->unbind();
        m_vertexBuffer->unbind(attributes);
    }
}