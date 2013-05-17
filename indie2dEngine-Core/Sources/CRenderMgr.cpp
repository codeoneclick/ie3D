//
//  CRenderMgr.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/17/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CRenderMgr.h"
#include "CCommonOS.h"
#include "IOGLContext.h"
#include "CMaterial.h"
#include "CTexture.h"
#include "CRenderOperationWorldSpace.h"
#include "CRenderOperationScreenSpace.h"
#include "CRenderOperationOutput.h"

CRenderMgr::CRenderMgr(const IOGLContext* _glContext, std::shared_ptr<CMaterial> _material) :
m_glContext(_glContext),
m_outputRenderMaterial(_material)
{
    assert(m_outputRenderMaterial != nullptr);
    m_outputOperation = std::make_shared<CRenderOperationOutput>(Get_ScreenWidth(),
                                                                      Get_ScreenHeight(),
                                                                      m_outputRenderMaterial,
                                                                      m_glContext->Get_FrameBufferHandle(),
                                                                      m_glContext->Get_RenderBufferHandle());
}

CRenderMgr::~CRenderMgr(void)
{
    
}

void CRenderMgr::RegisterWorldSpaceRenderOperation(const std::string &_mode, std::shared_ptr<CRenderOperationWorldSpace> _operation)
{
    assert(m_worldSpaceOperations.find(_mode) == m_worldSpaceOperations.end());
    m_worldSpaceOperations.insert(std::make_pair(_mode, _operation));
}

void CRenderMgr::UnregisterWorldSpaceRenderOperation(const std::string &_mode)
{
    auto iterator = m_worldSpaceOperations.find(_mode);
    assert(iterator != m_worldSpaceOperations.end());
    m_worldSpaceOperations.erase(iterator);
}

void CRenderMgr::RegisterScreenSpaceRenderOperation(const std::string &_mode, std::shared_ptr<CRenderOperationScreenSpace> _operation)
{
    assert(m_screenSpaceOperations.find(_mode) == m_screenSpaceOperations.end());
    m_screenSpaceOperations.insert(std::make_pair(_mode, _operation));
}

void CRenderMgr::UnregisterScreenSpaceRenderOperation(const std::string &_mode)
{
    auto iterator = m_screenSpaceOperations.find(_mode);
    assert(iterator != m_screenSpaceOperations.end());
    m_screenSpaceOperations.erase(iterator);
}

std::shared_ptr<CTexture> CRenderMgr::PushCustomScreenSpaceRenderOperation(std::shared_ptr<CMaterial> _material, ui32 _textureWidth, ui32 _textureHeight)
{
    std::shared_ptr<CRenderOperationScreenSpace> operation = std::make_shared<CRenderOperationScreenSpace>(_textureWidth, _textureHeight, "render.mode.custom", _material);
    m_customScreenSpaceOperationsQueue.push(operation);
    return operation->Get_OperatingTexture();
}

void CRenderMgr::RegisterWorldSpaceRenderHandler(const std::string &_mode, std::shared_ptr<IRenderHandler> _handler)
{
    auto iterator = m_worldSpaceOperations.find(_mode);
    assert(iterator != m_worldSpaceOperations.end());
    iterator->second->RegisterRenderHandler(_handler);
}

void CRenderMgr::UnregisterWorldSpaceRenderHandler(const std::string &_mode, std::shared_ptr<IRenderHandler> _handler)
{
    auto iterator = m_worldSpaceOperations.find(_mode);
    assert(iterator != m_worldSpaceOperations.end());
    iterator->second->UnregisterRenderHandler(_handler);
}

void CRenderMgr::OnGameLoopUpdate(f32 _deltatime)
{
    for(auto iterator : m_worldSpaceOperations)
    {
        std::shared_ptr<CRenderOperationWorldSpace> operation = iterator.second;
        operation->Bind();
        operation->Draw();
        operation->Unbind();
    }
    
    for(auto iterator : m_screenSpaceOperations)
    {
        std::shared_ptr<CRenderOperationScreenSpace> operation = iterator.second;
        operation->Bind();
        operation->Draw();
        operation->Unbind();
    }
    
    while(!m_customScreenSpaceOperationsQueue.empty())
    {
        std::shared_ptr<CRenderOperationScreenSpace> operation = m_customScreenSpaceOperationsQueue.front();
        operation->Bind();
        operation->Draw();
        operation->Unbind();
        m_customScreenSpaceOperationsQueue.pop();
    }
    
    // TODO :
    assert(m_worldSpaceOperations.begin() != m_worldSpaceOperations.end());
    auto iterator = m_worldSpaceOperations.begin();
    std::shared_ptr<CRenderOperationWorldSpace> operation = iterator->second;
    m_outputOperation->Get_Material()->Set_Texture(operation->Get_OperatingTexture(), E_SHADER_SAMPLER_01);
    m_outputOperation->Bind();
    m_outputOperation->Draw();
    m_outputOperation->Unbind();
    
    m_glContext->Output();
    
    GLenum error = glGetError();
    assert(error == GL_NO_ERROR);
}