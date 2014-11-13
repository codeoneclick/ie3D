//
//  CIndexBuffer.cpp
//  gEngine-Core
//
//  Created by Sergey Sergeev on 5/7/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CIndexBuffer.h"

CIndexBuffer::CIndexBuffer(ui32 sizeToAllocate, GLenum mode) : IResourceData(E_RESOURCE_DATA_CLASS_INDEX_BUFFER_DATA),
m_allocatedSize(sizeToAllocate),
m_usedSize(0),
m_mode(mode),
m_isDataUploaded(false)
{
	assert(m_allocatedSize != 0);
	m_data = new ui16[m_allocatedSize];
    glGenBuffers(1, &m_handle);
}

CIndexBuffer::~CIndexBuffer(void)
{
    glDeleteBuffers(1, &m_handle);
    delete[] m_data;
}

ui32 CIndexBuffer::getAllocatedSize(void) const
{
    return m_allocatedSize;
}

ui32 CIndexBuffer::getUsedSize(void) const
{
    return m_usedSize;
}

ui16* CIndexBuffer::lock(void) const
{
    return m_data;
}

void CIndexBuffer::unlock(ui32 sizeToUse)
{
    assert(m_data != nullptr);
    assert(m_allocatedSize != 0);
    m_usedSize = sizeToUse > 0 && sizeToUse < m_allocatedSize ? sizeToUse : m_allocatedSize;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_handle);
    if(!m_isDataUploaded || m_usedSize == m_allocatedSize)
    {
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ui16) * m_usedSize, m_data, m_mode);
        m_isDataUploaded = true;
    }
    else
    {
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(ui16) * m_usedSize, m_data);
    }
}

void CIndexBuffer::bind(void) const
{
    if(m_usedSize != 0)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_handle);
    }
}

void CIndexBuffer::unbind(void) const
{
    /*if(m_usedSize != 0)
    {
        assert(m_index >= 0 && m_index <= (K_NUM_REPLACEMENT_INDEX_BUFFERS - 1));
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }*/
}