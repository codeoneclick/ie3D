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

void CRenderMgr::RegisterOutputRenderOperation(std::shared_ptr<CMaterial> material)
{
    assert(m_graphicsContext != nullptr);
    assert(material != nullptr);
    m_outputOperation = std::make_shared<CRenderOperationOutput>(m_graphicsContext->getWidth(),
                                                                 m_graphicsContext->getHeight(),
                                                                 material,
                                                                 m_graphicsContext->getFrameBuffer(),
                                                                 m_graphicsContext->getRenderBuffer());
    const auto& platform = g_platforms.find(getPlatform());
    assert(platform != g_platforms.end());
    std::cout<<"[Device] : "<<platform->second<<std::endl;
    std::cout<<"[Output resolution] : "<<m_graphicsContext->getWidth()<<
    "x"<<m_graphicsContext->getHeight()<<std::endl;
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

std::shared_ptr<CTexture> CRenderMgr::getSSOperationTexture(const std::string& mode)
{
    std::string findmode = mode;
    std::string::size_type location = mode.find(".depth");
    if (std::string::npos != location)
    {
        findmode = std::string(mode, 0, location);
    }
    std::shared_ptr<CTexture> texture = m_worldSpaceOperations.find(findmode) != m_worldSpaceOperations.end() ? std::string::npos == location ? m_worldSpaceOperations.find(findmode)->second->Get_OperatingColorTexture() : m_worldSpaceOperations.find(findmode)->second->Get_OperatingDepthTexture() : m_screenSpaceOperations.find(findmode) != m_screenSpaceOperations.end() ? m_screenSpaceOperations.find(findmode)->second->Get_OperatingTexture() : nullptr;
    assert(texture != nullptr);
    return texture;
}

std::shared_ptr<CTexture> CRenderMgr::preprocessSSOperationTexture(CSharedMaterialRef material, ui32 width, ui32 height)
{
    std::shared_ptr<CRenderOperationScreenSpace> operation = std::make_shared<CRenderOperationScreenSpace>(width, height, "render.mode.custom", material);
    m_customScreenSpaceOperationsQueue.push(operation);
    return operation->Get_OperatingTexture();
}

CSharedMaterial CRenderMgr::getSSOperationMaterial(const std::string& mode)
{
    CSharedMaterial material = m_screenSpaceOperations.find(mode) != m_screenSpaceOperations.end() ? m_screenSpaceOperations.find(mode)->second->Get_Material(): nullptr;
    assert(material != nullptr);
    return material;
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
        
        m_batchingMgr->lock(operation->Get_Mode());
        operation->Batch();
        m_batchingMgr->unlock(operation->Get_Mode());
        
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
    
    m_graphicsContext->draw();
}

ui32 CRenderMgr::getWidth(void) const
{
    return m_graphicsContext->getWidth();
}

ui32 CRenderMgr::getHeight(void) const
{
    return m_graphicsContext->getHeight();
}

