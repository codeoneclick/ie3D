//
//  CIndexBuffer.cpp
//  gEngine-Core
//
//  Created by Sergey Sergeev on 5/7/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CIndexBuffer.h"

CIndexBuffer::CIndexBuffer(ui32 _size, GLenum _mode) :
m_size(_size),
m_mode(_mode)
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

void CIndexBuffer::Unlock(ui32 _size)
{
    assert(m_data != nullptr);
    assert(m_size != 0);
    m_index = (m_index >= (K_NUM_REPLACEMENT_INDEX_BUFFERS - 1)) ? 0 : m_index + 1;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_handles[m_index]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ui16) * (_size > 0 ? _size : m_size), m_data, m_mode);
}

void CIndexBuffer::Bind(void) const
{
    assert(m_index >= 0 && m_index <= (K_NUM_REPLACEMENT_INDEX_BUFFERS - 1));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_handles[m_index]);
}

void CIndexBuffer::Unbind(void) const
{
    assert(m_index >= 0 && m_index <= (K_NUM_REPLACEMENT_INDEX_BUFFERS - 1));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}