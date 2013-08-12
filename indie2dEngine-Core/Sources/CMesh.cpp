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
#include "CTimer.h"
#include "CSequence.h"

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
m_sequence(std::make_shared<CSequence>()),
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
m_sequence(std::make_shared<CSequence>()),
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
        bone = m_skeleton->Get_BoneById(i);
        if (bone != nullptr)
        {
            bone->Set_Transformation(m_bonesTransformation + i);
        }
    }
}

void CMesh::_BindSequence(void)
{
    assert(m_skeleton != nullptr);
    assert(m_sequence != nullptr);
    
    std::shared_ptr<CFrame> frame = m_sequence->Get_AnimationFrame(0);
    std::shared_ptr<CBone> bone;
    
    for (i32 i = 0; i < m_skeleton->Get_NumBones(); ++i)
    {
        glm::vec3 position = frame->Get_Position(i);
        glm::quat rotation = frame->Get_Rotation(i);
        glm::vec3 scale = frame->Get_Scale(i);
        
        glm::mat4x4 matrixTranslation = glm::translate(glm::mat4(1.0f), position);
        glm::mat4x4 matrixRotation = glm::toMat4(rotation);
        glm::mat4x4 matrixScale = glm::scale(glm::mat4x4(1.0f), scale);
        m_bonesTransformation[i] = matrixTranslation * matrixRotation * matrixScale;
    }
    
    m_skeleton->Update();
    m_skeleton->Set_BindTransformation();
}

void CMesh::OnUpdate(f32 _deltatime)
{
    static CTimer::CTime oldTime;
    CTimer::CTime currentTime = CTimer::CClock::now();
    
    if(m_isLoaded && m_isLinked && m_sequence->Get_NumFrames() != 0 && CTimer::Get_TimeInterval(currentTime, oldTime) > 0)
    {
        oldTime = currentTime;
        
        assert(m_skeleton != nullptr);
        m_animationTime += _deltatime;
        
        std::function<void(void)> function = [this]()
        {
            f32 animationDeltaTime = m_animationTime * m_sequence->Get_Fps();
            i32 floorAnimationDeltaTime = static_cast<i32>(floorf(animationDeltaTime));
            i32 frameIndex_01 = floorAnimationDeltaTime % m_sequence->Get_NumFrames();
            i32 frameIndex_02 = (frameIndex_01 + 1) % m_sequence->Get_NumFrames();
            f32 interpolation = animationDeltaTime - static_cast<f32>(floorAnimationDeltaTime);
            
            std::shared_ptr<CFrame> frame_01 = m_sequence->Get_AnimationFrame(frameIndex_01);
            std::shared_ptr<CFrame> frame_02 = m_sequence->Get_AnimationFrame(frameIndex_02);
            
            std::shared_ptr<CBone> bone;
            
            for (i32 i = 0; i < m_skeleton->Get_NumBones(); ++i)
            {
                glm::vec3 position = glm::mix(frame_01->Get_Position(i), frame_02->Get_Position(i), interpolation );
                glm::quat rotation = glm::slerp(frame_01->Get_Rotation(i), frame_02->Get_Rotation(i), interpolation);
                glm::vec3 scale = glm::mix(frame_01->Get_Scale(i), frame_02->Get_Scale(i), interpolation);
                
                glm::mat4x4 matrixTranslation = glm::translate(glm::mat4(1.0f), position);
                glm::mat4x4 matrixRotation = glm::toMat4(rotation);
                glm::mat4x4 matrixScale = glm::scale(glm::mat4x4(1.0f), scale);
                
                m_bonesTransformation[i] = matrixTranslation * matrixRotation * matrixScale;
            }
            m_skeleton->Update();
            
            SVertex* vertexData = m_vertexBuffer->Lock();
            for(i32 i = 0; i < m_vertexBuffer->Get_NumVertexes(); ++i)
            {
                f32 sumWeights = 0.0f;
                glm::vec3 bonePosition(0.0f);
                for(i32 j = 0; j < m_sequenceData[i].m_numWeights; ++j)
                {
                    std::shared_ptr<CBone> bone = m_skeleton->Get_BoneById(m_sequenceData[i].m_weights[j].m_boneId);
                    if(bone == nullptr)
                    {
                        std::cout<<m_sequenceData[i].m_weights[j].m_boneId<<std::endl;
                        continue;
                    }
                    glm::mat4x4 boneTransformation = (*bone->Get_Transformation());
                    
                    f32 weight = m_sequenceData[i].m_weights[j].m_weigth;
                    if(j == 0)
                    {
                        bonePosition = glm::transform(m_sequenceData[i].m_position, boneTransformation) * weight;
                    }
                    else
                    {
                        bonePosition += glm::transform(m_sequenceData[i].m_position, boneTransformation) * weight;
                    }
                    sumWeights += weight;
                }
                vertexData[i].m_position = bonePosition;
            }
            
            std::function<void(void)> main = [this]()
            {
                m_vertexBuffer->Unlock();
            };
            gcdpp::impl::DispatchAsync(gcdpp::queue::GetMainQueue(), main);
        };
        gcdpp::impl::DispatchAsync(gcdpp::queue::GetGlobalQueue(gcdpp::queue::GCDPP_DISPATCH_QUEUE_PRIORITY_LOW), function);
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
