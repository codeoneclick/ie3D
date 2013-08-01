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
m_root(nullptr)
{
    
}

CSkeleton::~CSkeleton(void)
{
    
}

void CSkeleton::_Serialize(std::ifstream &_stream)
{
    _stream.read((char*)&m_numBones, sizeof(i32));
    i32 id, parentId, boneNameLength;
    for (i32 i = 0; i < m_numBones; ++i)
    {
        _stream.read((char*)&boneNameLength, sizeof(i32));
        char* buffer = new char[boneNameLength];
        _stream.read(buffer, boneNameLength * sizeof(char));
        _stream.read((char*)&id, sizeof(i32));
        _stream.read((char*)&parentId, sizeof(i32));
        CSkeleton::AddBone( std::make_shared<CBone>(id, parentId));
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
        parent->AddChild(_bone);
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
    return m_root->FindChild(_id);
}

void CSkeleton::AnimateHierarhy( void )
{
    if (m_root != nullptr)
        m_root->Update(nullptr);
}

void CSkeleton::Set_BindTransformation(void)
{
    m_root->Set_BindTransformation();
    /*return;
    std::function<void(void)> function = [this]()
    {
        
        i32 numIndexes = ((m_numBones - 1) * 2);
        
        m_indexBuffer = std::make_shared<CIndexBuffer>(numIndexes, GL_STATIC_DRAW);
        ui16* indexData = m_indexBuffer->Lock();
        m_root->FillIndexDataDebug(indexData, 0, 0);
        m_indexBuffer->Unlock();
        
        m_vertexBuffer = std::make_shared<CVertexBuffer>(m_numBones, GL_DYNAMIC_DRAW);
        SVertex* vertexData = m_vertexBuffer->Lock();
        m_root->FillVertexDataDebug(vertexData, 0);
        m_vertexBuffer->Unlock();
        
        for(i32 i = 0; i < numIndexes; ++i)
        {
            std::cout<<"[index] : "<<indexData[i]<<std::endl;
        }
    };
    gcdpp::impl::DispatchAsync(gcdpp::queue::GetMainQueue(), function);*/
}

void CSkeleton::DrawDebug(const i32 *_attributes)
{
    return;
    if (m_root != nullptr)
    {
        m_root->Update(nullptr);
        SVertex* vertexData = m_vertexBuffer->Lock();
        m_root->FillVertexDataDebug(vertexData, 0);
        m_vertexBuffer->Unlock();

        assert(m_vertexBuffer != nullptr);
        assert(m_indexBuffer != nullptr);
        
        m_vertexBuffer->Bind(_attributes);
        m_indexBuffer->Bind();
        glLineWidth(5.0f);
        glDrawElements(GL_LINES, m_indexBuffer->Get_NumIndexes(), GL_UNSIGNED_SHORT, NULL);
        m_vertexBuffer->Unbind(_attributes);
        m_indexBuffer->Unbind();
        
        //m_root->DrawDebug(_attributes);
    }
}

