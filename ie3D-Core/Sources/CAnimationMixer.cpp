//
//  CAnimationMixer.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 8/13/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CAnimationMixer.h"
#include "CMesh.h"
#include "CBone.h"
#include "CSkeleton.h"
#include "CAnimationSequence.h"

CAnimationMixer::CAnimationMixer(CSharedSkeletonDataRef skeletonData) :
m_animationTime(0.0),
m_currentAnimationSequence(nullptr),
m_currentAnimationName("")
{
    assert(skeletonData != nullptr);
    m_skeleton = std::make_shared<CSkeleton>(skeletonData);
    m_bonesTransformations = new glm::mat4x4[skeletonData->getNumBones()];
}

CAnimationMixer::~CAnimationMixer(void)
{
    
}

void CAnimationMixer::bindPoseTransformation(void)
{
    assert(m_skeleton != nullptr);
    assert(m_currentAnimationSequence != nullptr);
    
    CSharedFrameData frame = m_currentAnimationSequence->getFrame(0);
    CSharedBone bone = nullptr;
    
    for (ui32 i = 0; i < m_skeleton->getNumBones(); ++i)
    {
        glm::vec3 position = frame->getPosition(i);
        glm::quat rotation = frame->getRotation(i);
        glm::vec3 scale = frame->getScale(i);
        
        glm::mat4x4 matrixTranslation = glm::translate(glm::mat4(1.0f), position);
        glm::mat4x4 matrixRotation = glm::toMat4(rotation);
        glm::mat4x4 matrixScale = glm::scale(glm::mat4x4(1.0f), scale);
        m_skeleton->getBonesTransformations()[i] = matrixTranslation * matrixRotation * matrixScale;
    }
    m_skeleton->update();
    m_skeleton->bindPoseTransformation();
}

glm::mat4x4* CAnimationMixer::getTransformations(void) const
{
    for(ui32 i = 0; i < m_skeleton->getNumBones(); ++i)
    {
        CSharedBone bone = m_skeleton->getBone(i);
        if(bone->getTransformation() != nullptr)
        {
            glm::mat4x4 boneTransformation = (*bone->getTransformation());
            m_bonesTransformations[i] = boneTransformation;
        }
        else
        {
            m_bonesTransformations[i] = glm::mat4x4(1.0f);
        }
    }
    return m_bonesTransformations;
}

ui32 CAnimationMixer::getTransformationSize(void) const
{
    return m_skeleton->getNumBones();
}

bool CAnimationMixer::tryBindCurrentAnimationSequence(void)
{
    const auto& iterator = m_animationsSequences.find(m_currentAnimationName);
    if(iterator != m_animationsSequences.end())
    {
        if(iterator->second->isLoaded())
        {
            m_currentAnimationSequence = iterator->second;
            CAnimationMixer::bindPoseTransformation();
            return true;
        }
    }
    return false;
}

void CAnimationMixer::setAnimation(const std::string& name)
{
    if(m_currentAnimationName != name)
    {
        m_currentAnimationName = name;
        m_currentAnimationSequence = nullptr;
        CAnimationMixer::tryBindCurrentAnimationSequence();
    }
}

void CAnimationMixer::update(f32 deltatime)
{
    assert(m_skeleton != nullptr);
    
    if(m_currentAnimationName.length() != 0)
    {
        bool isCurrentAnimationSequenceBinded = true;
        if(m_currentAnimationSequence == nullptr)
        {
            isCurrentAnimationSequenceBinded = CAnimationMixer::tryBindCurrentAnimationSequence();
        }
        
        if(isCurrentAnimationSequenceBinded)
        {
            m_animationTime += deltatime;
            
            f32 animationDeltaTime = m_animationTime * m_currentAnimationSequence->getAnimationFPS();
            i32 floorAnimationDeltaTime = static_cast<i32>(floorf(animationDeltaTime));
            i32 frameIndex_01 = floorAnimationDeltaTime % m_currentAnimationSequence->getNumFrames();
            i32 frameIndex_02 = (frameIndex_01 + 1) % m_currentAnimationSequence->getNumFrames();
            f32 interpolation = animationDeltaTime - static_cast<f32>(floorAnimationDeltaTime);
            
            CSharedFrameData frame_01 = m_currentAnimationSequence->getFrame(frameIndex_01);
            CSharedFrameData frame_02 = m_currentAnimationSequence->getFrame(frameIndex_02);
            
            for (ui32 i = 0; i < m_skeleton->getNumBones(); ++i)
            {
                glm::vec3 position = glm::mix(frame_01->getPosition(i), frame_02->getPosition(i), interpolation );
                glm::quat rotation = glm::slerp(frame_01->getRotation(i), frame_02->getRotation(i), interpolation);
                glm::vec3 scale = glm::mix(frame_01->getScale(i), frame_02->getScale(i), interpolation);
                
                glm::mat4x4 matrixTranslation = glm::translate(glm::mat4(1.0f), position);
                glm::mat4x4 matrixRotation = glm::toMat4(rotation);
                glm::mat4x4 matrixScale = glm::scale(glm::mat4x4(1.0f), scale);
                m_skeleton->getBonesTransformations()[i] = matrixTranslation * matrixRotation * matrixScale;
            }
            m_skeleton->update();
        }
    }
}

bool CAnimationMixer::isAnimated(void)
{
    return m_currentAnimationName.length() != 0 && m_currentAnimationSequence != nullptr;
}

void CAnimationMixer::addAnimationSequence(CSharedAnimationSequenceRef animationSequence)
{
    std::string animationName = animationSequence->getAnimationName();
    m_animationsSequences.insert(std::make_pair(animationName, animationSequence));
}

void CAnimationMixer::removeAnimationSequence(CSharedAnimationSequenceRef animationSequence)
{
    const auto& iterator = m_animationsSequences.find(animationSequence->getAnimationName());
    if(iterator != m_animationsSequences.end())
    {
        m_animationsSequences.erase(iterator);
    }
}
