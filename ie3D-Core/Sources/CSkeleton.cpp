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
m_numBones(skeletonData->getNumBones())
{
    assert(skeletonData != nullptr);
    m_bonesTransformations = new glm::mat4x4[skeletonData->getNumBones()];
    
    std::vector<CSharedBoneData> bonesRawData = skeletonData->getBonesRawData();
    for(const auto& boneRawData : bonesRawData)
    {
        i32 id = boneRawData->getBoneId();
        i32 parentId = boneRawData->getBoneParentId();
        CSharedBone bone = CSkeleton::getBone(id);
        if(bone == nullptr)
        {
            bone = std::make_shared<CBone>(id, parentId);
        }
        CSkeleton::addBone(bone);
    }
}

CSkeleton::~CSkeleton(void)
{
    
}

void CSkeleton::addBone(CSharedBoneRef bone)
{
    if(bone == nullptr)
    {
        return;
    }
    
    if (bone->getParentId() == -1)
    {
        m_rootBones.insert(bone);
        return;
    }
    
    CSharedBone parent = CSkeleton::getBone(bone->getParentId());
    if (parent != nullptr)
    {
        parent->addChild(bone);
        return;
    }
    assert(false);
}

CSharedBone CSkeleton::getBone(ui32 index) const
{
    for(const auto& root : m_rootBones)
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

ui32 CSkeleton::getNumBones(void) const
{
    return m_numBones;
}

const std::set<CSharedBone> CSkeleton::getRootBones(void) const
{
    return m_rootBones;
}

glm::mat4* CSkeleton::getBonesTransformations(void) const
{
    return m_bonesTransformations;
}

void CSkeleton::update(void)
{
    CSharedBone bone = nullptr;
    for (ui32 i = 0; i < CSkeleton::getNumBones(); ++i)
    {
        bone = CSkeleton::getBone(i);
        if (bone != nullptr)
        {
            bone->setTransformation(m_bonesTransformations + i);
        }
    }
    for(const auto& rootBone : m_rootBones)
    {
        rootBone->update();
    }
}

void CSkeleton::bindPoseTransformation(void)
{
    for(const auto& rootBone : m_rootBones)
    {
        rootBone->bindPoseTransformation();
    }
}