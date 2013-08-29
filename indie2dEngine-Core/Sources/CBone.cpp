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

CBone::CBone(i32 _id, i32 _parentId) :
m_id(_id),
m_parentId(_parentId),
m_parent(nullptr),
m_transformation(nullptr),
m_bindTransformation(glm::mat4x4(1.0f)),
m_baseTransformation(glm::mat4x4(1.0f))
{
   
}

CBone::~CBone(void)
{
    
}

void CBone::AddChild(std::shared_ptr<CBone> _bone)
{
    m_childs.push_back(_bone);
    _bone->m_parent = shared_from_this();
    _bone->m_parentId = m_id;
}

std::shared_ptr<CBone> CBone::FindChild(i32 _id)
{
    if(m_childs.size() == 0)
    {
        return nullptr;
    }
    std::shared_ptr<CBone> bone = nullptr;
    for(const auto& iterator : m_childs)
    {
        if(iterator->m_id == _id)
        {
            bone = iterator;
            break;
        }
        bone = iterator->FindChild(_id);
        if(bone != nullptr)
        {
            break;
        }
    }
    return bone;
}

void CBone::Update(void)
{
    for(const auto& iterator : m_childs)
    {
        iterator->Update();
    }
    m_baseTransformation = (*m_transformation) * glm::inverse(m_bindTransformation);
    (*m_transformation) = (*m_transformation) * m_bindTransformation;
}

void CBone::Set_BindTransformation(void)
{
    if (m_transformation != nullptr)
    {
        m_bindTransformation = glm::inverse(*m_transformation);
    }
    
    for(const auto& iterator : m_childs)
    {
        iterator->Set_BindTransformation();
    }
}
