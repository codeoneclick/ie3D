//
//  CRenderTechniqueMain.cpp
//  ie3D-Core
//
//  Created by sergey.sergeev on 7/2/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#include "CRenderTechniqueMain.h"
#include "CMaterial.h"
#include "CQuad.h"
#include "CShader.h"

CRenderTechniqueMain::CRenderTechniqueMain(ui32 frameWidth, ui32 frameHeight, CSharedMaterialRef material, ui32 frameBuffer, ui32 renderBuffer) :
IRenderTechniqueBase(frameWidth, frameHeight, "render.technique.main", 0),
m_renderBuffer(renderBuffer),
m_material(material)
{
    m_frameBuffer = frameBuffer;
    assert(m_material != nullptr);
	m_quad = std::make_shared<CQuad>();
}

CRenderTechniqueMain::~CRenderTechniqueMain(void)
{
    
}

void CRenderTechniqueMain::bind(void)
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, m_renderBuffer);
    glViewport(0, 0, m_frameWidth, m_frameHeight);
    glClearColor(0.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    m_material->bind();
    m_quad->bind(m_material->getShader()->getAttributesRef());
}

void CRenderTechniqueMain::unbind(void)
{
    m_quad->unbind(m_material->getShader()->getAttributesRef());
    m_material->unbind();
}

void CRenderTechniqueMain::draw(void)
{
    m_quad->draw();
}