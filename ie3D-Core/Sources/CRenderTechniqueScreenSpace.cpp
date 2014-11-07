//
//  CRenderTechniqueScreenSpace.cpp
//  ie3D-Core
//
//  Created by sergey.sergeev on 7/2/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#include "CRenderTechniqueScreenSpace.h"
#include "CTexture.h"
#include "CQuad.h"
#include "CShader.h"
#include "CMaterial.h"

CRenderTechniqueScreenSpace::CRenderTechniqueScreenSpace(ui32 frameWidth, ui32 frameHeight, const std::string& name, CSharedMaterialRef material) :
IRenderTechniqueBase(frameWidth, frameHeight, name, 0),
m_material(material)
{
    ui32 textureId;
    glGenTextures(1, &textureId);
    glGenFramebuffers(1, &m_frameBuffer);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_frameWidth, m_frameHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureId, 0);
    
    assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
    
    std::string operatingTextureGuid = m_name;
    m_operatingTexture = CTexture::constructCustomTexture(operatingTextureGuid,
                                                          textureId,
                                                          m_frameWidth,
                                                          m_frameHeight);
    m_operatingTexture->setWrapMode(GL_CLAMP_TO_EDGE);
    m_quad = std::make_shared<CQuad>();
}

CRenderTechniqueScreenSpace::~CRenderTechniqueScreenSpace(void)
{
    
}

CSharedTexture CRenderTechniqueScreenSpace::getOperatingTexture(void) const
{
    assert(m_operatingTexture != nullptr);
    return m_operatingTexture;
}

CSharedMaterial CRenderTechniqueScreenSpace::getMaterial(void) const
{
    assert(m_material != nullptr);
    return m_material;
}

void CRenderTechniqueScreenSpace::bind(void)
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
    glViewport(0, 0, m_frameWidth, m_frameHeight);
    glClearColor(m_clearColor.r, m_clearColor.g, m_clearColor.b, m_clearColor.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_material->bind();
    m_quad->bind(m_material->getShader()->getAttributes());
}

void CRenderTechniqueScreenSpace::unbind(void)
{
    m_quad->unbind(m_material->getShader()->getAttributes());
    m_material->unbind();
}

void CRenderTechniqueScreenSpace::draw(void)
{
    m_quad->draw();
}

void CRenderTechniqueScreenSpace::batch(void)
{
    
}