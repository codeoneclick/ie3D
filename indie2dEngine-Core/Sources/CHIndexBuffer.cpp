//
//  CHIndexBuffer.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 8/29/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CHIndexBuffer.h"

CHIndexBuffer::CHIndexBuffer(ui32 _size, GLenum _mode) :
m_size(_size),
m_mode(_mode)
{
	assert(m_size != 0);
	m_data = new ui16[m_size];
    m_index = -1;
    glGenBuffers(K_NUM_REPLACEMENT_INDEX_BUFFERS, m_handles);
}

CHIndexBuffer::~CHIndexBuffer(void)
{
    glDeleteBuffers(K_NUM_REPLACEMENT_INDEX_BUFFERS, m_handles);
    delete[] m_data;
}

void CHIndexBuffer::Unlock(void)
{
    assert(m_data != nullptr);
    assert(m_size != 0);
    m_index = (m_index >= (K_NUM_REPLACEMENT_INDEX_BUFFERS - 1)) ? 0 : m_index + 1;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_handles[m_index]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ui16) * m_size, m_data, m_mode);
}

void CHIndexBuffer::Unlock(ui32 _size)
{
    assert(m_data != nullptr);
    assert(_size != 0);
    m_index = (m_index >= (K_NUM_REPLACEMENT_INDEX_BUFFERS - 1)) ? 0 : m_index + 1;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_handles[m_index]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ui16) * _size, m_data, m_mode);
}

void CHIndexBuffer::Unlock(ui16 *_data, ui32 _size)
{
    assert(_data != nullptr);
    assert(m_size != 0);
    assert(m_data != nullptr);
    assert(_size != 0 && m_size >= _size);
    m_index = (m_index >= (K_NUM_REPLACEMENT_INDEX_BUFFERS - 1)) ? 0 : m_index + 1;
    memcpy(m_data, _data, sizeof(ui16) * _size);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_handles[m_index]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ui16) * _size, m_data, m_mode);
}

void CHIndexBuffer::Bind(void) const
{
    assert(m_index >= 0 && m_index <= (K_NUM_REPLACEMENT_INDEX_BUFFERS - 1));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_handles[m_index]);
}

void CHIndexBuffer::Unbind(void) const
{
    assert(m_index >= 0 && m_index <= (K_NUM_REPLACEMENT_INDEX_BUFFERS - 1));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}