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
m_transformation(nullptr)
{
    std::function<void(void)> function = [this]()
    {
        m_vertexBuffer = std::make_shared<CVertexBuffer>(24, GL_DYNAMIC_DRAW);
        SVertex* vertexData = m_vertexBuffer->Lock();
        
        m_maxBound = glm::vec3( 2.5f,  0.5f,  0.5f);
        m_minBound = glm::vec3(-2.5f, -0.5f, -0.5f);
        
        vertexData[0].m_position = glm::vec3( m_minBound.x,  m_minBound.y, m_maxBound.z);
        vertexData[1].m_position = glm::vec3( m_maxBound.x,  m_minBound.y, m_maxBound.z);
        vertexData[2].m_position = glm::vec3( m_maxBound.x,  m_maxBound.y, m_maxBound.z);
        vertexData[3].m_position = glm::vec3( m_minBound.x,  m_maxBound.y, m_maxBound.z);
        
        vertexData[4].m_position = glm::vec3( m_minBound.x,  m_minBound.y,  m_minBound.z);
        vertexData[5].m_position = glm::vec3( m_minBound.x,  m_maxBound.y,  m_minBound.z);
        vertexData[6].m_position = glm::vec3( m_maxBound.x,  m_maxBound.y,  m_minBound.z);
        vertexData[7].m_position = glm::vec3( m_maxBound.x,  m_minBound.y,  m_minBound.z);
        
        vertexData[8].m_position = glm::vec3( m_minBound.x,  m_maxBound.y,  m_minBound.z);
        vertexData[9].m_position = glm::vec3( m_minBound.x,  m_maxBound.y,  m_maxBound.z);
        vertexData[10].m_position = glm::vec3( m_maxBound.x,  m_maxBound.y,  m_maxBound.z);
        vertexData[11].m_position = glm::vec3( m_maxBound.x,  m_maxBound.y,  m_minBound.z);
        
        vertexData[12].m_position = glm::vec3( m_minBound.x,  m_maxBound.y,  m_minBound.z);
        vertexData[13].m_position = glm::vec3( m_maxBound.x,  m_maxBound.y,  m_minBound.z);
        vertexData[14].m_position = glm::vec3( m_maxBound.x,  m_maxBound.y,  m_maxBound.z);
        vertexData[15].m_position = glm::vec3( m_minBound.x,  m_maxBound.y,  m_maxBound.z);
        
        vertexData[16].m_position = glm::vec3( m_maxBound.x,  m_minBound.y,  m_minBound.z);
        vertexData[17].m_position = glm::vec3( m_maxBound.x,  m_maxBound.y,  m_minBound.z);
        vertexData[18].m_position = glm::vec3( m_maxBound.x,  m_maxBound.y,  m_maxBound.z);
        vertexData[19].m_position = glm::vec3( m_maxBound.x,  m_minBound.y,  m_maxBound.z);
        
        vertexData[20].m_position = glm::vec3( m_minBound.x,  m_minBound.y,  m_minBound.z);
        vertexData[21].m_position = glm::vec3( m_minBound.x,  m_minBound.y,  m_maxBound.z);
        vertexData[22].m_position = glm::vec3( m_minBound.x,  m_maxBound.y,  m_maxBound.z);
        vertexData[23].m_position = glm::vec3( m_minBound.x,  m_maxBound.y,  m_minBound.z);
        
        m_vertexBuffer->Unlock();
        
        m_indexBuffer = std::make_shared<CIndexBuffer>(36, GL_STATIC_DRAW);
        ui16* indexData = m_indexBuffer->Lock();
        
        indexData[0] = 0;
        indexData[1] = 1;
        indexData[2] = 2;
        indexData[3] = 0;
        indexData[4] = 2;
        indexData[5] = 3;
        
        indexData[6] = 4;
        indexData[7] = 5;
        indexData[8] = 6;
        indexData[9] = 4;
        indexData[10] = 6;
        indexData[11] = 7;
        
        indexData[12] = 8;
        indexData[13] = 9;
        indexData[14] = 10;
        indexData[15] = 8;
        indexData[16] = 10;
        indexData[17] = 11;
        
        indexData[18] = 12;
        indexData[19] = 13;
        indexData[20] = 14;
        indexData[21] = 12;
        indexData[22] = 14;
        indexData[23] = 15;
        
        indexData[24] = 16;
        indexData[25] = 17;
        indexData[26] = 18;
        indexData[27] = 16;
        indexData[28] = 18;
        indexData[29] = 19;
        
        indexData[30] = 20;
        indexData[31] = 21;
        indexData[32] = 22;
        indexData[33] = 20;
        indexData[34] = 22;
        indexData[35] = 23;
        
        m_indexBuffer->Unlock();
    };
    gcdpp::impl::DispatchAsync(gcdpp::queue::GetMainQueue(), function);
}

CBone::~CBone(void)
{
    
}

glm::vec3 CBone::_TransformVertex(const glm::vec3 &_vertex, const glm::mat4x4 &_matrix)
{
    glm::vec4 value = _matrix * glm::vec4(_vertex, 1.0f);
    return glm::vec3(value.x, value.y, value.z);
}

void CBone::_Update(const glm::mat4x4 &_matrix)
{
    SVertex* vertexData = m_vertexBuffer->Lock();
    
    static f32 offset = 0.1f;
    
    vertexData[0].m_position = CBone::_TransformVertex(glm::vec3( m_minBound.x - offset,  m_minBound.y - offset, m_maxBound.z + offset), _matrix);
    vertexData[1].m_position = CBone::_TransformVertex(glm::vec3( m_maxBound.x + offset,  m_minBound.y - offset, m_maxBound.z + offset), _matrix);
    vertexData[2].m_position = CBone::_TransformVertex(glm::vec3( m_maxBound.x + offset,  m_maxBound.y + offset, m_maxBound.z + offset), _matrix);
    vertexData[3].m_position = CBone::_TransformVertex(glm::vec3( m_minBound.x - offset,  m_maxBound.y + offset, m_maxBound.z + offset), _matrix);
    
    vertexData[4].m_position = CBone::_TransformVertex(glm::vec3( m_minBound.x - offset,  m_minBound.y - offset,  m_minBound.z - offset), _matrix);
    vertexData[5].m_position = CBone::_TransformVertex(glm::vec3( m_minBound.x - offset,  m_maxBound.y + offset,  m_minBound.z - offset), _matrix);
    vertexData[6].m_position = CBone::_TransformVertex(glm::vec3( m_maxBound.x + offset,  m_maxBound.y + offset,  m_minBound.z - offset), _matrix);
    vertexData[7].m_position = CBone::_TransformVertex(glm::vec3( m_maxBound.x + offset,  m_minBound.y - offset,  m_minBound.z - offset), _matrix);
    
    vertexData[8].m_position = CBone::_TransformVertex(glm::vec3( m_minBound.x - offset,  m_maxBound.y + offset,  m_minBound.z - offset), _matrix);
    vertexData[9].m_position = CBone::_TransformVertex(glm::vec3( m_minBound.x - offset,  m_maxBound.y + offset,  m_maxBound.z + offset), _matrix);
    vertexData[10].m_position = CBone::_TransformVertex(glm::vec3( m_maxBound.x + offset,  m_maxBound.y + offset,  m_maxBound.z + offset), _matrix);
    vertexData[11].m_position = CBone::_TransformVertex(glm::vec3( m_maxBound.x + offset,  m_maxBound.y + offset,  m_minBound.z - offset), _matrix);
    
    vertexData[12].m_position = CBone::_TransformVertex(glm::vec3( m_minBound.x - offset,  m_maxBound.y + offset,  m_minBound.z - offset), _matrix);
    vertexData[13].m_position = CBone::_TransformVertex(glm::vec3( m_maxBound.x + offset,  m_maxBound.y + offset,  m_minBound.z - offset), _matrix);
    vertexData[14].m_position = CBone::_TransformVertex(glm::vec3( m_maxBound.x + offset,  m_maxBound.y + offset,  m_maxBound.z + offset), _matrix);
    vertexData[15].m_position = CBone::_TransformVertex(glm::vec3( m_minBound.x - offset,  m_maxBound.y + offset,  m_maxBound.z + offset), _matrix);
    
    vertexData[16].m_position = CBone::_TransformVertex(glm::vec3( m_maxBound.x + offset,  m_minBound.y - offset,  m_minBound.z - offset), _matrix);
    vertexData[17].m_position = CBone::_TransformVertex(glm::vec3( m_maxBound.x + offset,  m_maxBound.y + offset,  m_minBound.z - offset), _matrix);
    vertexData[18].m_position = CBone::_TransformVertex(glm::vec3( m_maxBound.x + offset,  m_maxBound.y + offset,  m_maxBound.z + offset), _matrix);
    vertexData[19].m_position = CBone::_TransformVertex(glm::vec3( m_maxBound.x + offset,  m_minBound.y - offset,  m_maxBound.z + offset), _matrix);
    
    vertexData[20].m_position = CBone::_TransformVertex(glm::vec3( m_minBound.x - offset,  m_minBound.y - offset,  m_minBound.z - offset), _matrix);
    vertexData[21].m_position = CBone::_TransformVertex(glm::vec3( m_minBound.x - offset,  m_minBound.y - offset,  m_maxBound.z + offset), _matrix);
    vertexData[22].m_position = CBone::_TransformVertex(glm::vec3( m_minBound.x - offset,  m_maxBound.y + offset,  m_maxBound.z + offset), _matrix);
    vertexData[23].m_position = CBone::_TransformVertex(glm::vec3( m_minBound.x - offset,  m_maxBound.y + offset,  m_minBound.z - offset), _matrix);
    
    m_vertexBuffer->Unlock();
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
    for(auto iterator : m_childs)
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

/*void CBone::LinkChildBone(std::shared_ptr<CBone> _bone)
{
    std::shared_ptr<CBone> oldchild = m_child;
    m_child = _bone;
    m_child->m_next = oldchild;
    m_child->m_parent = shared_from_this();
}*/

/*std::shared_ptr<CBone> CBone::FindInChildrenById(i32 _id)
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
}*/

void CBone::Update(const glm::mat4x4 *_matrix)
{
    if(_matrix != nullptr)
    {
        (*m_transformation) = (*m_transformation) * glm::inverse((*_matrix));
    }
    for(auto iterator : m_childs)
    {
        iterator->Update(m_transformation);
    }
}
/*
void CBone::AnimateHierarhy(const glm::mat4x4* _transformation)
{
    if(_transformation != nullptr)
    {
        (*m_transformation) = (*_transformation) * (*m_transformation);
    }
    std::shared_ptr<CBone> child = m_child;
    while(child != nullptr)
    {
        child->AnimateHierarhy(m_transformation);
        child = child->Get_Next();
    }
    if(m_transformation != nullptr)
    {
        //(*m_transformation) = (*m_transformation) * m_bindPosition;
        //CBone::_Update((*m_transformation));
    }
}
*/
/*
void CBone::SetupBindPosition(void)
{
    if(m_transformation != nullptr)
    {
        Set_BindPosition(glm::inverse((*m_transformation)));
    }
    std::shared_ptr<CBone> child = m_child;
    while(child != nullptr)
    {
        child->SetupBindPosition();
        child = child->Get_Next();
    }
}
*/

i32 CBone::FillNumIndexes(void)
{
    i32 numIndexes = 1;
    for(auto iterator : m_childs)
    {
        numIndexes += iterator->FillNumIndexes();
    }
    return numIndexes + m_childs.size();
}

i32 CBone::FillIndexDataDebug(ui16 *_indexData, i32 _index, i32 _offset)
{
    i32 offset = _offset;
    for(auto iterator : m_childs)
    {
        _indexData[_offset] = _index;
        _indexData[_offset + 1] = _index + 1;
        offset = iterator->FillIndexDataDebug(_indexData, _index + 1, _offset + 2);
    }
    return offset;
}

i32 CBone::FillVertexDataDebug(SVertex *_vertexData, i32 _offset)
{
    i32 offset = _offset;
    _vertexData[offset].m_position = glm::rotate(m_rotation, m_position); //CBone::_TransformVertex(glm::vec3(0.0f, 0.0f, 0.0f), (*m_transformation));
    for(auto iterator : m_childs)
    {
        offset = iterator->FillVertexDataDebug(_vertexData, offset + 1);
    }
    return offset;
}


void CBone::DrawDebug(const i32 *_attributes)
{
    for(auto iterator : m_childs)
    {
        iterator->DrawDebug(_attributes);
    }
    
    /*std::shared_ptr<CBone> child = m_child;
    while(child != nullptr)
    {
        child->DrawDebug(_attributes);
        child = child->Get_Next();
    }*/
    
    if(m_vertexBuffer == nullptr || m_indexBuffer == nullptr)
    {
        return;
    }
    
    assert(m_vertexBuffer != nullptr);
    assert(m_indexBuffer != nullptr);
    
    if(m_transformation != nullptr)
    {
        CBone::_Update((*m_transformation));
    }
    
    m_vertexBuffer->Bind(_attributes);
    m_indexBuffer->Bind();
    glDrawElements(GL_TRIANGLES, m_indexBuffer->Get_NumIndexes(), GL_UNSIGNED_SHORT, NULL);
    m_vertexBuffer->Unbind(_attributes);
    m_indexBuffer->Unbind();
}