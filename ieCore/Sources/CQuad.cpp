//
//  CQuad.cpp
//  gEngine-Core
//
//  Created by Sergey Sergeev on 5/7/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CQuad.h"

CQuad::CQuad(void) :
m_vertexBuffer(nullptr),
m_indexBuffer(nullptr),
m_maxBound(glm::vec3(-4096.0f, -4096.0f, -4096.0f)),
m_minBound(glm::vec3( 4096.0f,  4096.0f,  4096.0f))
{
    m_vertexBuffer = std::make_shared<CVertexBuffer>(4, GL_STATIC_DRAW);
    SHardwareVertex* vertexData = m_vertexBuffer->Lock();

    vertexData[0].m_position = glm::vec3(-1.0f, -1.0f, 0.0f);
    vertexData[0].m_texcoord = CVertexBuffer::CompressVec2(glm::vec2(0.0f, 0.0f));
    vertexData[1].m_position = glm::vec3(-1.0f, 1.0f, 0.0f);
    vertexData[1].m_texcoord = CVertexBuffer::CompressVec2(glm::vec2(0.0f, 1.0f));
    vertexData[2].m_position = glm::vec3(1.0f, -1.0f, 0.0f);
    vertexData[2].m_texcoord = CVertexBuffer::CompressVec2(glm::vec2(1.0f, 0.0f));
    vertexData[3].m_position = glm::vec3(1.0f, 1.0f, 0.0f);
    vertexData[3].m_texcoord = CVertexBuffer::CompressVec2(glm::vec2(1.0f, 1.0f));
    m_vertexBuffer->Unlock();
    
    m_indexBuffer = std::make_shared<CIndexBuffer>(6, GL_STATIC_DRAW);
    ui16* indexData = m_indexBuffer->Lock();
    indexData[0] = 0;
    indexData[1] = 1;
    indexData[2] = 2;
    indexData[3] = 1;
    indexData[4] = 2;
    indexData[5] = 3;
    m_indexBuffer->Unlock();
}

CQuad::~CQuad(void)
{

}

void CQuad::Bind(const i32* _attributes)
{
    assert(m_vertexBuffer != nullptr);
    assert(m_indexBuffer != nullptr);
    m_vertexBuffer->Bind(_attributes);
    m_indexBuffer->Bind();
}

void CQuad::Draw(void)
{
    assert(m_vertexBuffer != nullptr);
    assert(m_indexBuffer != nullptr);
    glDrawElements(GL_TRIANGLES, m_indexBuffer->Get_Size(), GL_UNSIGNED_SHORT, NULL);
}

void CQuad::Unbind(const i32* _attributes)
{
    assert(m_vertexBuffer != nullptr);
    assert(m_indexBuffer != nullptr);
    m_vertexBuffer->Unbind(_attributes);
    m_indexBuffer->Unbind();
}

