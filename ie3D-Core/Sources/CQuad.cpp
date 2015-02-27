//
//  CQuad.cpp
//  gEngine-Core
//
//  Created by Sergey Sergeev on 5/7/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CQuad.h"
#include "CVertexBuffer.h"
#include "CIndexBuffer.h"
#include "CVertexArrayBuffer.h"

CQuad::CQuad(void) :
m_vertexBuffer(nullptr),
m_indexBuffer(nullptr),
m_maxBound(glm::vec3(-4096.0f, -4096.0f, -4096.0f)),
m_minBound(glm::vec3( 4096.0f,  4096.0f,  4096.0f))
{
    m_vertexBuffer = std::make_shared<CVertexBuffer>(4, GL_STATIC_DRAW);
    SAttributeVertex* vertexData = m_vertexBuffer->lock();

    vertexData[0].m_position = glm::vec3(-1.0f, -1.0f, 0.0f);
    vertexData[0].m_texcoord = glm::packUnorm2x16(glm::vec2(0.0f, 0.0f));
    vertexData[1].m_position = glm::vec3(-1.0f, 1.0f, 0.0f);
    vertexData[1].m_texcoord = glm::packUnorm2x16(glm::vec2(0.0f, 1.0f));
    vertexData[2].m_position = glm::vec3(1.0f, -1.0f, 0.0f);
    vertexData[2].m_texcoord = glm::packUnorm2x16(glm::vec2(1.0f, 0.0f));
    vertexData[3].m_position = glm::vec3(1.0f, 1.0f, 0.0f);
    vertexData[3].m_texcoord = glm::packUnorm2x16(glm::vec2(1.0f, 1.0f));
    m_vertexBuffer->unlock();
    
    m_indexBuffer = std::make_shared<CIndexBuffer>(6, GL_STATIC_DRAW);
    ui16* indexData = m_indexBuffer->lock();
    indexData[0] = 0;
    indexData[1] = 1;
    indexData[2] = 2;
    indexData[3] = 1;
    indexData[4] = 2;
    indexData[5] = 3;
    m_indexBuffer->unlock();
}

CQuad::~CQuad(void)
{
    std::unordered_map<std::string, CSharedVertexArrayBuffer> eraser;
    m_VAOstates.swap(eraser);
}

void CQuad::bind(const std::string& attributesGUID, const std::array<i32, E_SHADER_ATTRIBUTE_MAX>& attributes)
{
    assert(m_vertexBuffer != nullptr);
    assert(m_indexBuffer != nullptr);
    assert(attributesGUID.length() != 0);
    
    CSharedVertexArrayBuffer vaoState = m_VAOstates[attributesGUID];
    if(!vaoState)
    {
        vaoState = std::make_shared<CVertexArrayBuffer>(m_vertexBuffer,
                                                        m_indexBuffer);
        vaoState->init(attributes);
        m_VAOstates[attributesGUID] = vaoState;
    }
    CVertexArrayBuffer::bind(vaoState);
}

void CQuad::draw(void)
{
    assert(m_vertexBuffer != nullptr);
    assert(m_indexBuffer != nullptr);
    ieDrawElements(GL_TRIANGLES, m_indexBuffer->getUsedSize(), GL_UNSIGNED_SHORT, NULL);
}

void CQuad::unbind(const std::string& attributesGUID, const std::array<i32, E_SHADER_ATTRIBUTE_MAX>& attributes)
{
    assert(m_vertexBuffer != nullptr);
    assert(m_indexBuffer != nullptr);
    
    CVertexArrayBuffer::unbind();
    
    /*m_vertexBuffer->unbind(attributes);
    m_indexBuffer->unbind();*/
}

