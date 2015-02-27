//
//  CRenderTechniqueWorldSpace.cpp
//  ie3D-Core
//
//  Created by sergey.sergeev on 7/2/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#include "CRenderTechniqueWorldSpace.h"
#include "IRenderTechniqueHandler.h"
#include "CTexture.h"
#include "CComponentRendering.h"
#include "CComponentDebugRendering.h"

CRenderTechniqueWorldSpace::CRenderTechniqueWorldSpace(ui32 frameWidth, ui32 frameHeight, const std::string& name, ui32 index) :
IRenderTechniqueBase(frameWidth, frameHeight, name, index),
m_numTriangles(0),
m_areDrawBoundingBoxes(false),
m_isOcclusionQueryEnabled(false)
{
    ui32 colorAttachmentId;
    ieGenTextures(1, &colorAttachmentId);
    ieBindTexture(GL_TEXTURE_2D, colorAttachmentId);
    ieTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    ieTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    ieTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    ieTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    ieTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_frameWidth, m_frameHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    
    ui32 depthAttachmentId;
    ieGenTextures(1, &depthAttachmentId);
    ieBindTexture(GL_TEXTURE_2D, depthAttachmentId);
    ieTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    ieTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    ieTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    ieTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
#if defined(__NDK__) || defined(__OSX__)
    ieTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, m_frameWidth, m_frameHeight, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, NULL);
#else
#if defined(__OPENGL_30__)
    ieTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, m_frameWidth, m_frameHeight, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, NULL);
#else
    ieTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_frameWidth, m_frameHeight, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT, NULL);
#endif
#endif
    
    ieGenFramebuffers(1, &m_frameBuffer);
    ieBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
    ieFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorAttachmentId, 0);
    ieFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthAttachmentId, 0);
    
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    assert(status == GL_FRAMEBUFFER_COMPLETE);
    
    std::string operatingColorTextureGuid = m_name;
    operatingColorTextureGuid.append(".color");
    m_operatingColorTexture = CTexture::constructCustomTexture(operatingColorTextureGuid,
                                                               colorAttachmentId,
                                                               m_frameWidth,
                                                               m_frameHeight);
    
    m_operatingColorTexture->setWrapMode(GL_CLAMP_TO_EDGE);
    
    std::string operatingDepthTextureGuid = m_name;
    operatingDepthTextureGuid.append(".depth");
    m_operatingDepthTexture = CTexture::constructCustomTexture(operatingDepthTextureGuid,
                                                               depthAttachmentId,
                                                               m_frameWidth,
                                                               m_frameHeight);
    m_operatingDepthTexture->setWrapMode(GL_CLAMP_TO_EDGE);
}

CRenderTechniqueWorldSpace::~CRenderTechniqueWorldSpace(void)
{
    
}

void CRenderTechniqueWorldSpace::setAreDrawBoundingBoxes(bool value)
{
    m_areDrawBoundingBoxes = value;
}

void CRenderTechniqueWorldSpace::setIsOcclusionQueryEnabled(bool value)
{
    m_isOcclusionQueryEnabled = value;
}
bool CRenderTechniqueWorldSpace::isOcclusionQueryEnabled(void) const
{
    return m_isOcclusionQueryEnabled;
}

CSharedTexture CRenderTechniqueWorldSpace::getOperatingColorTexture(void) const
{
    assert(m_operatingColorTexture != nullptr);
    return m_operatingColorTexture;
}

CSharedTexture CRenderTechniqueWorldSpace::getOperatingDepthTexture(void) const
{
    assert(m_operatingDepthTexture != nullptr);
    return m_operatingDepthTexture;
}

ui32 CRenderTechniqueWorldSpace::getNumTriangles(void) const
{
    return m_numTriangles;
}

void CRenderTechniqueWorldSpace::addRenderTechniqueHandler(ISharedRenderTechniqueHandlerRef handler)
{
    assert(handler);
    CSharedComponentRendering componentRendering = handler->getComponentRendering();
    assert(componentRendering);
    
    const auto& uniqueContainer = m_handlers.find(componentRendering->getZOrder());
    if(uniqueContainer != m_handlers.end())
    {
        uniqueContainer->second.insert(handler);
    }
    else
    {
        m_handlers[componentRendering->getZOrder()].insert(handler);
    }
}

void CRenderTechniqueWorldSpace::removeRenderTechniqueHandler(ISharedRenderTechniqueHandlerRef handler)
{
    assert(handler);
    CSharedComponentRendering componentRendering = handler->getComponentRendering();
    assert(componentRendering);
    
    const auto& uniqueContainer = m_handlers.find(componentRendering->getZOrder());
    if(uniqueContainer != m_handlers.end())
    {
        uniqueContainer->second.erase(handler);
    }
    else
    {
        m_handlers[componentRendering->getZOrder()].erase(handler);
    }
}

void CRenderTechniqueWorldSpace::bind(void)
{
    ieBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
    ieViewport(0, 0, m_frameWidth, m_frameHeight);
    ieClearColor(m_clearColor.r, m_clearColor.g, m_clearColor.b, m_clearColor.a);
    ieClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_numTriangles = 0;
}

void CRenderTechniqueWorldSpace::drawBoundingBox(void)
{
    
}

void CRenderTechniqueWorldSpace::unbind(void)
{
    
}

void CRenderTechniqueWorldSpace::draw(void)
{
    for(std::map<ui32, std::set<ISharedRenderTechniqueHandler>>::iterator iterator_01 = m_handlers.begin(); iterator_01 != m_handlers.end(); ++iterator_01)
    {
        for(std::set<ISharedRenderTechniqueHandler>::iterator iterator_02 = (*iterator_01).second.begin(); iterator_02 !=  (*iterator_01).second.end(); ++iterator_02)
        {
            ISharedRenderTechniqueHandler handler = (*iterator_02);
            CSharedComponentRendering componentRendering = handler->getComponentRendering();
            assert(componentRendering);
            if(componentRendering->isInCameraFrustum())
            {
                componentRendering->draw(m_name);
            }
            
            CSharedComponentDebugRendering componentDebugRendering = handler->getComponentDebugRendering();
            if(componentDebugRendering)
            {
                componentDebugRendering->draw(m_name);
            }
        }
    }
}

void CRenderTechniqueWorldSpace::batch(void)
{

}