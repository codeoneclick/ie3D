//
//  CBone.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 7/25/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CBone.h"
#include "CVertexBuffer.h"
#include "CIndexBuffer.h"

CBoneData::CBoneData(i32 id, i32 parentId) :
m_id(id),
m_parentId(parentId)
{
    
}

CBoneData::~CBoneData(void)
{
    
}

i32 CBoneData::getBoneId(void) const
{
    return m_id;
}

i32 CBoneData::getBoneParentId(void) const
{
    return m_parentId;
}

CBone::CBone(i32 id, i32 parentId) :
m_id(id),
m_parentId(parentId),
m_parent(nullptr),
m_transformation(nullptr),
m_baseTransformation(glm::mat4(1.0)),
m_bindTransformation(glm::mat4(1.0))
{
    
}

CBone::~CBone(void)
{
    
}

void CBone::addChild(CSharedBoneRef bone)
{
    assert(bone != nullptr);
    m_children.push_back(bone);
    bone->m_parent = shared_from_this();
    bone->m_parentId = m_id;
}

CSharedBone CBone::findChild(i32 id)
{
    if(m_children.size() == 0)
    {
        return nullptr;
    }
    CSharedBone bone = nullptr;
    for(const auto& iterator : m_children)
    {
        if(iterator->m_id == id)
        {
            bone = iterator;
            break;
        }
        bone = iterator->findChild(id);
        if(bone != nullptr)
        {
            break;
        }
    }
    return bone;
}

const std::vector<CSharedBone>& CBone::getChildren(void) const
{
    return m_children;
}

CSharedBone CBone::getParent(void) const
{
    return m_parent;
}

i32 CBone::getId(void) const
{
    return m_id;
}

i32 CBone::getParentId(void) const
{
    return m_parentId;
}

void CBone::setTransformation(glm::mat4* transformation)
{
    m_transformation = transformation;
}

glm::mat4* CBone::getTransformation(void) const
{
    return m_transformation;
}

glm::mat4 CBone::getBaseTransformation(void) const
{
    return m_baseTransformation;
}

void CBone::update(void)
{
    for(const auto& iterator : m_children)
    {
        iterator->update();
    }
    m_baseTransformation = (*m_transformation) * glm::inverse(m_bindTransformation);
    (*m_transformation) = (*m_transformation) * m_bindTransformation;
}

void CBone::bindPoseTransformation(void)
{
    if (m_transformation != nullptr)
    {
        m_bindTransformation = glm::inverse(*m_transformation);
    }
    for(const auto& iterator : m_children)
    {
        iterator->bindPoseTransformation();
    }
}
