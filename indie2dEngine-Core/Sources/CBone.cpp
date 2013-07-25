//
//  CBone.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 7/25/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CBone.h"

CBone::CBone(const std::string& _name, i32 _id, i32 _parentId) :
m_name(_name),
m_id(_id),
m_parentId(_parentId),
m_parent(nullptr),
m_next(nullptr),
m_child(nullptr),
m_transformation(glm::mat4x4(1.0f)),
m_bindPosition(glm::mat4x4(1.0f))
{
    
}

CBone::~CBone(void)
{
    
}

void CBone::LinkChildBone(std::shared_ptr<CBone> _bone)
{
    std::shared_ptr<CBone> oldchild = m_child;
    m_child = _bone;
    m_child->m_next = oldchild;
    m_child->m_parent = shared_from_this();
}

std::shared_ptr<CBone> CBone::FindInChildrenById(i32 _id)
{
    if (m_child == nullptr)
        return nullptr;
    
    std::shared_ptr<CBone> child = m_child;
    std::shared_ptr<CBone> found = nullptr;
    while (child != nullptr)
    {
        if (child->Get_Id() == _id)
            return child;
        
        found = child->FindInChildrenById(_id);
        if (found != nullptr)
            break;
        child = child->Get_Next();
    }
    return found;
}

void CBone::AnimateHierarhy(const glm::mat4x4 &_transformation)
{
    m_transformation = _transformation * m_transformation;
    std::shared_ptr<CBone> child = m_child;
    while(child != nullptr)
    {
        child->AnimateHierarhy(m_transformation);
        child = child->Get_Next();
    }
    m_transformation = m_transformation * m_bindPosition;
}

void CBone::SetupBindPosition(void)
{
    Set_BindPosition(glm::inverse(m_transformation));
    std::shared_ptr<CBone> child = m_child;
    while(child != nullptr)
    {
        child->SetupBindPosition();
        child = child->Get_Next();
    }
}