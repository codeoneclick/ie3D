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

CRenderOperationWorldSpace::CRenderOperationWorldSpace(ui32 _frameWidth, ui32 _frameHeight, const std::string& _mode) :
m_mode(_mode),
m_frameWidth(_frameWidth),
m_frameHeight(_frameHeight)
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
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_frameWidth, m_frameHeight, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, NULL);
    glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferHandle);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureHandle, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, m_depthBufferHandle);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, m_frameWidth, m_frameHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthBufferHandle);
    
    assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
    
    std::shared_ptr<CTextureHeader> header = std::make_shared<CTextureHeader>();
    header->_Set_Width(m_frameWidth);
    header->_Set_Height(m_frameHeight);
    m_operatingTexture = std::make_shared<CTexture>(m_mode);
    m_operatingTexture->_Set_Header(header);
    m_operatingTexture->_Set_Handle(textureHandle);
    m_operatingTexture->Set_Wrap(GL_CLAMP_TO_EDGE);
}

CRenderOperationWorldSpace::~CRenderOperationWorldSpace(void)
{
    
}

void CRenderOperationWorldSpace::RegisterRenderHandler(std::shared_ptr<IRenderHandler> _handler)
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

void CRenderOperationWorldSpace::UnregisterRenderHandler(std::shared_ptr<IRenderHandler> _handler)
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

void CRenderOperationWorldSpace::Bind(void)
{
    glDepthMask(GL_TRUE);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glDisable(GL_BLEND);
    
    glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferHandle);
    glViewport(0, 0, m_frameWidth, m_frameHeight);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}

void CRenderOperationWorldSpace::Unbind(void)
{
    
}

void CRenderOperationWorldSpace::Draw(void)
{
    for(std::map<ui32, std::set< std::shared_ptr<IRenderHandler> > >::iterator iterator_01 = m_handlers.begin(); iterator_01 != m_handlers.end(); ++iterator_01)
    {
        for(std::set< std::shared_ptr<IRenderHandler> >::iterator iterator_02 = (*iterator_01).second.begin(); iterator_02 !=  (*iterator_01).second.end(); ++iterator_02)
        {
            std::shared_ptr<IRenderHandler> handler = (*iterator_02);
            assert(handler != nullptr);
            
            handler->_Get_Commands()._ExecuteRenderBindCommand(m_mode);
            handler->_Get_Commands()._ExecuteRenderDrawCommand(m_mode);
            handler->_Get_Commands()._ExecuteRenderUnbindCommand(m_mode);
        }
    }
}









