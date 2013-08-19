//
//  CVertexBuffer.cpp
//  gEngine-Core
//
//  Created by Sergey Sergeev on 5/7/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CVertexBuffer.h"
#include "HEnums.h"

CVertexBuffer::CVertexBuffer(ui32 _size, GLenum _mode) :
m_size(_size),
m_mode(_mode)
{
    assert(_size != 0);
    m_main = new SVertex[m_size];
    m_index = -1;
    glGenBuffers(k_NUM_REPLACEMENT_VERTEX_BUFFERS, m_handles);
}

CVertexBuffer::~CVertexBuffer(void)
{
    glDeleteBuffers(k_NUM_REPLACEMENT_VERTEX_BUFFERS, m_handles);
    for(auto reference : m_references)
    {
        delete[] reference.second;
    }
    m_references.clear();
    delete[] m_main;
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

const std::string CVertexBuffer::_GenerateGuid(void)
{
    static ui32 value = 0;
    std::stringstream stringstream;
    stringstream<<"vertex.buffer.reference."<<++value;
    return stringstream.str();
}

const std::string CVertexBuffer::CreateReference(void)
{
    assert(m_size != 0);
    std::string guid = CVertexBuffer::_GenerateGuid();
    m_references.insert(std::make_pair(guid, new SVertex[m_size]));
    return guid;
}

void CVertexBuffer::DeleteReference(const std::string &_guid)
{
    assert(m_references.find(_guid) != m_references.end());
    m_references.erase(m_references.find(_guid));
}

SVertex* CVertexBuffer::Lock(void) const
{
    assert(m_main != nullptr);
    return m_main;
};

void CVertexBuffer::Unlock(void)
{
    assert(m_main != nullptr);
    assert(m_size != 0);
    m_index = (m_index >= (k_NUM_REPLACEMENT_VERTEX_BUFFERS - 1)) ? 0 : m_index + 1;
    glBindBuffer(GL_ARRAY_BUFFER, m_handles[m_index]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(SVertex) * m_size, m_main, m_mode);
}

SVertex* CVertexBuffer::Lock(const std::string &_guid) const
{
    assert(m_references.find(_guid) !=  m_references.end());
    return m_references.find(_guid)->second;
}

void CVertexBuffer::Unlock(const std::string &_guid)
{
    assert(m_references.find(_guid) !=  m_references.end());
    m_main = m_references.find(_guid)->second;
    CVertexBuffer::Unlock();
}

void CVertexBuffer::Bind(const i32* _attributes)
{
    assert(m_index >= 0 && m_index <= (k_NUM_REPLACEMENT_VERTEX_BUFFERS - 1));
    glBindBuffer(GL_ARRAY_BUFFER, m_handles[m_index]);
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
    assert(m_index >= 0 && m_index <= (k_NUM_REPLACEMENT_VERTEX_BUFFERS - 1));
    glBindBuffer(GL_ARRAY_BUFFER, m_handles[m_index]);
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