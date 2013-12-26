//
//  CRenderOperationWorldSpace.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/17/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CRenderOperationWorldSpace.h"
#include "CTexture.h"
#include "IRenderHandler.h"

CRenderOperationWorldSpace::CRenderOperationWorldSpace(ui32 _frameWidth, ui32 _frameHeight, const glm::vec4& _clearColor, const std::string& _mode, ui32 _index) :
m_mode(_mode),
m_index(_index),
m_frameWidth(_frameWidth),
m_frameHeight(_frameHeight),
m_clearColor(_clearColor),
m_numTriangles(0)
{
    ui32 textureColorHandle;
    glGenTextures(1, &textureColorHandle);
    glBindTexture(GL_TEXTURE_2D, textureColorHandle);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_frameWidth, m_frameHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    
    ui32 textureDepthHandle;
    glGenTextures(1, &textureDepthHandle);
    glBindTexture(GL_TEXTURE_2D, textureDepthHandle);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
#if defined(__NDK__)
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_frameWidth, m_frameHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
#else
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_frameWidth, m_frameHeight, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT, NULL);
#endif
    
    glGenFramebuffers(1, &m_frameBufferHandle);
    glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferHandle);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorHandle, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, textureDepthHandle, 0);
    
    assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
    
    std::shared_ptr<CTextureHeader> m_operatingColorTextureHeader = std::make_shared<CTextureHeader>();
    m_operatingColorTextureHeader->_Set_Width(m_frameWidth);
    m_operatingColorTextureHeader->_Set_Height(m_frameHeight);
    
    std::string m_operatingColorTextureGuid = m_mode;
    m_operatingColorTextureGuid.append("color");
    m_operatingColorTexture = std::make_shared<CTexture>(m_operatingColorTextureGuid);
    m_operatingColorTexture->Set_Header(m_operatingColorTextureHeader);
    m_operatingColorTexture->Set_Handle(textureColorHandle);
    m_operatingColorTexture->Set_WrapMode(GL_CLAMP_TO_EDGE);
    
    std::shared_ptr<CTextureHeader> m_operatingDepthTextureHeader = std::make_shared<CTextureHeader>();
    m_operatingDepthTextureHeader->_Set_Width(m_frameWidth);
    m_operatingDepthTextureHeader->_Set_Height(m_frameHeight);
    
    std::string m_operatingDepthTextureGuid = m_mode;
    m_operatingDepthTextureGuid.append("depth");
    m_operatingDepthTexture = std::make_shared<CTexture>(m_operatingDepthTextureGuid);
    m_operatingDepthTexture->Set_Header(m_operatingDepthTextureHeader);
    m_operatingDepthTexture->Set_Handle(textureDepthHandle);
    m_operatingDepthTexture->Set_WrapMode(GL_CLAMP_TO_EDGE);
}

CRenderOperationWorldSpace::~CRenderOperationWorldSpace(void)
{
    
}

void CRenderOperationWorldSpace::RegisterRenderHandler(const std::shared_ptr<IRenderHandler>& _handler)
{
    assert(_handler != nullptr);
    
    if(m_handlers.find(_handler->_Get_Commands()._ExecuteRenderQueuePositionCommand()) != m_handlers.end())
    {
        m_handlers.find(_handler->_Get_Commands()._ExecuteRenderQueuePositionCommand())->second.insert(_handler);
    }
    else
    {
        m_handlers[_handler->_Get_Commands()._ExecuteRenderQueuePositionCommand()].insert(_handler);
    }
}

void CRenderOperationWorldSpace::UnregisterRenderHandler(const std::shared_ptr<IRenderHandler>& _handler)
{
    assert(_handler != nullptr);
    
    if(m_handlers.find(_handler->_Get_Commands()._ExecuteRenderQueuePositionCommand()) != m_handlers.end())
    {
        m_handlers.find(_handler->_Get_Commands()._ExecuteRenderQueuePositionCommand())->second.erase(_handler);
    }
    else
    {
        m_handlers[_handler->_Get_Commands()._ExecuteRenderQueuePositionCommand()].erase(_handler);
    }
}

void CRenderOperationWorldSpace::Batch(void)
{
    for(std::map<ui32, std::set< std::shared_ptr<IRenderHandler> > >::iterator iterator_01 = m_handlers.begin(); iterator_01 != m_handlers.end(); ++iterator_01)
    {
        for(std::set< std::shared_ptr<IRenderHandler> >::iterator iterator_02 = (*iterator_01).second.begin(); iterator_02 !=  (*iterator_01).second.end(); ++iterator_02)
        {
            std::shared_ptr<IRenderHandler> handler = (*iterator_02);
            assert(handler != nullptr);
            if(!handler->_OnOcclusion())
            {
                handler->_Get_Commands()._ExecuteRenderBatchCommand(m_mode);
            }
        }
    }
}

void CRenderOperationWorldSpace::Bind(void)
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferHandle);
    glViewport(0, 0, m_frameWidth, m_frameHeight);
    glClearColor(m_clearColor.r, m_clearColor.g, m_clearColor.b, m_clearColor.a);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    m_numTriangles = 0;
}

void CRenderOperationWorldSpace::Unbind(void)
{
#if defined(__IOS__)
    
    const GLenum discards[]  = {GL_COLOR_ATTACHMENT0, GL_DEPTH_ATTACHMENT};
    glDiscardFramebufferEXT(GL_FRAMEBUFFER, 2, discards);
    
#elif defined(__WIN32__)

#endif
}

void CRenderOperationWorldSpace::Draw(void)
{
    for(std::map<ui32, std::set< std::shared_ptr<IRenderHandler> > >::iterator iterator_01 = m_handlers.begin(); iterator_01 != m_handlers.end(); ++iterator_01)
    {
        for(std::set< std::shared_ptr<IRenderHandler> >::iterator iterator_02 = (*iterator_01).second.begin(); iterator_02 !=  (*iterator_01).second.end(); ++iterator_02)
        {
            std::shared_ptr<IRenderHandler> handler = (*iterator_02);
            assert(handler != nullptr);
            if(!handler->_OnOcclusion())
            {
                handler->_Get_Commands()._ExecuteRenderBindCommand(m_mode);
                handler->_Get_Commands()._ExecuteRenderDrawCommand(m_mode);
                handler->_Get_Commands()._ExecuteRenderUnbindCommand(m_mode);
                handler->_Get_Commands()._ExecuteRenderDebugDrawCommand(m_mode);
                m_numTriangles += handler->_Get_Commands()._ExecuteRenderGetNumTrianglesCommand();
            }
        }
    }
}









