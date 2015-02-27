//
//  CVertexArrayBuffer.cpp
//  ie3D-Core
//
//  Created by sergey.sergeev on 2/26/15.
//  Copyright (c) 2015 Sergey Sergeev. All rights reserved.
//

#include "CVertexArrayBuffer.h"
#include "CVertexBuffer.h"
#include "CIndexBuffer.h"

CVertexArrayBuffer::CVertexArrayBuffer(CSharedVertexBufferRef vertexBuffer,
                                       CSharedIndexBufferRef indexBuffer) :
m_vertexBuffer(vertexBuffer),
m_indexBuffer(indexBuffer)
{
    ieGenVertexArrays(1, &m_vao);
    ieBindVertexArray(m_vao);
}

CVertexArrayBuffer::~CVertexArrayBuffer(void)
{
    ieDeleteVertexArrays(1, &m_vao);
}

void CVertexArrayBuffer::init(const std::array<i32, E_SHADER_ATTRIBUTE_MAX>& attributes)
{
    assert(m_vertexBuffer != nullptr);
    assert(m_indexBuffer != nullptr);
    
    ieBindVertexArray(m_vao);
    
    m_vertexBuffer->bind(attributes);
    m_indexBuffer->bind();
    
    CVertexArrayBuffer::unbind();
}

void CVertexArrayBuffer::bind(CSharedVertexArrayBufferRef vaoState)
{
    ieBindVertexArray(vaoState->m_vao);
}

void CVertexArrayBuffer::unbind(void)
{
    ieBindVertexArray(NULL);
}