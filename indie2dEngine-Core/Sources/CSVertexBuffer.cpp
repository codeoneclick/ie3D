//
//  CSVertexBuffer.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 8/29/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CSVertexBuffer.h"

CSVertexBuffer::CSVertexBuffer(CSVertexBuffer::SVertex* _data, ui32 _size)
{
    m_data = new CSVertexBuffer::SVertex[_size];
    m_size = _size;
    memcpy(m_data, _data, sizeof(CSVertexBuffer::SVertex) * _size);
}

CSVertexBuffer::~CSVertexBuffer(void)
{
    delete [] m_data;
    for(auto iterator : m_references)
    {
        delete [] iterator.second;
    }
    m_references.clear();
}

const ui32 CSVertexBuffer::_GenerateGuid(void)
{
    static ui32 value = 0;
    return ++value;
}

const ui32 CSVertexBuffer::CreateReference(void)
{
    assert(m_size != 0);
    ui32 guid = CSVertexBuffer::_GenerateGuid();
    CSVertexBuffer::SVertex* data = new CSVertexBuffer::SVertex[m_size];
    memcpy(data, m_data, sizeof(CSVertexBuffer::SVertex) * m_size);
    m_references.insert(std::make_pair(guid, data));
    return guid;
}

void CSVertexBuffer::DeleteReference(ui32 _guid)
{
    auto iterator = m_references.find(_guid);
    assert(iterator != m_references.end());
    delete[] iterator->second;
    m_references.erase(iterator);
}