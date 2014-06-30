//
//  CSkeleton.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 7/25/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CSkeleton.h"
#include "CBone.h"
#include "CMesh.h"

CSkeleton::CSkeleton(CSharedSkeletonDataRef skeletonData) :
m_skeletonData(skeletonData)
{
    assert(m_skeletonData != nullptr);
    m_bonesTransformations = new glm::mat4x4[m_skeletonData->getNumBones()];
}

CSkeleton::~CSkeleton(void)
{
    
}

ui32 CSkeleton::getNumBones(void) const
{
    assert(m_skeletonData != nullptr);
    return m_skeletonData->getNumBones();
}

CSharedBone CSkeleton::getBone(ui32 index) const
{
    assert(m_skeletonData != nullptr);
    return m_skeletonData->getBone(index);
}

glm::mat4* CSkeleton::getBonesTransformations(void) const
{
    return m_bonesTransformations;
}

void CSkeleton::update(void)
{
    assert(m_skeletonData != nullptr);
    CSharedBone bone = nullptr;
    for (ui32 i = 0; i < m_skeletonData->getNumBones(); ++i)
    {
        bone = m_skeletonData->getBone(i);
        if (bone != nullptr)
        {
            bone->setTransformation(m_bonesTransformations + i);
        }
    }
    for(const auto& rootBone : m_skeletonData->getRootBones())
    {
        rootBone->update();
    }
}

void CSkeleton::bindPoseTransformation(void)
{
    assert(m_skeletonData != nullptr);
    for(const auto& rootBone : m_skeletonData->getRootBones())
    {
        rootBone->bindPoseTransformation();
    }
}