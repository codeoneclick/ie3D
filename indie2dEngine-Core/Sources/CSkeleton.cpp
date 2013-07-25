//
//  CSkeleton.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 7/25/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CSkeleton.h"
#include "CBone.h"

CSkeleton::CSkeleton(void) :
m_numBones(0),
m_root(nullptr)
{
    
}

CSkeleton::~CSkeleton(void)
{
    
}

void CSkeleton::_Serialize(std::ifstream &_stream)
{
    _stream>>m_numBones;
    std::string name;
    i32 id, parentId;
    for (i32 i = 0; i < m_numBones; ++i)
    {
        _stream>>name>>id>>parentId;
        CSkeleton::AddBone( std::make_shared<CBone>(name, id, parentId));
    }
}

void CSkeleton::AddBone(std::shared_ptr<CBone> _bone)
{
    if (_bone == nullptr)
    {
        return;
    }
    
    if (m_root == nullptr)
    {
        m_root = _bone;
        return;
    }
    
    std::shared_ptr<CBone> parent = Get_BoneById(_bone->Get_ParentId());
    if (parent != nullptr)
    {
        parent->LinkChildBone(_bone);
    }
}

std::shared_ptr<CBone> CSkeleton::Get_BoneById(i32 _id)
{
    if (m_root == nullptr)
    {
        return nullptr;
    }
    
    if (m_root->Get_Id() == _id)
    {
        return m_root;
    }
    return m_root->FindInChildrenById(_id);
}

void CSkeleton::AnimateHierarhy( void )
{
    if (m_root != nullptr)
        m_root->AnimateHierarhy(glm::mat4(1.0f));
}

void CSkeleton::SetupBindPosition( void )
{
    if (m_root != nullptr)
    {
        m_root->SetupBindPosition();
    }
}

