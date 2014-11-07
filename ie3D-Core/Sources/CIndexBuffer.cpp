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
m_mode(mode)
{
	assert(m_allocatedSize != 0);
	m_data = new ui16[m_allocatedSize];
    m_index = -1;
    glGenBuffers(K_NUM_REPLACEMENT_INDEX_BUFFERS, m_handles);
}

CIndexBuffer::~CIndexBuffer(void)
{
    glDeleteBuffers(K_NUM_REPLACEMENT_INDEX_BUFFERS, m_handles);
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
    m_index = (m_index >= (K_NUM_REPLACEMENT_INDEX_BUFFERS - 1)) ? 0 : m_index + 1;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_handles[m_index]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ui16) * m_usedSize, m_data, m_mode);
}

void CIndexBuffer::bind(void) const
{
    if(m_usedSize != 0)
    {
        assert(m_index >= 0 && m_index <= (K_NUM_REPLACEMENT_INDEX_BUFFERS - 1));
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_handles[m_index]);
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