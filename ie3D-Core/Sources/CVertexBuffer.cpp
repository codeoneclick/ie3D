//
//  CVertexBuffer.cpp
//  gEngine-Core
//
//  Created by Sergey Sergeev on 5/7/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CVertexBuffer.h"
#include "HEnums.h"

CVertexBuffer::CVertexBuffer(ui32 sizeToAllocate, GLenum mode) : IResourceData(E_RESOURCE_DATA_CLASS_VERTEX_BUFFER_DATA),
m_allocatedSize(sizeToAllocate),
m_usedSize(0),
m_mode(mode),
m_isDataUploaded(false)
{
    assert(m_allocatedSize != 0);
    m_data = new SAttributeVertex[m_allocatedSize];
    glGenBuffers(1, &m_handle);
    
#if defined(__OPENGL_30__)
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
#endif
}

CVertexBuffer::~CVertexBuffer(void)
{
    glDeleteBuffers(1, &m_handle);
#if defined(__OPENGL_30__)
    glDeleteVertexArrays(1, &m_vao);
#endif
    delete[] m_data;
}

ui32 CVertexBuffer::getAllocatedSize(void) const
{
    return m_allocatedSize;
}

ui32 CVertexBuffer::getUsedSize(void) const
{
    return m_usedSize;
}

SAttributeVertex* CVertexBuffer::lock(void) const
{
    assert(m_data != nullptr);
    return m_data;
}

void CVertexBuffer::unlock(ui32 sizeToUse)
{
    assert(m_data != nullptr);
    assert(m_allocatedSize != 0);
    m_usedSize = sizeToUse > 0 && sizeToUse < m_allocatedSize ? sizeToUse : m_allocatedSize;
    glBindBuffer(GL_ARRAY_BUFFER, m_handle);
    
    if(!m_isDataUploaded)
    {
        glBufferData(GL_ARRAY_BUFFER, sizeof(SAttributeVertex) * m_usedSize, m_data, m_mode);
        //m_isDataUploaded = true;
    }
    else
    {
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(SAttributeVertex) * m_usedSize, m_data);
    }
    GLenum error = glGetError();
    assert(error == GL_NO_ERROR);
}

void CVertexBuffer::bind(const std::array<i32, E_SHADER_ATTRIBUTE_MAX>& attributes) const
{
    if(m_usedSize != 0)
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_handle);
#if defined(__OPENGL_30__)
        glBindVertexArray(m_vao);
#endif
        if(attributes.at(E_SHADER_ATTRIBUTE_POSITION) >= 0)
        {
            glEnableVertexAttribArray(attributes.at(E_SHADER_ATTRIBUTE_POSITION));
            glVertexAttribPointer(attributes.at(E_SHADER_ATTRIBUTE_POSITION), 3, GL_FLOAT, GL_FALSE,
                                  sizeof(SAttributeVertex),
                                  (GLvoid*)offsetof(SAttributeVertex, m_position));
        }
        if(attributes.at(E_SHADER_ATTRIBUTE_TEXCOORD) >= 0)
        {
            glEnableVertexAttribArray(attributes.at(E_SHADER_ATTRIBUTE_TEXCOORD));
            glVertexAttribPointer(attributes.at(E_SHADER_ATTRIBUTE_TEXCOORD), 2, GL_UNSIGNED_SHORT, GL_TRUE,
                                  sizeof(SAttributeVertex),
                                  (GLvoid*)offsetof(SAttributeVertex, m_texcoord));
        }
        if(attributes.at(E_SHADER_ATTRIBUTE_NORMAL) >= 0)
        {
            glEnableVertexAttribArray(attributes.at(E_SHADER_ATTRIBUTE_NORMAL));
            glVertexAttribPointer(attributes.at(E_SHADER_ATTRIBUTE_NORMAL), 4, GL_BYTE, GL_TRUE,
                                  sizeof(SAttributeVertex),
                                  (GLvoid*)offsetof(SAttributeVertex, m_normal));
        }
        if(attributes[E_SHADER_ATTRIBUTE_TANGENT] >= 0)
        {
            glEnableVertexAttribArray(attributes.at(E_SHADER_ATTRIBUTE_TANGENT));
            glVertexAttribPointer(attributes.at(E_SHADER_ATTRIBUTE_TANGENT), 4, GL_BYTE, GL_TRUE,
                                  sizeof(SAttributeVertex),
                                  (GLvoid*)offsetof(SAttributeVertex, m_tangent));
        }
        if(attributes.at(E_SHADER_ATTRIBUTE_COLOR) >= 0)
        {
            glEnableVertexAttribArray(attributes.at(E_SHADER_ATTRIBUTE_COLOR));
            glVertexAttribPointer(attributes.at(E_SHADER_ATTRIBUTE_COLOR), 4, GL_UNSIGNED_BYTE, GL_FALSE,
                                  sizeof(SAttributeVertex),
                                  (GLvoid*)offsetof(SAttributeVertex, m_color));
        }
        if(attributes.at(E_SHADER_ATTRIBUTE_EXTRA) >= 0)
        {
            glEnableVertexAttribArray(attributes.at(E_SHADER_ATTRIBUTE_EXTRA));
            glVertexAttribPointer(attributes.at(E_SHADER_ATTRIBUTE_EXTRA), 4, GL_UNSIGNED_BYTE, GL_FALSE,
                                  sizeof(SAttributeVertex),
                                  (GLvoid*)offsetof(SAttributeVertex, m_extra));
        }
    }
}

void CVertexBuffer::unbind(const std::array<i32, E_SHADER_ATTRIBUTE_MAX>& attributes) const
{
    if(m_usedSize != 0)
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_handle);
        if(attributes.at(E_SHADER_ATTRIBUTE_POSITION) >= 0)
        {
            glDisableVertexAttribArray(attributes.at(E_SHADER_ATTRIBUTE_POSITION));
        }
        if(attributes.at(E_SHADER_ATTRIBUTE_TEXCOORD) >= 0)
        {
            glDisableVertexAttribArray(attributes.at(E_SHADER_ATTRIBUTE_TEXCOORD));
        }
        if(attributes.at(E_SHADER_ATTRIBUTE_NORMAL) >= 0)
        {
            glDisableVertexAttribArray(attributes.at(E_SHADER_ATTRIBUTE_NORMAL));
        }
        if(attributes.at(E_SHADER_ATTRIBUTE_TANGENT) >= 0)
        {
            glDisableVertexAttribArray(attributes.at(E_SHADER_ATTRIBUTE_TANGENT));
        }
        if(attributes.at(E_SHADER_ATTRIBUTE_COLOR) >= 0)
        {
            glDisableVertexAttribArray(attributes.at(E_SHADER_ATTRIBUTE_COLOR));
        }
        if(attributes.at(E_SHADER_ATTRIBUTE_EXTRA) >= 0)
        {
            glDisableVertexAttribArray(attributes.at(E_SHADER_ATTRIBUTE_EXTRA));
        }
        glBindBuffer(GL_ARRAY_BUFFER, NULL);
    }
}
