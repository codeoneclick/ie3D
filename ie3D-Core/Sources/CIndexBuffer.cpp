//
//  CIndexBuffer.cpp
//  gEngine-Core
//
//  Created by Sergey Sergeev on 5/7/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CIndexBuffer.h"

CIndexBuffer::CIndexBuffer(ui32 sizeToAllocate, GLenum mode, ui16* mmap) : IResourceData(E_RESOURCE_DATA_CLASS_INDEX_BUFFER_DATA),
m_allocatedSize(sizeToAllocate),
m_usedSize(0),
m_mode(mode),
m_isMMAP(false)
{
	assert(m_allocatedSize != 0);
    ieGenBuffers(1, &m_handle);
    
    if(mmap == nullptr)
    {
        m_data = new ui16[m_allocatedSize];
    }
    else
    {
        m_data = mmap;
        m_isMMAP = true;
    }
}

CIndexBuffer::~CIndexBuffer(void)
{
    ieDeleteBuffers(1, &m_handle);
    
    if(!m_isMMAP)
    {
        delete[] m_data;
    }
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
    ieBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_handle);
    ieBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ui16) * m_usedSize, m_data, m_mode);
}

void CIndexBuffer::bind(void) const
{
    if(m_usedSize != 0)
    {
        ieBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_handle);
    }
}

void CIndexBuffer::unbind(void) const
{
    if(m_usedSize != 0)
    {
        ieBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);
    }
}