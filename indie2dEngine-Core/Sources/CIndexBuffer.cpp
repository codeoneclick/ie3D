//
//  CIndexBuffer.cpp
//  gEngine-Core
//
//  Created by Sergey Sergeev on 5/7/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CIndexBuffer.h"

CIndexBuffer::CIndexBuffer(ui32 _numIndexes, GLenum _mode) :
m_numIndexes(_numIndexes),
m_mode(_mode)
{
	assert(m_numIndexes != 0);
	m_data = new ui16[m_numIndexes];
    m_currentHandleIndex = -1;
    glGenBuffers(K_NUM_REPLACEMENT_INDEX_BUFFERS, m_handles);
}

CIndexBuffer::~CIndexBuffer(void)
{
    glDeleteBuffers(K_NUM_REPLACEMENT_INDEX_BUFFERS, m_handles);
}

void CIndexBuffer::Unlock(void)
{
    assert(m_data != nullptr);
    assert(m_numIndexes != 0);
    m_currentHandleIndex = (m_currentHandleIndex >= (K_NUM_REPLACEMENT_INDEX_BUFFERS - 1)) ? 0 : m_currentHandleIndex + 1;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_handles[m_currentHandleIndex]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ui16) * m_numIndexes, m_data, m_mode);
}

void CIndexBuffer::Bind(void)
{
    assert(m_currentHandleIndex >= 0 && m_currentHandleIndex <= (K_NUM_REPLACEMENT_INDEX_BUFFERS - 1));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_handles[m_currentHandleIndex]);
}

void CIndexBuffer::Unbind(void)
{
    assert(m_currentHandleIndex >= 0 && m_currentHandleIndex <= (K_NUM_REPLACEMENT_INDEX_BUFFERS - 1));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
