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

CAnimationMixer::CAnimationMixer(CSharedSkeletonDataRef skeletonData,
                                 CSharedSequenceDataRef sequenceData) :
m_sequenceData(sequenceData),
m_animationTime(0.0)
{
    assert(skeletonData != nullptr);
    m_skeleton = std::make_shared<CSkeleton>(skeletonData);
    m_bonesTransformations = new glm::mat4x4[skeletonData->getNumBones()];
    CAnimationMixer::bindPoseTransformation();
}

CAnimationMixer::~CAnimationMixer(void)
{
    
}

void CAnimationMixer::bindPoseTransformation(void)
{
    assert(m_skeleton != nullptr);
    assert(m_sequenceData != nullptr);
    
    CSharedFrameData frame = m_sequenceData->getFrame(0);
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

void CAnimationMixer::setAnimation(const std::string& name)
{
    
}

void CAnimationMixer::update(f32 deltatime)
{
    assert(m_skeleton != nullptr);
    assert(m_sequenceData != nullptr);
    
    m_animationTime += deltatime;
    
    f32 animationDeltaTime = m_animationTime * m_sequenceData->getAnimationFPS();
    i32 floorAnimationDeltaTime = static_cast<i32>(floorf(animationDeltaTime));
    i32 frameIndex_01 = floorAnimationDeltaTime % m_sequenceData->getNumFrames();
    i32 frameIndex_02 = (frameIndex_01 + 1) % m_sequenceData->getNumFrames();
    f32 interpolation = animationDeltaTime - static_cast<f32>(floorAnimationDeltaTime);
    
    CSharedFrameData frame_01 = m_sequenceData->getFrame(frameIndex_01);
    CSharedFrameData frame_02 = m_sequenceData->getFrame(frameIndex_02);
    
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
