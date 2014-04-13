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

CMeshData::CMeshData(const std::vector<SVertexData>& vertexData,
                     const std::vector<ui16>& indexData,
                     const glm::vec3& maxBound,
                     const glm::vec3& minBound) :
IResourceData(E_RESOURCE_DATA_CLASS_MESH_DATA),
m_vertexData(vertexData),
m_indexData(indexData),
m_maxBound(maxBound),
m_minBound(minBound)
{
    
}

CMeshData::~CMeshData(void)
{
    m_vertexData.clear();
    m_indexData.clear();
}

const std::vector<SVertexData>& CMeshData::getVertexData(void) const
{
    return m_vertexData;
}

const std::vector<ui16>& CMeshData::getIndexData(void) const
{
    return m_indexData;
}

const ui32 CMeshData::getNumVertices(void) const
{
    return m_vertexData.size();
}

const ui32 CMeshData::getNumIndices(void) const
{
    return m_indexData.size();
}

const glm::vec3& CMeshData::getMaxBound(void) const
{
    return m_maxBound;
}

const glm::vec3& CMeshData::getMinBound(void) const
{
    return m_minBound;
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

CMesh::~CMesh(void)
{
    
}

void CMesh::onResourceDataSerialized(ISharedResourceDataRef resourceData,
                                     E_RESOURCE_DATA_STATUS status)
{
    if(status == E_RESOURCE_DATA_STATUS_STARTED)
    {
        
    } else if(status == E_RESOURCE_DATA_STATUS_PROGRESS) {
        assert(resourceData != nullptr);
        
        switch(resourceData->getResourceDataClass())
        {
            case E_RESOURCE_DATA_CLASS_MESH_DATA:
            {
                m_meshData = std::static_pointer_cast<CMeshData>(resourceData);
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
        
    } else if(status == E_RESOURCE_DATA_STATUS_FINISHED) {
        m_status |= E_RESOURCE_STATUS_LOADED;
    }
}

void CMesh::onResourceDataCommited(ISharedResourceDataRef resourceData,
                                   E_RESOURCE_DATA_STATUS status)
{
    if(status == E_RESOURCE_DATA_STATUS_STARTED)
    {
        
    } else if(status == E_RESOURCE_DATA_STATUS_PROGRESS) {
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
        
    } else if(status == E_RESOURCE_DATA_STATUS_FINISHED) {
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

const std::vector<SVertexData>& CMesh::getVertexData(void) const
{
    static std::vector<SVertexData> stub;
    return IResource::isLoaded() ? m_meshData->getVertexData() : stub;
}

const std::vector<ui16>& CMesh::getIndexData(void) const
{
    static std::vector<ui16> stub;
    return IResource::isLoaded() ? m_meshData->getIndexData() : stub;
}

const ui32 CMesh::getNumVertices(void) const
{
    return IResource::isLoaded() ? m_meshData->getNumVertices() : 0;
}

const ui32 CMesh::getNumIndices(void) const
{
    return IResource::isLoaded() ? m_meshData->getNumIndices() : 0;
}

const glm::vec3 CMesh::getMaxBound(void) const
{
    return IResource::isLoaded() ? m_meshData->getMaxBound() : glm::vec3(0.0, 0.0, 0.0);
}

const glm::vec3 CMesh::getMinBound(void) const
{
    return IResource::isLoaded() ? m_meshData->getMaxBound() : glm::vec3(0.0, 0.0, 0.0);
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
