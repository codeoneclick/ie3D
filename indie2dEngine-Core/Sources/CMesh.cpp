//
//  CMesh.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/26/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CMesh.h"

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
    m_header->m_numVertexes = m_vertexBuffer->Get_NumVertexes();
    m_header->m_numIndexes = m_indexBuffer->Get_NumIndexes();
    m_header->m_vertexData = m_vertexBuffer->Lock();
    m_header->m_indexData = m_indexBuffer->Lock();
    m_header->m_maxBound = glm::vec3(0.0f);
    m_header->m_minBound = glm::vec3(0.0f);
    
    m_isLoaded = true;
    m_isLinked = true;
}

CMesh::~CMesh(void)
{
    
}

void CMesh::Bind(const i32 *_attributes)
{
    if(m_isLoaded && m_isLinked)
    {
        assert(m_vertexBuffer != nullptr);
        assert(m_indexBuffer != nullptr);
        m_vertexBuffer->Bind(_attributes);
        m_indexBuffer->Bind();
    }
}

void CMesh::Draw(void)
{
    if(m_isLoaded && m_isLinked)
    {
        assert(m_vertexBuffer != nullptr);
        assert(m_indexBuffer != nullptr);
        glDrawElements(GL_TRIANGLES, m_indexBuffer->Get_NumIndexes(), GL_UNSIGNED_SHORT, NULL);
    }
}

void CMesh::Unbind(const i32 *_attributes)
{
    if(m_isLoaded && m_isLinked)
    {
        assert(m_vertexBuffer != nullptr);
        assert(m_indexBuffer != nullptr);
        m_vertexBuffer->Unbind(_attributes);
        m_indexBuffer->Unbind();
    }
}
