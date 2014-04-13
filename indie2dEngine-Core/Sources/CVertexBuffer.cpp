//
//  CVertexBuffer.cpp
//  gEngine-Core
//
//  Created by Sergey Sergeev on 5/7/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CVertexBuffer.h"
#include "HEnums.h"

CVertexBuffer::CVertexBuffer(ui32 size, GLenum mode) : IResourceData(E_RESOURCE_DATA_CLASS_VERTEX_BUFFER_DATA),
m_size(size),
m_mode(mode)
{
    assert(size != 0);
    m_data = new SAttributeVertex[m_size];
    m_index = -1;
    glGenBuffers(k_NUM_REPLACEMENT_VERTEX_BUFFERS, m_handles);
}

CVertexBuffer::~CVertexBuffer(void)
{
    glDeleteBuffers(k_NUM_REPLACEMENT_VERTEX_BUFFERS, m_handles);
    delete[] m_data;
}

glm::u8vec4 CVertexBuffer::compressVec3(const glm::vec3& uncompressed)
{
    glm::vec3 normalized = glm::normalize(uncompressed);
    glm::u8vec4 compressed;
    compressed.x = static_cast<ui8>((normalized.x + 1) * 0.5 * 255);
    compressed.y = static_cast<ui8>((normalized.y + 1) * 0.5 * 255);
    compressed.z = static_cast<ui8>((normalized.z + 1) * 0.5 * 255);
    compressed.w = 0;
    return compressed;
}

glm::vec3 CVertexBuffer::uncompressU8Vec4(const glm::u8vec4& compressed)
{
    glm::vec3 uncompressed;
    uncompressed.x = static_cast<f32>(compressed.x / (255 * 0.5) - 1);
    uncompressed.y = static_cast<f32>(compressed.y / (255 * 0.5) - 1);
    uncompressed.z = static_cast<f32>(compressed.z / (255 * 0.5) - 1);
    return uncompressed;
}

glm::u16vec2 CVertexBuffer::compressVec2(const glm::vec2& uncompressed)
{
    glm::u16vec2 compressed;
    compressed.x = static_cast<ui16>((uncompressed.x + 1) * 0.5 * 65535);
    compressed.y = static_cast<ui16>((uncompressed.y + 1) * 0.5 * 65535);
    return compressed;
}

glm::vec2 CVertexBuffer::uncompressU16Vec2(const glm::u16vec2& compressed)
{
    glm::vec2 uncompressed;
    uncompressed.x = static_cast<f32>(compressed.x / (65535 * 0.5) - 1);
    uncompressed.y = static_cast<f32>(compressed.y / (65535 * 0.5) - 1);
    return uncompressed;
}

SAttributeVertex* CVertexBuffer::lock(void) const
{
    assert(m_data != nullptr);
    return m_data;
}

void CVertexBuffer::unlock(ui32 size)
{
    assert(m_data != nullptr);
    assert(m_size != 0);
    
    m_index = (m_index >= (k_NUM_REPLACEMENT_VERTEX_BUFFERS - 1)) ? 0 : m_index + 1;
    glBindBuffer(GL_ARRAY_BUFFER, m_handles[m_index]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(SAttributeVertex) * (size > 0 ? size : m_size) , m_data, m_mode);
}

void CVertexBuffer::bind(const i32* attributes) const
{
    assert(m_index >= 0 && m_index <= (k_NUM_REPLACEMENT_VERTEX_BUFFERS - 1));
    glBindBuffer(GL_ARRAY_BUFFER, m_handles[m_index]);
    if(attributes[E_SHADER_ATTRIBUTE_POSITION] >= 0)
    {
        glEnableVertexAttribArray(attributes[E_SHADER_ATTRIBUTE_POSITION]);
        glVertexAttribPointer(attributes[E_SHADER_ATTRIBUTE_POSITION], 3, GL_FLOAT, GL_FALSE,
                              sizeof(SAttributeVertex),
                              (GLvoid*)offsetof(SAttributeVertex, m_position));
    }
    if(attributes[E_SHADER_ATTRIBUTE_TEXCOORD] >= 0)
    {
        glEnableVertexAttribArray(attributes[E_SHADER_ATTRIBUTE_TEXCOORD]);
        glVertexAttribPointer(attributes[E_SHADER_ATTRIBUTE_TEXCOORD], 2, GL_UNSIGNED_SHORT, GL_FALSE,
                              sizeof(SAttributeVertex),
                              (GLvoid*)offsetof(SAttributeVertex, m_texcoord));
    }
    if(attributes[E_SHADER_ATTRIBUTE_NORMAL] >= 0)
    {
        glEnableVertexAttribArray(attributes[E_SHADER_ATTRIBUTE_NORMAL]);
        glVertexAttribPointer(attributes[E_SHADER_ATTRIBUTE_NORMAL], 4, GL_UNSIGNED_BYTE, GL_FALSE,
                              sizeof(SAttributeVertex),
                              (GLvoid*)offsetof(SAttributeVertex, m_normal));
    }
    if(attributes[E_SHADER_ATTRIBUTE_TANGENT] >= 0)
    {
        glEnableVertexAttribArray(attributes[E_SHADER_ATTRIBUTE_TANGENT]);
        glVertexAttribPointer(attributes[E_SHADER_ATTRIBUTE_TANGENT], 4, GL_UNSIGNED_BYTE, GL_FALSE,
                              sizeof(SAttributeVertex),
                              (GLvoid*)offsetof(SAttributeVertex, m_tangent));
    }
    if(attributes[E_SHADER_ATTRIBUTE_COLOR] >= 0)
    {
        glEnableVertexAttribArray(attributes[E_SHADER_ATTRIBUTE_COLOR]);
        glVertexAttribPointer(attributes[E_SHADER_ATTRIBUTE_COLOR], 4, GL_UNSIGNED_BYTE, GL_FALSE,
                              sizeof(SAttributeVertex),
                              (GLvoid*)offsetof(SAttributeVertex, m_color));
    }
    if(attributes[E_SHADER_ATTRIBUTE_EXTRA] >= 0)
    {
        glEnableVertexAttribArray(attributes[E_SHADER_ATTRIBUTE_EXTRA]);
        glVertexAttribPointer(attributes[E_SHADER_ATTRIBUTE_EXTRA], 4, GL_UNSIGNED_BYTE, GL_FALSE,
                              sizeof(SAttributeVertex),
                              (GLvoid*)offsetof(SAttributeVertex, m_extra));
    }
}

void CVertexBuffer::unbind(const i32* attributes) const
{
    assert(m_index >= 0 && m_index <= (k_NUM_REPLACEMENT_VERTEX_BUFFERS - 1));
    glBindBuffer(GL_ARRAY_BUFFER, m_handles[m_index]);
    if(attributes[E_SHADER_ATTRIBUTE_POSITION] >= 0)
    {
        glDisableVertexAttribArray(attributes[E_SHADER_ATTRIBUTE_POSITION]);
    }
    if(attributes[E_SHADER_ATTRIBUTE_TEXCOORD] >= 0)
    {
        glDisableVertexAttribArray(attributes[E_SHADER_ATTRIBUTE_TEXCOORD]);
    }
    if(attributes[E_SHADER_ATTRIBUTE_NORMAL] >= 0)
    {
        glDisableVertexAttribArray(attributes[E_SHADER_ATTRIBUTE_NORMAL]);
    }
    if(attributes[E_SHADER_ATTRIBUTE_TANGENT] >= 0)
    {
        glDisableVertexAttribArray(attributes[E_SHADER_ATTRIBUTE_TANGENT]);
    }
    if(attributes[E_SHADER_ATTRIBUTE_COLOR] >= 0)
    {
        glDisableVertexAttribArray(attributes[E_SHADER_ATTRIBUTE_COLOR]);
    }
    if(attributes[E_SHADER_ATTRIBUTE_EXTRA] >= 0)
    {
        glDisableVertexAttribArray(attributes[E_SHADER_ATTRIBUTE_EXTRA]);
    }
    glBindBuffer(GL_ARRAY_BUFFER, NULL);
}
