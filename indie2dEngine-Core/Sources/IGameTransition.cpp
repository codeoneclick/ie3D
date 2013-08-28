//
//  IGameTransition.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 6/6/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "IGameTransition.h"
#include "CResourceAccessor.h"
#include "CTemplateAccessor.h"
#include "CMaterial.h"
#include "CShader.h"
#include "CTexture.h"
#include "IGameObject.h"
#include "CGameLoopExecutor.h"
#include "CRenderMgr.h"
#include "CSceneUpdateMgr.h"
#include "CCollisionMgr.h"
#include "CRenderOperationWorldSpace.h"
#include "CRenderOperationScreenSpace.h"
#include "CCommonOS.h"
#include "ITemplate.h"
#include "CGraphicsContext_iOS.h"
#include "IInputContext.h"

IGameTransition::IGameTransition(const std::string& _filename, std::shared_ptr<IGraphicsContext> _graphicsContext, std::shared_ptr<IInputContext> _inputContext, std::shared_ptr<CResourceAccessor> _resourceAccessor, std::shared_ptr<CTemplateAccessor> _templateAccessor) :
CSceneFabricator(_templateAccessor, _resourceAccessor),
m_guid(_filename),
m_scene(nullptr),
m_isLoaded(false)
{
    assert(_graphicsContext != nullptr);
    assert(_inputContext != nullptr);
    m_renderMgr = std::make_shared<CRenderMgr>(_graphicsContext);
    m_sceneUpdateMgr = std::make_shared<CSceneUpdateMgr>();
    m_collisionMgr = std::make_shared<CCollisionMgr>();
    
    m_inputContext = _inputContext;
    m_inputContext->RegisterTapRecognizerHandler(m_collisionMgr);
}

IGameTransition::~IGameTransition(void)
{
    
}

void IGameTransition::_OnRegistered(void)
{
    
}

void IGameTransition::_OnUnregistered(void)
{
    
}

void IGameTransition::_OnActivate(void)
{
    ConnectToGameLoop(m_renderMgr);
    ConnectToGameLoop(m_sceneUpdateMgr);
}

void IGameTransition::_OnDeactivate(void)
{
    DisconnectFromGameLoop(m_renderMgr);
    DisconnectFromGameLoop(m_sceneUpdateMgr);
}

void IGameTransition::_OnLoaded(void)
{
    assert(false);
}

void IGameTransition::_OnGameLoopUpdate(f32 _deltatime)
{
    assert(false);
}

void IGameTransition::_OnTemplateLoaded(std::shared_ptr<ITemplate> _template)
{
    assert(m_renderMgr != nullptr);
    assert(m_resourceAccessor != nullptr);
    
   
    std::shared_ptr<SGameTransitionTemplate> gameTransitionTemplate = std::static_pointer_cast<SGameTransitionTemplate>(_template);
    assert(gameTransitionTemplate != nullptr);
    
    for(const auto& worldSpaceRenderOperationTemplate : gameTransitionTemplate->m_worldSpaceRenderOperationsTemplates)
    {
        
        std::shared_ptr<CRenderOperationWorldSpace> worldSpaceRenderOperation = std::make_shared<CRenderOperationWorldSpace>(worldSpaceRenderOperationTemplate->m_screenWidth,
                                                                                                                             worldSpaceRenderOperationTemplate->m_screenHeight,
                                                                                                                             worldSpaceRenderOperationTemplate->m_guid);
        m_renderMgr->RegisterWorldSpaceRenderOperation(worldSpaceRenderOperationTemplate->m_guid, worldSpaceRenderOperation);
    }
    
    for(const auto& screenSpaceRenderOperationTemplate : gameTransitionTemplate->m_screenSpaceRenderOperationsTemplates)
    {
        std::shared_ptr<SMaterialTemplate> screenSpaceRenderOperationMaterialTemplate = screenSpaceRenderOperationTemplate->m_materialTemplate;
        assert(screenSpaceRenderOperationMaterialTemplate != nullptr);

        std::shared_ptr<CShader> screenSpaceRenderOperationShader = m_resourceAccessor->CreateShader(screenSpaceRenderOperationMaterialTemplate->m_shaderTemplate->m_vsFilename,
                                                                                                     screenSpaceRenderOperationMaterialTemplate->m_shaderTemplate->m_fsFilename);
        assert(screenSpaceRenderOperationShader != nullptr);
        
        std::shared_ptr<CMaterial> screenSpaceRenderOperationMaterial = std::make_shared<CMaterial>(screenSpaceRenderOperationShader);
        screenSpaceRenderOperationMaterial->Serialize(screenSpaceRenderOperationMaterialTemplate, m_resourceAccessor, m_renderMgr);
        
        std::shared_ptr<CRenderOperationScreenSpace> screenSpaceRenderOperation = std::make_shared<CRenderOperationScreenSpace>(screenSpaceRenderOperationTemplate->m_screenWidth,
                                                                                                                                screenSpaceRenderOperationTemplate->m_screenHeight,
                                                                                                                                screenSpaceRenderOperationTemplate->m_guid,
                                                                                                                                screenSpaceRenderOperationMaterial);
        m_renderMgr->RegisterScreenSpaceRenderOperation( screenSpaceRenderOperationTemplate->m_guid, screenSpaceRenderOperation);
    }
    
    std::shared_ptr<SMaterialTemplate> outputRenderOperationMaterialTemplate = gameTransitionTemplate->m_outputRenderOperationTemplate->m_materialTemplate;
    assert(outputRenderOperationMaterialTemplate != nullptr);
    
    std::shared_ptr<CShader> outputRenderOperationShader = m_resourceAccessor->CreateShader(outputRenderOperationMaterialTemplate->m_shaderTemplate->m_vsFilename,
                                                                                            outputRenderOperationMaterialTemplate->m_shaderTemplate->m_fsFilename);
    assert(outputRenderOperationShader != nullptr);
    
    std::shared_ptr<CMaterial> outputRenderOperationMaterial = std::make_shared<CMaterial>(outputRenderOperationShader);
    outputRenderOperationMaterial->Serialize(outputRenderOperationMaterialTemplate, m_resourceAccessor, m_renderMgr);
    m_renderMgr->RegisterOutputRenderOperation(outputRenderOperationMaterial);
    
    _OnLoaded();
}

ui32 IGameTransition::Get_CurrentNumTriangles(void)
{
    assert(m_renderMgr != nullptr);
    return m_renderMgr->Get_NumTriangles();
}


ui32 IGameTransition::Get_TotalNumTriangles(void)
{
    ui32 numTriangles = 0;
    for(const auto& gameObject : CSceneFabricator::m_gameObjectsContainer)
    {
        numTriangles += gameObject->Get_NumTriangles();
    }
    return numTriangles;
}
