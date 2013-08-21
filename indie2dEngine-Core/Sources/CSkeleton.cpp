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
    for(auto transformation : m_transformations)
    {
        delete[] transformation.second;
    }
    m_transformations.clear();
}

void CSkeleton::_Serialize(std::ifstream &_stream)
{
    _stream.read((char*)&m_numBones, sizeof(i32));
    i32 id, parentId;

    for (i32 i = 0; i < m_numBones; ++i)
    {
        _stream.read((char*)&id, sizeof(i32));
        _stream.read((char*)&parentId, sizeof(i32));
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

void CSkeleton::Update(const std::string& _guid)
{
    assert(m_transformations.find(_guid) != m_transformations.end());
    glm::mat4x4* transformation = m_transformations.find(_guid)->second;
    std::shared_ptr<CBone> bone;
    for (int i = 0; i < CSkeleton::Get_NumBones(); ++i)
    {
        bone = CSkeleton::Get_Bone(i);
        if (bone != nullptr)
        {
            bone->Set_Transformation(transformation + i);
        }
    }

    for(auto root : m_roots)
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
    for(auto root : m_roots)
    {
        root->Set_BindTransformation();
    }
    
    /*std::function<void(void)> function = [this]()
    {
        i32 numIndexes = ((m_numBones - 1) * 2);
        m_indexBuffer = std::make_shared<CIndexBuffer>(numIndexes, GL_STATIC_DRAW);
        ui16* indexData = m_indexBuffer->Lock();
        memset(indexData, 0x0, sizeof(ui16) * m_indexBuffer->Get_NumIndexes());
        i32 index = 0;
        for(auto root : m_roots)
        {
            root->WriteIndexData(indexData, &index, numIndexes);
        }
        m_indexBuffer->Unlock();
        
        for(i32 i = 0; i < numIndexes; ++i)
        {
            std::cout<<"[Index] "<<indexData[i]<<std::endl;
        }
        
        m_vertexBuffer = std::make_shared<CVertexBuffer>(m_numBones, GL_DYNAMIC_DRAW);
        SVertex* vertexData = m_vertexBuffer->Lock();
        for(auto root : m_roots)
        {
           root->WriteVertexData(vertexData, m_vertexBuffer->Get_Size());
        }
        m_vertexBuffer->Unlock();
    };
    gcdpp::impl::DispatchAsync(gcdpp::queue::GetMainQueue(), function);*/
}

void CSkeleton::Draw(const i32 *_attributes)
{
    return;
    if (m_vertexBuffer != nullptr && m_indexBuffer != nullptr)
    {
        for(auto root : m_roots)
        {
            root->Update();
        }
        
        std::function<void(void)> function = [this]()
        {
            SVertex* vertexData = m_vertexBuffer->Lock();
        
            for(auto root : m_roots)
            {
                root->WriteVertexData(vertexData, m_vertexBuffer->Get_Size());
            }
            
            std::function<void(void)> main = [this]()
            {
                m_vertexBuffer->Unlock();
            };
            gcdpp::impl::DispatchAsync(gcdpp::queue::GetMainQueue(), main);
        };
        
        gcdpp::impl::DispatchAsync(gcdpp::queue::GetGlobalQueue(gcdpp::queue::GCDPP_DISPATCH_QUEUE_PRIORITY_LOW), function);
        
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

