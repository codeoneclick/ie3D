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
m_numIndices(0),
m_numVerticies(0),
m_maxBound(glm::vec3(-4096.0f)),
m_minBound(glm::vec3( 4096.0f))
{
    
}

CMeshHeader::~CMeshHeader(void)
{
    delete[] m_vertexData;
    delete[] m_indexData;
}

CMesh::CMesh(const std::string& _guid) :
IResource(E_RESOURCE_CLASS_MESH, _guid),
m_header(nullptr),
m_vertexBuffer(nullptr),
m_indexBuffer(nullptr)
{
    
}

CMesh::CMesh(const std::string& _guid, std::shared_ptr<CVertexBuffer> _vertexBuffer, std::shared_ptr<CIndexBuffer> _indexBuffer) :
IResource(E_RESOURCE_CLASS_MESH, _guid),
m_header(std::make_shared<CMeshHeader>()),
m_vertexBuffer(_vertexBuffer),
m_indexBuffer(_indexBuffer)
{
    m_header->m_numVerticies = m_vertexBuffer->Get_Size();
    m_header->m_numIndices = m_indexBuffer->Get_Size();
    m_header->m_vertexData = nullptr;
    m_header->m_indexData = nullptr;
    
    SHardwareVertex* vertexData = m_vertexBuffer->Lock();
    
    for(ui32 i = 0; i < m_header->m_numVerticies; ++i)
    {
        if(vertexData[i].m_position.x > m_header->m_maxBound.x)
        {
            m_header->m_maxBound.x = vertexData[i].m_position.x;
        }
        if(vertexData[i].m_position.y > m_header->m_maxBound.y)
        {
            m_header->m_maxBound.y = vertexData[i].m_position.y;
        }
        if(vertexData[i].m_position.z > m_header->m_maxBound.z)
        {
            m_header->m_maxBound.z = vertexData[i].m_position.z;
        }
        if(vertexData[i].m_position.x < m_header->m_minBound.x)
        {
            m_header->m_minBound.x = vertexData[i].m_position.x;
        }
        if(vertexData[i].m_position.y < m_header->m_minBound.y)
        {
            m_header->m_minBound.y = vertexData[i].m_position.y;
        }
        if(vertexData[i].m_position.z < m_header->m_minBound.z)
        {
            m_header->m_minBound.z = vertexData[i].m_position.z;
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
    
    for(const auto& bound : m_bounds)
    {
        bound->_Set_MaxBound(m_header->m_maxBound);
        bound->_Set_MinBound(m_header->m_minBound);
    }
};

std::shared_ptr<CAABoundBox> CMesh::CreateBoundBox(void)
{
    std::shared_ptr<CAABoundBox> bound(std::make_shared<CAABoundBox>(m_header == nullptr ? glm::vec3(0.0f) : m_header->m_maxBound, m_header == nullptr ? glm::vec3(0.0f) : m_header->m_minBound));
    assert(bound != nullptr);
    m_bounds.push_back(bound);
    return bound;
}

void CMesh::Bind(const i32 *_attributes) const
{
    if(IResource::IsLoaded() && IResource::IsCommited())
    {
        assert(m_vertexBuffer != nullptr);
        assert(m_indexBuffer != nullptr);
        m_vertexBuffer->Bind(_attributes);
        m_indexBuffer->Bind();
    }
}

void CMesh::Draw(void) const
{
    if(IResource::IsLoaded() && IResource::IsCommited())
    {
        assert(m_vertexBuffer != nullptr);
        assert(m_indexBuffer != nullptr);
        glDrawElements(GL_TRIANGLES, m_indexBuffer->Get_Size(), GL_UNSIGNED_SHORT, NULL);
    }
}

void CMesh::Draw(ui32 _indices) const
{
    if(IResource::IsLoaded() && IResource::IsCommited())
    {
        assert(m_vertexBuffer != nullptr);
        assert(m_indexBuffer != nullptr);
        glDrawElements(GL_TRIANGLES, _indices, GL_UNSIGNED_SHORT, NULL);
    }
}

void CMesh::Unbind(const i32 *_attributes) const
{
    if(IResource::IsLoaded() && IResource::IsCommited())
    {
        assert(m_vertexBuffer != nullptr);
        assert(m_indexBuffer != nullptr);
        m_indexBuffer->Unbind();
        m_vertexBuffer->Unbind(_attributes);
    }
}
