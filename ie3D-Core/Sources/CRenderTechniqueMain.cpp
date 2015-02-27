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
    ieBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
    ieBindRenderbuffer(GL_RENDERBUFFER, m_renderBuffer);
    ieViewport(0, 0, m_frameWidth, m_frameHeight);
    ieClearColor(m_clearColor.r, m_clearColor.g, m_clearColor.b, m_clearColor.a);
    ieClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    if(m_material->getShader()->isLoaded() &&
       m_material->getShader()->isCommited())
    {
        m_material->bind();
        m_quad->bind(m_material->getShader()->getGUID(), m_material->getShader()->getAttributes());
    }
}

void CRenderTechniqueMain::unbind(void)
{
    if(m_material->getShader()->isLoaded() &&
       m_material->getShader()->isCommited())
    {
        m_quad->unbind(m_material->getShader()->getGUID(), m_material->getShader()->getAttributes());
        m_material->unbind();
    }
}

void CRenderTechniqueMain::draw(void)
{
    if(m_material->getShader()->isLoaded() &&
       m_material->getShader()->isCommited())
    {
        m_quad->draw();
    }
}

void CRenderTechniqueMain::batch(void)
{
    
}