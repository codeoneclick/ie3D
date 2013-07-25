//
//  CMesh.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/26/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CMesh.h"
#include "CAABoundBox.h"
#include "CSkeleton.h"
#include "CBone.h"
#include "CAnimationSequence.h"

CMeshHeader::CMeshHeader(void) :
m_vertexData(nullptr),
m_indexData(nullptr),
m_numIndexes(0),
m_numVertexes(0),
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
IResource(E_RESOURCE_TYPE_MESH, _guid),
m_header(nullptr),
m_vertexBuffer(nullptr),
m_indexBuffer(nullptr),
m_skeleton(std::make_shared<CSkeleton>()),
m_sequence(std::make_shared<CAnimationSequence>()),
m_animationTime(0.0f),
m_bonesTransformation(nullptr),
m_sequenceData(nullptr)
{
    
}

CMesh::CMesh(const std::string& _guid, std::shared_ptr<CVertexBuffer> _vertexBuffer, std::shared_ptr<CIndexBuffer> _indexBuffer) :
IResource(E_RESOURCE_TYPE_MESH, _guid),
m_header(std::make_shared<CMeshHeader>()),
m_vertexBuffer(_vertexBuffer),
m_indexBuffer(_indexBuffer),
m_skeleton(std::make_shared<CSkeleton>()),
m_sequence(std::make_shared<CAnimationSequence>()),
m_animationTime(0.0f),
m_bonesTransformation(nullptr),
m_sequenceData(nullptr)
{
    assert(_vertexBuffer != nullptr);
    assert(m_indexBuffer != nullptr);
    m_header->m_numVertexes = m_vertexBuffer->Get_NumVertexes();
    m_header->m_numIndexes = m_indexBuffer->Get_NumIndexes();
    m_header->m_vertexData = m_vertexBuffer->Lock();
    m_header->m_indexData = m_indexBuffer->Lock();
    
    for(ui32 i = 0; i < m_header->m_numVertexes; ++i)
    {
        if(m_header->m_vertexData[i].m_position.x > m_header->m_maxBound.x)
        {
            m_header->m_maxBound.x = m_header->m_vertexData[i].m_position.x;
        }
        if(m_header->m_vertexData[i].m_position.y > m_header->m_maxBound.y)
        {
            m_header->m_maxBound.y = m_header->m_vertexData[i].m_position.y;
        }
        if(m_header->m_vertexData[i].m_position.z > m_header->m_maxBound.z)
        {
            m_header->m_maxBound.z = m_header->m_vertexData[i].m_position.z;
        }
        if(m_header->m_vertexData[i].m_position.x < m_header->m_minBound.x)
        {
            m_header->m_minBound.x = m_header->m_vertexData[i].m_position.x;
        }
        if(m_header->m_vertexData[i].m_position.y < m_header->m_minBound.y)
        {
            m_header->m_minBound.y = m_header->m_vertexData[i].m_position.y;
        }
        if(m_header->m_vertexData[i].m_position.z < m_header->m_minBound.z)
        {
            m_header->m_minBound.z = m_header->m_vertexData[i].m_position.z;
        }
    }

    m_isLoaded = true;
    m_isLinked = true;
}

CMesh::~CMesh(void)
{
    m_bounds.clear();
    delete[] m_bonesTransformation;
    delete[] m_sequenceData;
}

void CMesh::_Set_Header(std::shared_ptr<CMeshHeader> _header)
{
    assert(_header != nullptr);
    m_header = _header;
    m_isLoaded = true;
    
    for(auto bound : m_bounds)
    {
        bound->_Set_MaxBound(m_header->Get_MaxBound());
        bound->_Set_MinBound(m_header->Get_MinBound());
    }
};

std::shared_ptr<CAABoundBox> CMesh::CreateBoundBox(void)
{
    std::shared_ptr<CAABoundBox> bound(std::make_shared<CAABoundBox>(m_header == nullptr ? glm::vec3(0.0f) : m_header->Get_MaxBound(), m_header == nullptr ? glm::vec3(0.0f) : m_header->Get_MinBound()));
    assert(bound != nullptr);
    m_bounds.push_back(bound);
    return bound;
}

void CMesh::_BindSkeleton(void)
{
    assert(m_skeleton != nullptr);
    assert(m_bonesTransformation == nullptr);
    m_bonesTransformation = new glm::mat4x4[m_skeleton->Get_NumBones()];
    
    std::shared_ptr<CBone> bone;
    for (int i = 0; i < m_skeleton->Get_NumBones(); ++i)
    {
        if (bone == m_skeleton->Get_BoneById(i))
        {
            bone->Set_Transformation(m_bonesTransformation[i]);
        }
    }
}

void CMesh::_BindSequence(void)
{
    assert(m_skeleton != nullptr);
    assert(m_sequence != nullptr);
    CMesh::OnUpdate(0);
    m_skeleton->SetupBindPosition();
}

glm::vec3 CMesh::_TransformVertex(const glm::vec3 &_vertex, const glm::mat4x4 &_matrix)
{
    glm::vec4 value = _matrix * glm::vec4(_vertex, 1.0f);
    return glm::vec3(value.x, value.y, value.z);
}

void CMesh::OnUpdate(f32 _deltatime)
{
    if(m_isLoaded && m_isLinked && m_sequence->Get_NumFrames() != 0)
    {
        assert(m_skeleton != nullptr);
        const f32 fps  = 10.0f;
        m_animationTime += _deltatime;
        
        f32 animationDeltaTime = m_animationTime * fps;
        i32 floorAnimationDeltaTime = static_cast<i32>(floorf(animationDeltaTime));
        i32 frameIndex_01 = floorAnimationDeltaTime % m_sequence->Get_NumFrames();
        i32 frameIndex_02 = (frameIndex_01 + 1) % m_sequence->Get_NumFrames();
        f32 interpolation = animationDeltaTime - static_cast<f32>(floorAnimationDeltaTime);
        
        std::shared_ptr<CAnimationFrame> frame_01 = m_sequence->Get_AnimationFrame(frameIndex_01);
        std::shared_ptr<CAnimationFrame> frame_02 = m_sequence->Get_AnimationFrame(frameIndex_02);
        
        for (i32 i = 0; i < m_skeleton->Get_NumBones(); ++i)
        {
            glm::vec3 position = glm::mix(frame_01->Get_Position(i), frame_02->Get_Position(i), interpolation );
            glm::quat rotation = glm::mix(frame_01->Get_Rotation(i), frame_02->Get_Rotation(i), interpolation );
            
            m_bonesTransformation[i] = glm::toMat4(rotation);
            m_bonesTransformation[i] = glm::translate(m_bonesTransformation[i], position);
        }
        m_skeleton->AnimateHierarhy();
        
        SVertex* vertexData = m_vertexBuffer->Lock();
        for(i32 i = 0; i < m_vertexBuffer->Get_NumVertexes(); ++i)
        {
            for(i32 j = 0; j < m_sequenceData[i].m_numWeights; ++j)
            {
                glm::mat4x4 boneTransformation = m_bonesTransformation[m_sequenceData[i].m_weights[j].m_boneId];
                f32 weight = m_sequenceData[i].m_weights[j].m_weigth;
                
                if (j != 0)
                {
                    vertexData[i].m_position = CMesh::_TransformVertex(m_sequenceData[i].m_position, boneTransformation) * weight;
                    vertexData[i].m_normal = CVertexBuffer::CompressVec3(CMesh::_TransformVertex(m_sequenceData[i].m_normal, boneTransformation) * weight);
                }
                else
                {
                    vertexData[i].m_position += CMesh::_TransformVertex(m_sequenceData[i].m_position, boneTransformation) * weight;
                    vertexData[i].m_normal += CVertexBuffer::CompressVec3(CMesh::_TransformVertex(m_sequenceData[i].m_normal, boneTransformation) * weight);
                }
            }
        }
        m_vertexBuffer->Unlock();
    }
}

void CMesh::Bind(const i32 *_attributes)
{
    if(m_isLoaded && m_isLinked)
    {
        assert(m_vertexBuffer != nullptr);
        assert(m_indexBuffer != nullptr);
        m_vertexBuffer->Bind(_attributes);
        m_indexBuffer->Bind();
    }
}

void CMesh::Draw(void)
{
    if(m_isLoaded && m_isLinked)
    {
        assert(m_vertexBuffer != nullptr);
        assert(m_indexBuffer != nullptr);
        glDrawElements(GL_TRIANGLES, m_indexBuffer->Get_NumIndexes(), GL_UNSIGNED_SHORT, NULL);
    }
}

void CMesh::Unbind(const i32 *_attributes)
{
    if(m_isLoaded && m_isLinked)
    {
        assert(m_vertexBuffer != nullptr);
        assert(m_indexBuffer != nullptr);
        m_vertexBuffer->Unbind(_attributes);
        m_indexBuffer->Unbind();
    }
}
