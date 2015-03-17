//
//  CVertexBuffer.cpp
//  gEngine-Core
//
//  Created by Sergey Sergeev on 5/7/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CVertexBuffer.h"
#include "HEnums.h"

CVertexBuffer::CVertexBuffer(ui32 sizeToAllocate, GLenum mode, ui16* mmap) : IResourceData(E_RESOURCE_DATA_CLASS_VERTEX_BUFFER_DATA),
m_allocatedSize(sizeToAllocate),
m_usedSize(0),
m_mode(mode),
m_isMMAP(false)
{
    assert(m_allocatedSize != 0);
    m_data = new SAttributeVertex[m_allocatedSize];
    memset(m_data, 0x0, sizeof(SAttributeVertex) * m_allocatedSize);
    ieGenBuffers(1, &m_handle);
    ieBindBuffer(GL_ARRAY_BUFFER, m_handle);
    ieBufferData(GL_ARRAY_BUFFER, sizeof(SAttributeVertex) * m_allocatedSize, m_data, m_mode);
}

CVertexBuffer::~CVertexBuffer(void)
{
    ieDeleteBuffers(1, &m_handle);
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
    ieBindBuffer(GL_ARRAY_BUFFER, m_handle);
    
    //if(!m_isDataUploaded)
    //{
    //    ieBufferData(GL_ARRAY_BUFFER, sizeof(SAttributeVertex) * m_usedSize, m_data, m_mode);
        //m_isDataUploaded = true;
    //}
    //else
    {
        ieBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(SAttributeVertex) * m_usedSize, m_data);
    }
}

void CVertexBuffer::bind(const std::array<i32, E_SHADER_ATTRIBUTE_MAX>& attributes) const
{
    if(m_usedSize != 0)
    {
        ieBindBuffer(GL_ARRAY_BUFFER, m_handle);
        if(attributes.at(E_SHADER_ATTRIBUTE_POSITION) >= 0)
        {
            ieEnableVertexAttribArray(attributes.at(E_SHADER_ATTRIBUTE_POSITION));
            ieVertexAttribPointer(attributes.at(E_SHADER_ATTRIBUTE_POSITION), 3, GL_FLOAT, GL_FALSE,
                                  sizeof(SAttributeVertex),
                                  (GLvoid*)offsetof(SAttributeVertex, m_position));
        }
        if(attributes.at(E_SHADER_ATTRIBUTE_TEXCOORD) >= 0)
        {
            ieEnableVertexAttribArray(attributes.at(E_SHADER_ATTRIBUTE_TEXCOORD));
            ieVertexAttribPointer(attributes.at(E_SHADER_ATTRIBUTE_TEXCOORD), 2, GL_UNSIGNED_SHORT, GL_TRUE,
                                  sizeof(SAttributeVertex),
                                  (GLvoid*)offsetof(SAttributeVertex, m_texcoord));
        }
        if(attributes.at(E_SHADER_ATTRIBUTE_NORMAL) >= 0)
        {
            ieEnableVertexAttribArray(attributes.at(E_SHADER_ATTRIBUTE_NORMAL));
            ieVertexAttribPointer(attributes.at(E_SHADER_ATTRIBUTE_NORMAL), 4, GL_BYTE, GL_TRUE,
                                  sizeof(SAttributeVertex),
                                  (GLvoid*)offsetof(SAttributeVertex, m_normal));
        }
        if(attributes[E_SHADER_ATTRIBUTE_TANGENT] >= 0)
        {
            ieEnableVertexAttribArray(attributes.at(E_SHADER_ATTRIBUTE_TANGENT));
            ieVertexAttribPointer(attributes.at(E_SHADER_ATTRIBUTE_TANGENT), 4, GL_BYTE, GL_TRUE,
                                  sizeof(SAttributeVertex),
                                  (GLvoid*)offsetof(SAttributeVertex, m_tangent));
        }
        if(attributes.at(E_SHADER_ATTRIBUTE_COLOR) >= 0)
        {
            ieEnableVertexAttribArray(attributes.at(E_SHADER_ATTRIBUTE_COLOR));
            ieVertexAttribPointer(attributes.at(E_SHADER_ATTRIBUTE_COLOR), 4, GL_UNSIGNED_BYTE, GL_FALSE,
                                  sizeof(SAttributeVertex),
                                  (GLvoid*)offsetof(SAttributeVertex, m_color));
        }
        if(attributes.at(E_SHADER_ATTRIBUTE_EXTRA) >= 0)
        {
            ieEnableVertexAttribArray(attributes.at(E_SHADER_ATTRIBUTE_EXTRA));
            ieVertexAttribPointer(attributes.at(E_SHADER_ATTRIBUTE_EXTRA), 4, GL_UNSIGNED_BYTE, GL_FALSE,
                                  sizeof(SAttributeVertex),
                                  (GLvoid*)offsetof(SAttributeVertex, m_extra));
        }
    }
}

void CVertexBuffer::unbind(const std::array<i32, E_SHADER_ATTRIBUTE_MAX>& attributes) const
{
    if(m_usedSize != 0)
    {
        ieBindBuffer(GL_ARRAY_BUFFER, m_handle);
        if(attributes.at(E_SHADER_ATTRIBUTE_POSITION) >= 0)
        {
            ieDisableVertexAttribArray(attributes.at(E_SHADER_ATTRIBUTE_POSITION));
        }
        if(attributes.at(E_SHADER_ATTRIBUTE_TEXCOORD) >= 0)
        {
            ieDisableVertexAttribArray(attributes.at(E_SHADER_ATTRIBUTE_TEXCOORD));
        }
        if(attributes.at(E_SHADER_ATTRIBUTE_NORMAL) >= 0)
        {
            ieDisableVertexAttribArray(attributes.at(E_SHADER_ATTRIBUTE_NORMAL));
        }
        if(attributes.at(E_SHADER_ATTRIBUTE_TANGENT) >= 0)
        {
            ieDisableVertexAttribArray(attributes.at(E_SHADER_ATTRIBUTE_TANGENT));
        }
        if(attributes.at(E_SHADER_ATTRIBUTE_COLOR) >= 0)
        {
            ieDisableVertexAttribArray(attributes.at(E_SHADER_ATTRIBUTE_COLOR));
        }
        if(attributes.at(E_SHADER_ATTRIBUTE_EXTRA) >= 0)
        {
            ieDisableVertexAttribArray(attributes.at(E_SHADER_ATTRIBUTE_EXTRA));
        }
        ieBindBuffer(GL_ARRAY_BUFFER, NULL);
    }
}
