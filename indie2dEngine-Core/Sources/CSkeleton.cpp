//
//  CSkeleton.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 7/25/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CSkeleton.h"
#include "CBone.h"
#include "CVertexBuffer.h"
#include "CIndexBuffer.h"

CSkeleton::CSkeleton(const std::string& _guid) :
IResource(E_RESOURCE_CLASS_SKELETON, _guid),
m_numBones(0),
m_boneWidth(3.0f)
{
    
}

CSkeleton::~CSkeleton(void)
{
    for(const auto& transformation : m_transformations)
    {
        delete[] transformation.second;
    }
    m_transformations.clear();
}

void CSkeleton::_Serialize(std::istream *_stream)
{
    _stream->read((char*)&m_numBones, sizeof(i32));
    i32 id, parentId;

    for (ui32 i = 0; i < m_numBones; ++i)
    {
        _stream->read((char*)&id, sizeof(i32));
        _stream->read((char*)&parentId, sizeof(i32));
        std::shared_ptr<CBone> bone = CSkeleton::Get_Bone(id);
        if(bone == nullptr)
        {
            bone = std::make_shared<CBone>(id, parentId);
            bone->Set_Name("");
        }
        CSkeleton::AddBone(bone);
    }
    m_status |= E_RESOURCE_STATUS_LOADED;
}

void CSkeleton::_BindSkeleton(void)
{
    m_status |= E_RESOURCE_STATUS_COMMITED;
}


void CSkeleton::AddBone(std::shared_ptr<CBone> _bone)
{
    if(_bone == nullptr)
    {
        return;
    }
    
    if (_bone->Get_ParentId() == -1)
    {
        m_roots.insert(_bone);
        return;
    }
    
    std::shared_ptr<CBone> parent = Get_Bone(_bone->Get_ParentId());
    if (parent != nullptr)
    {
        parent->AddChild(_bone);
        return;
    }
    assert(false);
}

std::shared_ptr<CBone> CSkeleton::Get_Bone(i32 _id)
{
    for(const auto& root : m_roots)
    {
        if (root->Get_Id() == _id)
        {
            return root;
        }
        else
        {
            std::shared_ptr<CBone> child = root->FindChild(_id);
            if(child != nullptr)
            {
                return child;
            }
        }
    }
    return nullptr;
}

void CSkeleton::Update(const std::string& _guid)
{
    assert(m_transformations.find(_guid) != m_transformations.end());
    glm::mat4x4* transformation = m_transformations.find(_guid)->second;
    std::shared_ptr<CBone> bone;
    for (ui32 i = 0; i < CSkeleton::Get_NumBones(); ++i)
    {
        bone = CSkeleton::Get_Bone(i);
        if (bone != nullptr)
        {
            bone->Set_Transformation(transformation + i);
        }
    }

    for(const auto& root : m_roots)
    {
        root->Update();
    }
}

const std::string CSkeleton::_GenerateGuid(void)
{
    static ui32 value = 0;
    std::stringstream stringstream;
    stringstream<<"skeleton.reference."<<++value;
    return stringstream.str();
}

std::string CSkeleton::CreateTransformations(void)
{
    std::string guid = CSkeleton::_GenerateGuid();
    m_transformations.insert(std::make_pair(guid, new glm::mat4x4[CSkeleton::Get_NumBones()]));
    return guid;
}

glm::mat4* CSkeleton::Get_Transformations(const std::string &_guid)
{
    assert(m_transformations.find(_guid) != m_transformations.end());
    return m_transformations.find(_guid)->second;
}

void CSkeleton::BindTransformation(void)
{
    for(const auto& root : m_roots)
    {
        root->Set_BindTransformation();
    }
}

void CSkeleton::Draw(const i32 *_attributes, const std::string& _guid)
{

}

