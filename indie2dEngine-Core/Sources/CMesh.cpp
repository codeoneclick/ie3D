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
}

CMesh::CMesh(const std::string& _guid) :
IResource(E_RESOURCE_CLASS_MESH, _guid),
m_header(nullptr),
m_softwareVertexBuffer(nullptr),
m_hardwareVertexBuffer(nullptr),
m_softwareIndexBuffer(nullptr),
m_hardwareIndexBuffer(nullptr)
{
    
}

CMesh::CMesh(const std::string& _guid, std::shared_ptr<CSVertexBuffer> _softwareVertexBuffer, std::shared_ptr<CSIndexBuffer> _softwareIndexBuffer) :
IResource(E_RESOURCE_CLASS_MESH, _guid),
m_header(std::make_shared<CMeshHeader>()),
m_softwareVertexBuffer(_softwareVertexBuffer),
m_hardwareVertexBuffer(nullptr),
m_softwareIndexBuffer(_softwareIndexBuffer),
m_hardwareIndexBuffer(nullptr)
{
    assert(_softwareVertexBuffer != nullptr);
    assert(_softwareIndexBuffer != nullptr);
    m_header->m_numVertexes = m_softwareVertexBuffer->Get_Size();
    m_header->m_numIndexes = m_softwareIndexBuffer->Get_Size();
    m_header->m_vertexData = m_softwareVertexBuffer->Lock();
    m_header->m_indexData = m_softwareIndexBuffer->Lock();
    
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
}

CMesh::~CMesh(void)
{
    m_bounds.clear();
}

void CMesh::CreateHardwareBuffers(GLenum _vertexBufferMode, GLenum _indexBufferMode)
{
    assert(m_softwareVertexBuffer != nullptr);
    assert(m_softwareVertexBuffer->Get_Size() != 0);
    if(m_hardwareVertexBuffer == nullptr)
    {
        m_hardwareVertexBuffer = std::make_shared<CHVertexBuffer>(m_softwareVertexBuffer->Get_Size(), _vertexBufferMode);
        m_hardwareVertexBuffer->Unlock(m_softwareVertexBuffer->Lock(), m_softwareVertexBuffer->Get_Size());
    }
    
    assert(m_softwareIndexBuffer != nullptr);
    assert(m_softwareIndexBuffer->Get_Size() != 0);
    if(m_hardwareIndexBuffer == nullptr)
    {
        m_hardwareIndexBuffer = std::make_shared<CHIndexBuffer>(m_softwareIndexBuffer->Get_Size(), _indexBufferMode);
        m_hardwareIndexBuffer->Unlock(m_softwareIndexBuffer->Lock(), m_softwareIndexBuffer->Get_Size());
    }
    m_status |= E_RESOURCE_STATUS_COMMITED;
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
        assert(m_hardwareVertexBuffer != nullptr);
        assert(m_hardwareIndexBuffer != nullptr);
        m_hardwareIndexBuffer->Bind();
        m_hardwareVertexBuffer->Bind(_attributes);
    }
}

void CMesh::Draw(void) const
{
    if(IResource::IsLoaded() && IResource::IsCommited())
    {
        assert(m_hardwareVertexBuffer != nullptr);
        assert(m_hardwareIndexBuffer != nullptr);
        glDrawElements(GL_TRIANGLES, m_hardwareIndexBuffer->Get_Size(), GL_UNSIGNED_SHORT, NULL);
    }
}

void CMesh::Draw(ui32 _indices) const
{
    if(IResource::IsLoaded() && IResource::IsCommited())
    {
        assert(m_hardwareVertexBuffer != nullptr);
        assert(m_hardwareIndexBuffer != nullptr);
        glDrawElements(GL_TRIANGLES, _indices, GL_UNSIGNED_SHORT, NULL);
    }
}

void CMesh::Unbind(const i32 *_attributes) const
{
    if(IResource::IsLoaded() && IResource::IsCommited())
    {
        assert(m_hardwareVertexBuffer != nullptr);
        assert(m_hardwareIndexBuffer != nullptr);
        m_hardwareIndexBuffer->Unbind();
        m_hardwareVertexBuffer->Unbind(_attributes);
    }
}
