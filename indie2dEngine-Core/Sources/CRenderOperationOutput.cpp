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
#include "CQuad.h"

CRenderOperationOutput::CRenderOperationOutput(ui32 _frameWidth, ui32 _frameHeight, std::shared_ptr<CMaterial> _material, ui32 _frameBufferHandle, ui32 _renderBufferHandle) :
m_frameWidth(_frameWidth),
m_frameHeight(_frameHeight),
m_material(_material),
m_frameBufferHandle(_frameBufferHandle),
m_renderBufferHandle(_renderBufferHandle)
{
    assert(m_material != nullptr);
    m_material->Set_IsCulling(false);
    m_material->Set_IsDepthMask(true);
    m_material->Set_IsDepthTest(false);
    m_material->Set_IsBlending(false);
    
    m_material->Set_CullingMode(GL_FRONT);
    m_material->Set_BlendingFunctionSource(GL_SRC_ALPHA);
    m_material->Set_BlendingFunctionDestination(GL_ONE_MINUS_SRC_ALPHA);
    
	m_quad = std::make_shared<CQuad>();
}

CRenderOperationOutput::~CRenderOperationOutput(void)
{
    
}

void CRenderOperationOutput::Bind(void)
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferHandle);
    glBindRenderbuffer(GL_RENDERBUFFER, m_renderBufferHandle);
    glViewport(0, 0, m_frameWidth, m_frameHeight);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    m_material->Bind();
    m_quad->Bind(m_material->Get_Shader()->Get_Attributes());
}

void CRenderOperationOutput::Unbind(void)
{
    m_quad->Unbind(m_material->Get_Shader()->Get_Attributes());
    m_material->Unbind();

#if defined(__IOS__)
    
    const GLenum discards[]  = {GL_COLOR_ATTACHMENT0, GL_DEPTH_ATTACHMENT};
    glDiscardFramebufferEXT(GL_FRAMEBUFFER, 2, discards);
    
#elif defined(__WIN32__)

#endif
}

void CRenderOperationOutput::Draw(void)
{
    m_quad->Draw();
}

