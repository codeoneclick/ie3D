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

void CBone::WriteIndexData(ui16 *_indexData, i32* _offset, i32 _numIndexes)
{
    assert((*_offset) < _numIndexes);
    
    for(const auto& iterator : m_childs)
    {
        _indexData[(*_offset)] = m_id;
        i32 index = iterator->Get_Id();
        _indexData[(*_offset) + 1] = index;
        (*_offset) += 2;
        iterator->WriteIndexData(_indexData, _offset, _numIndexes);
    }
}

void CBone::WriteVertexData(SVertex *_vertexData, i32 _numVertexes)
{
    assert(m_id < _numVertexes);
    
    _vertexData[m_id].m_position = glm::transform(glm::vec3(0.0f, 0.0f, 0.0f), m_baseTransformation);
    for(const auto& iterator : m_childs)
    {
        iterator->WriteVertexData(_vertexData, _numVertexes);
    }
}