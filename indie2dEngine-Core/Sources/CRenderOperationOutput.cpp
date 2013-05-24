//
//  CRenderOperationOutput.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/17/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CRenderOperationOutput.h"
#include "CMaterial.h"
#include "CShader.h"
#include "CShape.h"

CRenderOperationOutput::CRenderOperationOutput(ui32 _frameWidth, ui32 _frameHeight, std::shared_ptr<CMaterial> _material, ui32 _frameBufferHandle, ui32 _renderBufferHandle) :
m_frameWidth(_frameWidth),
m_frameHeight(_frameHeight),
m_material(_material),
m_frameBufferHandle(_frameBufferHandle),
m_renderBufferHandle(_renderBufferHandle)
{
    CVertexBuffer* vertexBuffer = new CVertexBuffer(4, GL_STATIC_DRAW);
    SVertex* vertexData = vertexBuffer->Lock();
    vertexData[0].m_position = glm::vec3(-1.0f, -1.0f, 0.0f);
    vertexData[0].m_texcoord = glm::vec2(0.0f, 0.0f);
    vertexData[1].m_position = glm::vec3(-1.0f, 1.0f, 0.0f);
    vertexData[1].m_texcoord = glm::vec2(0.0f, 1.0f);
    vertexData[2].m_position = glm::vec3(1.0f, -1.0f, 0.0f);
    vertexData[2].m_texcoord = glm::vec2(1.0f, 0.0f);
    vertexData[3].m_position = glm::vec3(1.0f, 1.0f, 0.0f);
    vertexData[3].m_texcoord = glm::vec2(1.0f, 1.0f);
    vertexBuffer->Unlock();
    
    CIndexBuffer* indexBuffer = new CIndexBuffer(6, GL_STATIC_DRAW);
    ui16* indexData = indexBuffer->Lock();
    indexData[0] = 0;
    indexData[1] = 1;
    indexData[2] = 2;
    indexData[3] = 1;
    indexData[4] = 2;
    indexData[5] = 3;
    indexBuffer->Unlock();
    
    m_material->Set_RenderState(E_RENDER_STATE_CULL_MODE, false);
    m_material->Set_RenderState(E_RENDER_STATE_DEPTH_MASK, true);
    m_material->Set_RenderState(E_RENDER_STATE_DEPTH_TEST, false);
    m_material->Set_RenderState(E_RENDER_STATE_BLEND_MODE, false);
    
    m_material->Set_CullFaceMode(GL_FRONT);
    m_material->Set_BlendFunctionSource(GL_SRC_ALPHA);
    m_material->Set_BlendFunctionDest(GL_ONE_MINUS_SRC_ALPHA);
    
    m_shape = std::make_shared<CShape>();
    m_shape->Link(std::move(vertexBuffer), std::move(indexBuffer));
}

CRenderOperationOutput::~CRenderOperationOutput(void)
{
    
}

void CRenderOperationOutput::Bind(void)
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferHandle);
    glBindRenderbuffer(GL_RENDERBUFFER, m_renderBufferHandle);
    glViewport(0, 0, m_frameWidth, m_frameHeight);
    glClearColor(0.5, 0.5, 0.5, 1.0);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    m_material->Bind();
    m_shape->Bind(m_material->Get_Shader()->Get_Attributes());
}

void CRenderOperationOutput::Unbind(void)
{
    m_shape->Unbind(m_material->Get_Shader()->Get_Attributes());
    m_material->Unbind();
}

void CRenderOperationOutput::Draw(void)
{
    m_shape->Draw();
}
