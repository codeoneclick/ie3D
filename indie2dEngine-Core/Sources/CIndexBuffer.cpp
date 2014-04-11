//
//  CIndexBuffer.cpp
//  gEngine-Core
//
//  Created by Sergey Sergeev on 5/7/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CIndexBuffer.h"

CIndexBuffer::CIndexBuffer(ui32 size, GLenum mode) : IResourceData(E_RESOURCE_DATA_CLASS_INDEX_BUFFER_DATA)
m_size(size),
m_mode(mode)
{
	assert(m_size != 0);
	m_data = new ui16[m_size];
    m_index = -1;
    glGenBuffers(K_NUM_REPLACEMENT_INDEX_BUFFERS, m_handles);
}

CIndexBuffer::~CIndexBuffer(void)
{
    glDeleteBuffers(K_NUM_REPLACEMENT_INDEX_BUFFERS, m_handles);
}

ui16* CIndexBuffer::lock(void) const
{
    return m_data;
}

void CIndexBuffer::unlock(ui32 size)
{
    assert(m_data != nullptr);
    assert(m_size != 0);
    m_index = (m_index >= (K_NUM_REPLACEMENT_INDEX_BUFFERS - 1)) ? 0 : m_index + 1;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_handles[m_index]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ui16) * (size > 0 ? size : m_size), m_data, m_mode);
}

void CIndexBuffer::bind(void) const
{
    assert(m_index >= 0 && m_index <= (K_NUM_REPLACEMENT_INDEX_BUFFERS - 1));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_handles[m_index]);
}

void CIndexBuffer::unbind(void) const
{
    assert(m_index >= 0 && m_index <= (K_NUM_REPLACEMENT_INDEX_BUFFERS - 1));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}