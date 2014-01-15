//
//  CRenderOperationScreenSpace.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/17/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CRenderOperationScreenSpace.h"
#include "CTexture.h"
#include "CMaterial.h"
#include "CShader.h"
#include "CQuad.h"

CRenderOperationScreenSpace::CRenderOperationScreenSpace(ui32 _frameWidth, ui32 _frameHeight, const std::string& _mode, std::shared_ptr<CMaterial> _material) :
m_mode(_mode),
m_frameWidth(_frameWidth),
m_frameHeight(_frameHeight),
m_material(_material)
{
    ui32 textureHandle;
    glGenTextures(1, &textureHandle);
    glGenFramebuffers(1, &m_frameBufferHandle);
    glGenRenderbuffers(1, &m_depthBufferHandle);
    glBindTexture(GL_TEXTURE_2D, textureHandle);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_frameWidth, m_frameHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferHandle);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureHandle, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, m_depthBufferHandle);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, m_frameWidth, m_frameHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthBufferHandle);
    
    assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
    
    m_operatingTexture = std::make_shared<CTexture>(m_mode);
    std::shared_ptr<CTextureHeader> header = std::make_shared<CTextureHeader>();
    header->_Set_Width(m_frameWidth);
    header->_Set_Height(m_frameHeight);
    m_operatingTexture = std::make_shared<CTexture>(m_mode);
    m_operatingTexture->Set_Header(header);
    m_operatingTexture->Set_Handle(textureHandle);
    m_operatingTexture->Set_WrapMode(GL_CLAMP_TO_EDGE);
    
    m_material->Set_IsCulling(false);
    m_material->Set_IsDepthMask(true);
    m_material->Set_IsDepthTest(false);
    m_material->Set_IsBlending(false);
    
    m_material->Set_CullingMode(GL_FRONT);
    m_material->Set_BlendingFunctionSource(GL_SRC_ALPHA);
    m_material->Set_BlendingFunctionDestination(GL_ONE_MINUS_SRC_ALPHA);
    
    m_quad = std::make_shared<CQuad>();
}

CRenderOperationScreenSpace::~CRenderOperationScreenSpace(void)
{
   
}

void CRenderOperationScreenSpace::Bind(void)
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferHandle);
    glViewport(0, 0, m_frameWidth, m_frameHeight);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    m_material->Bind();
    m_quad->Bind(m_material->Get_Shader()->Get_Attributes());
}

void CRenderOperationScreenSpace::Unbind(void)
{
    m_quad->Unbind(m_material->Get_Shader()->Get_Attributes());
    m_material->Unbind();
}

void CRenderOperationScreenSpace::Draw(void)
{
    m_quad->Draw();
}

