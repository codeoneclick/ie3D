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
    ui32 textureId;
    glGenTextures(1, &textureId);
    glGenFramebuffers(1, &m_frameBufferHandle);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_frameWidth, m_frameHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferHandle);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureId, 0);
    
    assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
    
    std::string operatingColorTextureGuid = m_mode;
    operatingColorTextureGuid.append("color");
    m_operatingTexture = CTexture::constructCustomTexture(operatingColorTextureGuid,
                                                       textureId,
                                                       m_frameWidth,
                                                       m_frameHeight);
    m_operatingTexture->setWrapMode(GL_CLAMP_TO_EDGE);
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
    
    m_material->bind();
    m_quad->Bind(m_material->getShader()->getAttributesRef());
}

void CRenderOperationScreenSpace::Unbind(void)
{
    m_quad->Unbind(m_material->getShader()->getAttributesRef());
    m_material->unbind();
}

void CRenderOperationScreenSpace::Draw(void)
{
    m_quad->Draw();
}

