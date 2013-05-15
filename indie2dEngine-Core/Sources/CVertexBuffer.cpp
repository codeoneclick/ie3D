//
//  CVertexBuffer.cpp
//  gEngine-Core
//
//  Created by Sergey Sergeev on 5/7/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CVertexBuffer.h"
#include "HEnums.h"

CVertexBuffer::CVertexBuffer(ui32 _numVertexes, GLenum _mode) :
m_numVertexes(_numVertexes),
m_mode(_mode)
{
    assert(m_numVertexes != 0);
    m_data = new SVertex[m_numVertexes];
    m_currentHandleIndex = -1;
    glGenBuffers(K_NUM_REPLACEMENT_VERTEX_BUFFERS, m_handles);
}

CVertexBuffer::~CVertexBuffer(void)
{
    glDeleteBuffers(K_NUM_REPLACEMENT_VERTEX_BUFFERS, m_handles);
}

glm::u8vec4 CVertexBuffer::CompressVec3(const glm::vec3& _uncompressed)
{
    glm::vec3 normalized = glm::normalize(_uncompressed);
    glm::u8vec4 compressed;
    compressed.x = static_cast<ui8>((normalized.x + 1.0f) * 0.5f * 255.0f);
    compressed.y = static_cast<ui8>((normalized.y + 1.0f) * 0.5f * 255.0f);
    compressed.z = static_cast<ui8>((normalized.z + 1.0f) * 0.5f * 255.0f);
    compressed.w = 0;
    return compressed;
}

glm::vec3 CVertexBuffer::UncompressU8Vec4(const glm::u8vec4& _compressed)
{
    glm::vec3 uncompressed;
    uncompressed.x = static_cast<f32>(_compressed.x / (255.0f * 0.5f) - 1.0f);
    uncompressed.y = static_cast<f32>(_compressed.y / (255.0f * 0.5f) - 1.0f);
    uncompressed.z = static_cast<f32>(_compressed.z / (255.0f * 0.5f) - 1.0f);
    return uncompressed;
}

void CVertexBuffer::Unlock(void)
{
    assert(m_data != nullptr);
    assert(m_numVertexes != 0);
    m_currentHandleIndex = (m_currentHandleIndex >= (K_NUM_REPLACEMENT_VERTEX_BUFFERS - 1)) ? 0 : m_currentHandleIndex + 1;
    glBindBuffer(GL_ARRAY_BUFFER, m_handles[m_currentHandleIndex]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(SVertex) * m_numVertexes, m_data, m_mode);
}

void CVertexBuffer::Bind(const i32* _attributes)
{
    assert(m_currentHandleIndex >= 0 && m_currentHandleIndex <= (K_NUM_REPLACEMENT_VERTEX_BUFFERS - 1));
    glBindBuffer(GL_ARRAY_BUFFER, m_handles[m_currentHandleIndex]);
    ui32 stride = 0;
    i32 attribute = _attributes[E_SHADER_ATTRIBUTE_POSITION];
    if(attribute >= 0)
    {
        glEnableVertexAttribArray(attribute);
        glVertexAttribPointer(attribute, 3, GL_FLOAT, GL_FALSE, sizeof(SVertex), (GLvoid*)stride);
    }
    stride += sizeof(glm::vec3);
    attribute = _attributes[E_SHADER_ATTRIBUTE_TEXCOORD];
    if(attribute >= 0)
    {
        glEnableVertexAttribArray(attribute);
        glVertexAttribPointer(attribute, 2, GL_FLOAT, GL_FALSE, sizeof(SVertex), (GLvoid*)stride);
    }
    stride += sizeof(glm::vec2);
    attribute = _attributes[E_SHADER_ATTRIBUTE_NORMAL];
    if(attribute >= 0)
    {
        glEnableVertexAttribArray(attribute);
        glVertexAttribPointer(attribute, 4, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(SVertex), (GLvoid*)stride);
    }
    stride += sizeof(glm::u8vec4);
    attribute = _attributes[E_SHADER_ATTRIBUTE_TANGENT];
    if(attribute >= 0)
    {
        glEnableVertexAttribArray(attribute);
        glVertexAttribPointer(attribute, 4, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(SVertex), (GLvoid*)stride);
    }
    stride += sizeof(glm::u8vec4);
    attribute = _attributes[E_SHADER_ATTRIBUTE_COLOR];
    if(attribute >= 0)
    {
        glEnableVertexAttribArray(attribute);
        glVertexAttribPointer(attribute, 4, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(SVertex), (GLvoid*)stride);
    }
}

void CVertexBuffer::Unbind(const i32* _attributes)
{
    assert(m_currentHandleIndex >= 0 && m_currentHandleIndex <= (K_NUM_REPLACEMENT_VERTEX_BUFFERS - 1));
    glBindBuffer(GL_ARRAY_BUFFER, m_handles[m_currentHandleIndex]);
    i32 attribute = _attributes[E_SHADER_ATTRIBUTE_POSITION];
    if(attribute >= 0)
    {
        glDisableVertexAttribArray(attribute);
    }
    attribute = _attributes[E_SHADER_ATTRIBUTE_TEXCOORD];
    if(attribute >= 0)
    {
        glDisableVertexAttribArray(attribute);
    }
    attribute = _attributes[E_SHADER_ATTRIBUTE_NORMAL];
    if(attribute >= 0)
    {
        glDisableVertexAttribArray(attribute);
    }
    attribute = _attributes[E_SHADER_ATTRIBUTE_TANGENT];
    if(attribute >= 0)
    {
        glDisableVertexAttribArray(attribute);
    }
    attribute = _attributes[E_SHADER_ATTRIBUTE_COLOR];
    if(attribute >= 0)
    {
        glDisableVertexAttribArray(attribute);
    }
    glBindBuffer(GL_ARRAY_BUFFER, NULL);
}