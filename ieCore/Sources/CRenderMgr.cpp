//
//  CRenderMgr.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/17/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CRenderMgr.h"
#include "CCommonOS.h"
#include "IGraphicsContext.h"
#include "CMaterial.h"
#include "CTexture.h"
#include "CBatchingMgr.h"
#include "CRenderOperationWorldSpace.h"
#include "CRenderOperationScreenSpace.h"
#include "CRenderOperationOutput.h"

CRenderMgr::CRenderMgr(const std::shared_ptr<IGraphicsContext> _graphicsContext) :
m_graphicsContext(_graphicsContext),
m_outputOperation(nullptr),
m_batchingMgr(nullptr),
m_numTriangles(0)
{
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDisable(GL_CULL_FACE);
    glDisable(GL_BLEND);
    glDisable(GL_STENCIL_TEST);
    glDisable(GL_DITHER);
    glDepthFunc(GL_LEQUAL);
}

CRenderMgr::~CRenderMgr(void)
{

}

void CRenderMgr::RegisterOutputRenderOperation(std::shared_ptr<CMaterial> _material)
{
    assert(_material != nullptr);
    m_outputOperation = std::make_shared<CRenderOperationOutput>(Get_ScreenWidth(),
                                                                 Get_ScreenHeight(),
                                                                 _material,
                                                                 m_graphicsContext->Get_FrameBufferHandle(),
                                                                 m_graphicsContext->Get_RenderBufferHandle());
    std::cout<<"[Output resolution] : "<<Get_ScreenWidth()<<"x"<<Get_ScreenHeight()<<std::endl;
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

std::shared_ptr<CTexture> CRenderMgr::Get_RenderOperationTexture(const std::string& _mode)
{
    std::string mode = _mode;
    std::string::size_type location = _mode.find(".depth");
    if (std::string::npos != location)
    {
        mode = std::string(_mode, 0, location);
    }
    std::shared_ptr<CTexture> texture = m_worldSpaceOperations.find(mode) != m_worldSpaceOperations.end() ? std::string::npos == location ? m_worldSpaceOperations.find(mode)->second->Get_OperatingColorTexture() : m_worldSpaceOperations.find(mode)->second->Get_OperatingDepthTexture() : m_screenSpaceOperations.find(mode) != m_screenSpaceOperations.end() ? m_screenSpaceOperations.find(mode)->second->Get_OperatingTexture() : nullptr;
    return texture;
}

std::shared_ptr<CTexture> CRenderMgr::Get_CustomScreenSpaceOperationTexture(const std::shared_ptr<CMaterial> _material, ui32 _textureWidth, ui32 _textureHeight)
{
    std::shared_ptr<CRenderOperationScreenSpace> operation = std::make_shared<CRenderOperationScreenSpace>(_textureWidth, _textureHeight, "render.mode.custom", _material);
    m_customScreenSpaceOperationsQueue.push(operation);
    return operation->Get_OperatingTexture();
}

void CRenderMgr::_OnGameLoopUpdate(f32 _deltatime)
{
    assert(m_batchingMgr != nullptr);
    m_numTriangles = 0;
    
    typedef std::pair<std::string, std::shared_ptr<CRenderOperationWorldSpace>> operation_t;
    
    std::vector<operation_t> operations(m_worldSpaceOperations.begin(), m_worldSpaceOperations.end());
    std::sort(operations.begin(), operations.end(), [](const operation_t& _operation_01, const operation_t& _operation_02){
        return _operation_01.second->Get_Index() < _operation_02.second->Get_Index();
    });
    
    for(const auto& iterator : operations)
    {
        std::shared_ptr<CRenderOperationWorldSpace> operation = iterator.second;
        
        m_batchingMgr->Lock(operation->Get_Mode());
        operation->Batch();
        m_batchingMgr->Unlock(operation->Get_Mode());
        
        operation->Bind();
        operation->Draw();
        operation->Unbind();

        m_numTriangles += operation->Get_NumTriangles();
    }
    
    for(const auto& iterator : m_screenSpaceOperations)
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
    
    if(m_outputOperation != nullptr)
    {
        m_outputOperation->Bind();
        m_outputOperation->Draw();
        m_outputOperation->Unbind();
    }
    
    m_graphicsContext->Output();
}