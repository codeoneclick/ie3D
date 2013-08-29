//
//  CSIndexBuffer.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 8/29/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CSIndexBuffer.h"

CSIndexBuffer::CSIndexBuffer(ui16* _data, ui32 _size)
{
    m_data = new ui16[_size];
    m_size = _size;
    memcpy(m_data, _data, sizeof(ui16) * _size);
}

CSIndexBuffer::~CSIndexBuffer(void)
{
    delete [] m_data;
    for(auto iterator : m_references)
    {
        delete [] iterator.second;
    }
    m_references.clear();
}

const ui32 CSIndexBuffer::_GenerateGuid(void)
{
    static ui32 value = 0;
    return ++value;
}

const ui32 CSIndexBuffer::CreateReference(void)
{
    assert(m_size != 0);
    ui32 guid = CSIndexBuffer::_GenerateGuid();
    ui16* data = new ui16[m_size];
    memcpy(data, m_data, sizeof(ui16) * m_size);
    m_references.insert(std::make_pair(guid, data));
    return guid;
}

void CSIndexBuffer::DeleteReference(ui32 _guid)
{
    auto iterator = m_references.find(_guid);
    assert(iterator != m_references.end());
    delete[] iterator->second;
    m_references.erase(iterator);
}
