//
//  CHVertexBuffer.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 8/29/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CHVertexBuffer.h"
#include "HEnums.h"

CHVertexBuffer::CHVertexBuffer(ui32 _size, GLenum _mode) :
m_size(_size),
m_mode(_mode)
{
    assert(_size != 0);
    m_data = new CHVertexBuffer::SVertex[m_size];
    m_index = -1;
    glGenBuffers(k_NUM_REPLACEMENT_VERTEX_BUFFERS, m_handles);
}

CHVertexBuffer::~CHVertexBuffer(void)
{
    glDeleteBuffers(k_NUM_REPLACEMENT_VERTEX_BUFFERS, m_handles);
    delete[] m_data;
}

glm::u8vec4 CHVertexBuffer::CompressVec3(const glm::vec3& _uncompressed)
{
    glm::vec3 normalized = glm::normalize(_uncompressed);
    glm::u8vec4 compressed;
    compressed.x = static_cast<ui8>((normalized.x + 1.0f) * 0.5f * std::numeric_limits<ui8>::max());
    compressed.y = static_cast<ui8>((normalized.y + 1.0f) * 0.5f * std::numeric_limits<ui8>::max());
    compressed.z = static_cast<ui8>((normalized.z + 1.0f) * 0.5f * std::numeric_limits<ui8>::max());
    compressed.w = 0;
    return compressed;
}

glm::vec3 CHVertexBuffer::UncompressU8Vec4(const glm::u8vec4& _compressed)
{
    glm::vec3 uncompressed;
    uncompressed.x = static_cast<f32>(_compressed.x / (std::numeric_limits<ui8>::max() * 0.5f) - 1.0f);
    uncompressed.y = static_cast<f32>(_compressed.y / (std::numeric_limits<ui8>::max() * 0.5f) - 1.0f);
    uncompressed.z = static_cast<f32>(_compressed.z / (std::numeric_limits<ui8>::max() * 0.5f) - 1.0f);
    return uncompressed;
}

glm::u16vec2 CHVertexBuffer::CompressVec2(const glm::vec2& _uncompressed)
{
    glm::u16vec2 compressed;
    compressed.x = static_cast<ui16>((_uncompressed.x + 1.0f) * 0.5f * std::numeric_limits<ui16>::max());
    compressed.y = static_cast<ui16>((_uncompressed.y + 1.0f) * 0.5f * std::numeric_limits<ui16>::max());
    return compressed;
}

glm::vec2 CHVertexBuffer::UncompressU16Vec2(const glm::u16vec2& _compressed)
{
    glm::vec2 uncompressed;
    uncompressed.x = static_cast<f32>(_compressed.x / (std::numeric_limits<ui16>::max() * 0.5f) - 1.0f);
    uncompressed.y = static_cast<f32>(_compressed.y / (std::numeric_limits<ui16>::max() * 0.5f) - 1.0f);
    return uncompressed;
}

CHVertexBuffer::SVertex* CHVertexBuffer::Lock(void) const
{
    assert(m_data != nullptr);
    return m_data;
}

void CHVertexBuffer::Unlock(void)
{
    assert(m_data != nullptr);
    assert(m_size != 0);
    m_index = (m_index >= (k_NUM_REPLACEMENT_VERTEX_BUFFERS - 1)) ? 0 : m_index + 1;
    glBindBuffer(GL_ARRAY_BUFFER, m_handles[m_index]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(CHVertexBuffer::SVertex) * m_size, m_data, m_mode);
}

void CHVertexBuffer::Unlock(ui32 _size)
{
    assert(m_data != nullptr);
    assert(_size != 0 && _size <= m_size);
    m_index = (m_index >= (k_NUM_REPLACEMENT_VERTEX_BUFFERS - 1)) ? 0 : m_index + 1;
    glBindBuffer(GL_ARRAY_BUFFER, m_handles[m_index]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(CHVertexBuffer::SVertex) * _size, m_data, m_mode);
}

void CHVertexBuffer::Unlock(CSVertexBuffer::SVertex *_data, ui32 _size)
{
    assert(_data != nullptr);
    assert(m_size != 0);
    assert(m_data != nullptr);
    assert(_size != 0 && m_size >= _size);
    m_index = (m_index >= (k_NUM_REPLACEMENT_VERTEX_BUFFERS - 1)) ? 0 : m_index + 1;
    
    for(ui32 i = 0; i < _size; ++i)
    {
        m_data[i].m_position = _data[i].m_position;
        m_data[i].m_texcoord = CHVertexBuffer::CompressVec2(_data[i].m_texcoord);
        m_data[i].m_normal = CHVertexBuffer::CompressVec3(_data[i].m_normal);
        
        assert(_data[i].m_bones.size() <= 4);
        m_data[i].m_tangent = glm::u8vec4(_data[i].m_bones.size() >= 1 ? static_cast<ui8>(_data[i].m_bones[0].m_id) : 0,
                                          _data[i].m_bones.size() >= 2 ? static_cast<ui8>(_data[i].m_bones[1].m_id) : 0,
                                          _data[i].m_bones.size() >= 3 ? static_cast<ui8>(_data[i].m_bones[2].m_id) : 0,
                                          _data[i].m_bones.size() == 4 ? static_cast<ui8>(_data[i].m_bones[3].m_id) : 0);
        
        glm::u8vec4 bones = glm::u8vec4(_data[i].m_bones.size() >= 1 ? static_cast<ui8>(_data[i].m_bones[0].m_weigth * 255.0f) : 0,
                                        _data[i].m_bones.size() >= 2 ? static_cast<ui8>(_data[i].m_bones[1].m_weigth * 255.0f) : 0,
                                        _data[i].m_bones.size() >= 3 ? static_cast<ui8>(_data[i].m_bones[2].m_weigth * 255.0f) : 0,
                                        _data[i].m_bones.size() == 4 ? static_cast<ui8>(_data[i].m_bones[3].m_weigth * 255.0f) : 0);
        m_data[i].m_color = bones;
    }
    
    glBindBuffer(GL_ARRAY_BUFFER, m_handles[m_index]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(CHVertexBuffer::SVertex) * _size, m_data, m_mode);
}

void CHVertexBuffer::Bind(const i32* _attributes) const
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
        glVertexAttribPointer(attribute, 2, GL_UNSIGNED_SHORT, GL_FALSE, sizeof(SVertex), (GLvoid*)stride);
    }
    stride += sizeof(glm::u16vec2);
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
    stride += sizeof(glm::u8vec4);
    attribute = _attributes[E_SHADER_ATTRIBUTE_EXTRA];
    if(attribute >= 0)
    {
        glEnableVertexAttribArray(attribute);
        glVertexAttribPointer(attribute, 4, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(SVertex), (GLvoid*)stride);
    }
}

void CHVertexBuffer::Unbind(const i32* _attributes) const
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
    attribute = _attributes[E_SHADER_ATTRIBUTE_EXTRA];
    if(attribute >= 0)
    {
        glDisableVertexAttribArray(attribute);
    }
    glBindBuffer(GL_ARRAY_BUFFER, NULL);
}