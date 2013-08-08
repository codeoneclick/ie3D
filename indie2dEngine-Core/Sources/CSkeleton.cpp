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

CSkeleton::CSkeleton(void) :
m_numBones(0),
m_boneWidth(3.0f)
{
    
}

CSkeleton::~CSkeleton(void)
{
    
}

void CSkeleton::_Serialize(std::ifstream &_stream)
{
    _stream.read((char*)&m_numBones, sizeof(i32));
    i32 id, parentId;
    for (i32 i = 0; i < m_numBones; ++i)
    {
        _stream.read((char*)&id, sizeof(i32));
        _stream.read((char*)&parentId, sizeof(i32));
        std::shared_ptr<CBone> bone = std::make_shared<CBone>(id, parentId);
        bone->Set_Name("");
        CSkeleton::AddBone(bone);
    }
}

void CSkeleton::AddBone(std::shared_ptr<CBone> _bone)
{
    if (_bone == nullptr)
    {
        return;
    }
    
    if (_bone->Get_ParentId() == -1)
    {
        m_roots.push_back(_bone);
        return;
    }
    
    std::shared_ptr<CBone> parent = Get_BoneById(_bone->Get_ParentId());
    if (parent != nullptr)
    {
        parent->AddChild(_bone);
    }
}

std::shared_ptr<CBone> CSkeleton::Get_BoneById(i32 _id)
{
    for(auto root : m_roots)
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

void CSkeleton::Update( void )
{
    for(auto root : m_roots)
    {
        root->Update(nullptr);
    }
}

void CSkeleton::Set_BindTransformation(void)
{
    for(auto root : m_roots)
    {
        root->Set_BindTransformation();
    }

    std::function<void(void)> function = [this]()
    {
        i32 numIndexes = ((m_numBones - 1) * 2);
        m_indexBuffer = std::make_shared<CIndexBuffer>(numIndexes, GL_STATIC_DRAW);
        ui16* indexData = m_indexBuffer->Lock();
        i32 index = 0;
        for(auto root : m_roots)
        {
            root->WriteIndexData(indexData, &index);
        }
        m_indexBuffer->Unlock();
        
        m_vertexBuffer = std::make_shared<CVertexBuffer>(m_numBones, GL_DYNAMIC_DRAW);
        SVertex* vertexData = m_vertexBuffer->Lock();
        index = 0;
        for(auto root : m_roots)
        {
            index = root->WriteVertexData(vertexData, index);
        }
        m_vertexBuffer->Unlock();
    };
    gcdpp::impl::DispatchAsync(gcdpp::queue::GetMainQueue(), function);
}

void CSkeleton::Draw(const i32 *_attributes)
{
    if (m_vertexBuffer != nullptr && m_indexBuffer != nullptr)
    {
        for(auto root : m_roots)
        {
            root->Update(nullptr);
        }
        
        SVertex* vertexData = m_vertexBuffer->Lock();
        i32 index = 0;
        for(auto root : m_roots)
        {
            index = root->WriteVertexData(vertexData, index);
        }

        m_vertexBuffer->Unlock();

        assert(m_vertexBuffer != nullptr);
        assert(m_indexBuffer != nullptr);
        
        m_vertexBuffer->Bind(_attributes);
        m_indexBuffer->Bind();
        glLineWidth(m_boneWidth);
        glDrawElements(GL_LINES, m_indexBuffer->Get_NumIndexes(), GL_UNSIGNED_SHORT, NULL);
        m_vertexBuffer->Unbind(_attributes);
        m_indexBuffer->Unbind();
    }
}

