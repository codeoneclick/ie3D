//
//  CBoundingBox.cpp
//  ie3D-Core
//
//  Created by sergey.sergeev on 11/10/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#include "CBoundingBox.h"
#include "CVertexBuffer.h"
#include "CIndexBuffer.h"

CBoundingBox::CBoundingBox(const glm::vec3& minBound, const glm::vec3& maxBound) :
m_vertexBuffer(nullptr),
m_indexBuffer(nullptr),
m_wireframeIndexBuffer(nullptr),
m_maxBound(maxBound),
m_minBound(minBound)
{
    m_vertexBuffer = std::make_shared<CVertexBuffer>(8, GL_STATIC_DRAW);
    SAttributeVertex* vertexData = m_vertexBuffer->lock();
    
    glm::vec3 middle = (m_minBound + m_maxBound) * 0.5f;
    glm::vec3 minCoords = middle + (m_minBound - middle) * 1.01f;
    glm::vec3 maxCoords = middle + (m_maxBound - middle) * 1.01f;

    vertexData[0].m_position = minCoords;
    vertexData[1].m_position = glm::vec3(maxCoords.x, minCoords.y, minCoords.z);
    vertexData[2].m_position = glm::vec3(maxCoords.x, maxCoords.y, minCoords.z);
    vertexData[3].m_position = glm::vec3(minCoords.x, maxCoords.y, minCoords.z);
    
    vertexData[4].m_position = glm::vec3(minCoords.x, minCoords.y, maxCoords.z);
    vertexData[5].m_position = glm::vec3(maxCoords.x, minCoords.y, maxCoords.z);
    vertexData[6].m_position = maxCoords;
    vertexData[7].m_position = glm::vec3(minCoords.x, maxCoords.y, maxCoords.z);
    m_vertexBuffer->unlock();
    
    GLushort indices[] = {
        4, 5, 6,  4, 6, 7,
        1, 0, 3,  1, 3, 2,
        0, 4, 7,  0, 7, 3,
        5, 1, 2,  5, 2, 6,
        0, 4, 5,  0, 5, 1,
        3, 7, 6,  3, 6, 2
    };
    
    GLushort wireframeIndices[] = {
        0, 1, 1, 2,	2, 3, 3, 0,
        4, 5, 5, 6, 6, 7, 7, 4,
        0, 4, 1, 5, 2, 6, 3, 7
    };
    
    m_indexBuffer = std::make_shared<CIndexBuffer>(36, GL_STATIC_DRAW);
    ui16* indexData = m_indexBuffer->lock();
    memcpy(indexData, indices, sizeof(indices));
    m_indexBuffer->unlock();
    
    m_wireframeIndexBuffer = std::make_shared<CIndexBuffer>(24, GL_STATIC_DRAW);
    indexData = m_wireframeIndexBuffer->lock();
    memcpy(indexData, wireframeIndices, sizeof(wireframeIndices));
    m_wireframeIndexBuffer->unlock();
}

CBoundingBox::~CBoundingBox(void)
{
    
}

void CBoundingBox::bind(const std::array<i32, E_SHADER_ATTRIBUTE_MAX>& attributes, bool isWireframe)
{
    assert(m_vertexBuffer != nullptr);
    assert(m_indexBuffer != nullptr);
    m_vertexBuffer->bind(attributes);
    isWireframe ? m_wireframeIndexBuffer->bind() : m_indexBuffer->bind();
}

void CBoundingBox::draw(bool isWireframe)
{
    assert(m_vertexBuffer != nullptr);
    assert(m_indexBuffer != nullptr);
    glDrawElements(isWireframe ? GL_LINES : GL_TRIANGLES,
                   isWireframe ? m_wireframeIndexBuffer->getUsedSize() : m_indexBuffer->getUsedSize(),
                   GL_UNSIGNED_SHORT, NULL);
}

void CBoundingBox::unbind(const std::array<i32, E_SHADER_ATTRIBUTE_MAX>& attributes, bool isWireframe)
{
    assert(m_vertexBuffer != nullptr);
    assert(m_indexBuffer != nullptr);
    assert(m_wireframeIndexBuffer);
    m_vertexBuffer->unbind(attributes);
    isWireframe ? m_wireframeIndexBuffer->unbind() : m_indexBuffer->unbind();
}
