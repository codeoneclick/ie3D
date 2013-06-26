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
#include "CGameLoopExecutor.h"
#include "CRenderMgr.h"
#include "CSceneUpdateMgr.h"
#include "CRenderOperationWorldSpace.h"
#include "CRenderOperationScreenSpace.h"
#include "CCommonOS.h"
#include "ITemplate.h"
#include "CGraphicsContext_iOS.h"

IGameTransition::IGameTransition(const std::string& _filename, std::shared_ptr<IGraphicsContext> _graphicsContext, std::shared_ptr<CResourceAccessor> _resourceAccessor, std::shared_ptr<CTemplateAccessor> _templateAccessor) :
CSceneFabricator(_templateAccessor, _resourceAccessor),
m_guid(_filename)
{
    assert(_graphicsContext != nullptr);
    m_renderMgr = std::make_shared<CRenderMgr>(_graphicsContext);
    m_sceneUpdateMgr = std::make_shared<CSceneUpdateMgr>();
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
    
    for(auto worldSpaceRenderOperationTemplate : gameTransitionTemplate->m_worldSpaceRenderOperationsTemplates)
    {

        std::shared_ptr<CRenderOperationWorldSpace> worldSpaceRenderOperation = std::make_shared<CRenderOperationWorldSpace>(Get_ScreenWidth(),
                                                                                                                                 Get_ScreenHeight(),
                                                                                                                                 worldSpaceRenderOperationTemplate->m_guid);
        m_renderMgr->RegisterWorldSpaceRenderOperation(worldSpaceRenderOperationTemplate->m_guid, worldSpaceRenderOperation);
    }
    
    for(auto screenSpaceRenderOperationTemplate : gameTransitionTemplate->m_screenSpaceRenderOperationsTemplates)
    {
        std::shared_ptr<SMaterialTemplate> screenSpaceRenderOperationMaterialTemplate = screenSpaceRenderOperationTemplate->m_materialTemplate;
        assert(screenSpaceRenderOperationMaterialTemplate != nullptr);

        std::shared_ptr<CShader> screenSpaceRenderOperationShader = m_resourceAccessor->CreateShader(screenSpaceRenderOperationMaterialTemplate->m_shaderTemplate->m_vsFilename,
                                                                                                     screenSpaceRenderOperationMaterialTemplate->m_shaderTemplate->m_fsFilename);
        assert(screenSpaceRenderOperationShader != nullptr);
        
        std::shared_ptr<CMaterial> screenSpaceRenderOperationMaterial = std::make_shared<CMaterial>(screenSpaceRenderOperationShader);
        screenSpaceRenderOperationMaterial->Serialize(screenSpaceRenderOperationMaterialTemplate, m_resourceAccessor, m_renderMgr);
        
        std::shared_ptr<CRenderOperationScreenSpace> screenSpaceRenderOperation = std::make_shared<CRenderOperationScreenSpace>(Get_ScreenWidth(),
                                                                                                                                Get_ScreenHeight(),
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
