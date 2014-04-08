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

CMeshData::CMeshData(const std::vector<SVertex>& vertexData,
                     const std::vector<ui16>& indexData,
                     const glm::vec3& maxBound,
                     const glm::vec3& minBound) :
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

const std::vector<SVertex>& CMeshData::getVertexData(void) const
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

void CMesh::init(CSharedMeshDataRef meshData,
                 CSharedSkeletonDataRef skeletonData,
                 CSharedSequenceDataRef sequenceData)
{
    m_meshData = meshData;
    m_skeletonData = skeletonData;
    m_sequenceData = sequenceData;
    
    m_status |= E_RESOURCE_STATUS_LOADED;
    m_status |= E_RESOURCE_STATUS_COMMITED;
}

CSharedVertexBuffer CMesh::getVertexBuffer(void) const;
CSharedIndexBuffer CMesh::getIndexBuffer(void) const;

const ui32 getNumVertices(void) const;
const ui32 getNumIndices(void) const;

const glm::vec3 getMaxBound(void) const;
const glm::vec3 getMinBound(void) const;

const ui32 getNumFrames(void) const;
const ui32 getAnimationFPS(void) const;
const std::string getAnimationName(void) const;
CSharedFrameData getFrame(ui32 index) const;

CSharedBone getBone(ui32 index) const;
ui32 getNumBones(void) const;

void bind(const i32* attributes) const;
void draw(void) const;
void draw(ui32 indices) const;
void unbind(const i32* attributes) const;

class CMesh : public IResource
{
private:
    
protected:
    
    CSharedMeshData m_meshData;
    CSharedSkeletonData m_skeletonData;
    CSharedSequenceData m_sequenceData;
    
    CSharedVertexBuffer m_vertexBuffer;
    CSharedIndexBuffer m_indexBuffer;
    
public:
    
    
};


CMeshHeader::CMeshHeader(void) :
m_vertexData(nullptr),
m_indexData(nullptr),
m_numIndices(0),
m_numVerticies(0),
m_maxBound(glm::vec3(-4096.0f)),
m_minBound(glm::vec3( 4096.0f))
{
    
}

CMeshHeader::~CMeshHeader(void)
{
    delete[] m_vertexData;
    delete[] m_indexData;
}

CMesh::CMesh(const std::string& _guid) :
IResource(E_RESOURCE_CLASS_MESH, _guid),
m_header(nullptr),
m_vertexBuffer(nullptr),
m_indexBuffer(nullptr)
{
    
}

CMesh::CMesh(const std::string& _guid, std::shared_ptr<CVertexBuffer> _vertexBuffer, std::shared_ptr<CIndexBuffer> _indexBuffer) :
IResource(E_RESOURCE_CLASS_MESH, _guid),
m_header(std::make_shared<CMeshHeader>()),
m_vertexBuffer(_vertexBuffer),
m_indexBuffer(_indexBuffer)
{
    m_header->m_numVerticies = m_vertexBuffer->Get_Size();
    m_header->m_numIndices = m_indexBuffer->Get_Size();
    m_header->m_vertexData = nullptr;
    m_header->m_indexData = nullptr;
    
    SHardwareVertex* vertexData = m_vertexBuffer->Lock();
    
    for(ui32 i = 0; i < m_header->m_numVerticies; ++i)
    {
        if(vertexData[i].m_position.x > m_header->m_maxBound.x)
        {
            m_header->m_maxBound.x = vertexData[i].m_position.x;
        }
        if(vertexData[i].m_position.y > m_header->m_maxBound.y)
        {
            m_header->m_maxBound.y = vertexData[i].m_position.y;
        }
        if(vertexData[i].m_position.z > m_header->m_maxBound.z)
        {
            m_header->m_maxBound.z = vertexData[i].m_position.z;
        }
        if(vertexData[i].m_position.x < m_header->m_minBound.x)
        {
            m_header->m_minBound.x = vertexData[i].m_position.x;
        }
        if(vertexData[i].m_position.y < m_header->m_minBound.y)
        {
            m_header->m_minBound.y = vertexData[i].m_position.y;
        }
        if(vertexData[i].m_position.z < m_header->m_minBound.z)
        {
            m_header->m_minBound.z = vertexData[i].m_position.z;
        }
    }
    
    m_status |= E_RESOURCE_STATUS_LOADED;
    m_status |= E_RESOURCE_STATUS_COMMITED;
}

CMesh::~CMesh(void)
{
    m_bounds.clear();
}

void CMesh::_Set_Header(std::shared_ptr<CMeshHeader> _header)
{
    assert(_header != nullptr);
    m_header = _header;
    m_status |= E_RESOURCE_STATUS_LOADED;
    
    for(const auto& bound : m_bounds)
    {
        bound->_Set_MaxBound(m_header->m_maxBound);
        bound->_Set_MinBound(m_header->m_minBound);
    }
};

std::shared_ptr<CAABoundBox> CMesh::CreateBoundBox(void)
{
    std::shared_ptr<CAABoundBox> bound(std::make_shared<CAABoundBox>(m_header == nullptr ? glm::vec3(0.0f) : m_header->m_maxBound, m_header == nullptr ? glm::vec3(0.0f) : m_header->m_minBound));
    assert(bound != nullptr);
    m_bounds.push_back(bound);
    return bound;
}

void CMesh::Bind(const i32 *_attributes) const
{
    if(IResource::IsLoaded() && IResource::IsCommited())
    {
        assert(m_vertexBuffer != nullptr);
        assert(m_indexBuffer != nullptr);
        m_vertexBuffer->Bind(_attributes);
        m_indexBuffer->Bind();
    }
}

void CMesh::Draw(void) const
{
    if(IResource::IsLoaded() && IResource::IsCommited())
    {
        assert(m_vertexBuffer != nullptr);
        assert(m_indexBuffer != nullptr);
        glDrawElements(GL_TRIANGLES, m_indexBuffer->Get_Size(), GL_UNSIGNED_SHORT, NULL);
    }
}

void CMesh::Draw(ui32 _indices) const
{
    if(IResource::IsLoaded() && IResource::IsCommited())
    {
        assert(m_vertexBuffer != nullptr);
        assert(m_indexBuffer != nullptr);
        glDrawElements(GL_TRIANGLES, _indices, GL_UNSIGNED_SHORT, NULL);
    }
}

void CMesh::Unbind(const i32 *_attributes) const
{
    if(IResource::IsLoaded() && IResource::IsCommited())
    {
        assert(m_vertexBuffer != nullptr);
        assert(m_indexBuffer != nullptr);
        m_indexBuffer->Unbind();
        m_vertexBuffer->Unbind(_attributes);
    }
}
