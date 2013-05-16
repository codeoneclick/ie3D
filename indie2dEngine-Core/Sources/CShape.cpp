//
//  CShape.cpp
//  gEngine-Core
//
//  Created by Sergey Sergeev on 5/7/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CShape.h"

CShape::CShape(void) :
m_vertexBuffer(nullptr),
m_indexBuffer(nullptr),
m_maxBound(glm::vec2(-4096.0f, -4096.0f)),
m_minBound(glm::vec2( 4096.0f,  4096.0f))
{
    
}

CShape::~CShape(void)
{
    delete m_vertexBuffer;
    delete m_indexBuffer;
}

void CShape::Link(CVertexBuffer* _vertexBuffer, CIndexBuffer* _indexBuffer)
{
    assert(_vertexBuffer != nullptr);
    assert(_indexBuffer != nullptr);
    
    m_vertexBuffer = _vertexBuffer;
    m_indexBuffer = _indexBuffer;
    
    SVertex* vertexData = m_vertexBuffer->Lock();
    
    for(ui32 i = 0; i < m_vertexBuffer->Get_NumVertexes(); ++i)
    {
        if(vertexData[i].m_position.x > m_maxBound.x)
        {
            m_maxBound.x = vertexData[i].m_position.x;
        }
        if(vertexData[i].m_position.y > m_maxBound.y)
        {
            m_maxBound.y = vertexData[i].m_position.y;
        }
        if(vertexData[i].m_position.x < m_minBound.x)
        {
            m_minBound.x = vertexData[i].m_position.x;
        }
        if(vertexData[i].m_position.y < m_minBound.y)
        {
            m_minBound.y = vertexData[i].m_position.y;
        }
    }
}

void CShape::Link(CVertexBuffer* _vertexBuffer, CIndexBuffer* _indexBuffer, const glm::vec2 &_maxBound, const glm::vec2 &_minBound)
{
    assert(_vertexBuffer != nullptr);
    assert(_indexBuffer != nullptr);
    
    m_vertexBuffer = _vertexBuffer;
    m_indexBuffer = _indexBuffer;
    
    m_maxBound = _maxBound;
    m_minBound = _minBound;
}

void CShape::Bind(const i32* _attributes)
{
    assert(m_vertexBuffer != nullptr);
    assert(m_indexBuffer != nullptr);
    m_vertexBuffer->Bind(_attributes);
    m_indexBuffer->Bind();
}

void CShape::Draw(void)
{
    assert(m_vertexBuffer != nullptr);
    assert(m_indexBuffer != nullptr);
    glDrawElements(GL_TRIANGLES, m_indexBuffer->Get_NumIndexes(), GL_UNSIGNED_SHORT, NULL);
}

void CShape::Unbind(const i32* _attributes)
{
    assert(m_vertexBuffer != nullptr);
    assert(m_indexBuffer != nullptr);
    m_vertexBuffer->Unbind(_attributes);
    m_indexBuffer->Unbind();
}