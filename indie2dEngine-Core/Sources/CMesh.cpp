//
//  CMesh.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/26/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CMesh.h"
#include "CAABoundBox.h"
#include "CSkeleton.h"
#include "CBone.h"
#include "CTimer.h"
#include "CSequence.h"

CMeshHeader::CMeshHeader(void) :
m_vertexData(nullptr),
m_indexData(nullptr),
m_vertexBindData(nullptr),
m_numIndexes(0),
m_numVertexes(0),
m_maxBound(glm::vec3(-4096.0f)),
m_minBound(glm::vec3( 4096.0f))
{
    
}

CMeshHeader::~CMeshHeader(void)
{
    delete[] m_vertexData;
    delete[] m_indexData;
    delete[] m_vertexBindData;
}

CMesh::CMesh(const std::string& _guid) :
IResource(E_RESOURCE_TYPE_MESH, _guid),
m_header(nullptr),
m_vertexBuffer(nullptr),
m_indexBuffer(nullptr)
{
    
}

CMesh::CMesh(const std::string& _guid, std::shared_ptr<CVertexBuffer> _vertexBuffer, std::shared_ptr<CIndexBuffer> _indexBuffer) :
IResource(E_RESOURCE_TYPE_MESH, _guid),
m_header(std::make_shared<CMeshHeader>()),
m_vertexBuffer(_vertexBuffer),
m_indexBuffer(_indexBuffer)
{
    assert(_vertexBuffer != nullptr);
    assert(m_indexBuffer != nullptr);
    m_header->m_numVertexes = m_vertexBuffer->Get_Size();
    m_header->m_numIndexes = m_indexBuffer->Get_NumIndexes();
    m_header->m_vertexData = m_vertexBuffer->Lock();
    m_header->m_indexData = m_indexBuffer->Lock();
    
    for(ui32 i = 0; i < m_header->m_numVertexes; ++i)
    {
        if(m_header->m_vertexData[i].m_position.x > m_header->m_maxBound.x)
        {
            m_header->m_maxBound.x = m_header->m_vertexData[i].m_position.x;
        }
        if(m_header->m_vertexData[i].m_position.y > m_header->m_maxBound.y)
        {
            m_header->m_maxBound.y = m_header->m_vertexData[i].m_position.y;
        }
        if(m_header->m_vertexData[i].m_position.z > m_header->m_maxBound.z)
        {
            m_header->m_maxBound.z = m_header->m_vertexData[i].m_position.z;
        }
        if(m_header->m_vertexData[i].m_position.x < m_header->m_minBound.x)
        {
            m_header->m_minBound.x = m_header->m_vertexData[i].m_position.x;
        }
        if(m_header->m_vertexData[i].m_position.y < m_header->m_minBound.y)
        {
            m_header->m_minBound.y = m_header->m_vertexData[i].m_position.y;
        }
        if(m_header->m_vertexData[i].m_position.z < m_header->m_minBound.z)
        {
            m_header->m_minBound.z = m_header->m_vertexData[i].m_position.z;
        }
    }

    m_status |= E_RESOURCE_STATUS_LOADED;
    m_status |= E_RESOURCE_STATUS_COMMITED;
}

CMesh::~CMesh(void)
{
    m_bounds.clear();
}

void CMesh::_Set_Header(std::shared_ptr<CMeshHeader> _header)
{
    assert(_header != nullptr);
    m_header = _header;
    m_status |= E_RESOURCE_STATUS_LOADED;
    
    for(auto bound : m_bounds)
    {
        bound->_Set_MaxBound(m_header->Get_MaxBound());
        bound->_Set_MinBound(m_header->Get_MinBound());
    }
};

std::shared_ptr<CAABoundBox> CMesh::CreateBoundBox(void)
{
    std::shared_ptr<CAABoundBox> bound(std::make_shared<CAABoundBox>(m_header == nullptr ? glm::vec3(0.0f) : m_header->Get_MaxBound(), m_header == nullptr ? glm::vec3(0.0f) : m_header->Get_MinBound()));
    assert(bound != nullptr);
    m_bounds.push_back(bound);
    return bound;
}

void CMesh::Bind(const i32 *_attributes)
{
    if((m_status & E_RESOURCE_STATUS_LOADED) && (m_status & E_RESOURCE_STATUS_COMMITED))
    {
        assert(m_vertexBuffer != nullptr);
        assert(m_indexBuffer != nullptr);
        m_vertexBuffer->Bind(_attributes);
        m_indexBuffer->Bind();
    }
}

void CMesh::Draw(void)
{
    if((m_status & E_RESOURCE_STATUS_LOADED) && (m_status & E_RESOURCE_STATUS_COMMITED))
    {
        assert(m_vertexBuffer != nullptr);
        assert(m_indexBuffer != nullptr);
        glDrawElements(GL_TRIANGLES, m_indexBuffer->Get_NumIndexes(), GL_UNSIGNED_SHORT, NULL);
    }
}

void CMesh::Unbind(const i32 *_attributes)
{
    if((m_status & E_RESOURCE_STATUS_LOADED) && (m_status & E_RESOURCE_STATUS_COMMITED))
    {
        assert(m_vertexBuffer != nullptr);
        assert(m_indexBuffer != nullptr);
        m_vertexBuffer->Unbind(_attributes);
        m_indexBuffer->Unbind();
    }
}
