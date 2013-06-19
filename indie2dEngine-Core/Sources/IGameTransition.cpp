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
        std::function<void(std::string, std::shared_ptr<CRenderMgr>)> function = [](const std::string& _guid, std::shared_ptr<CRenderMgr> _renderMgr)
        {
            std::shared_ptr<CRenderOperationWorldSpace> worldSpaceRenderOperation = std::make_shared<CRenderOperationWorldSpace>(Get_ScreenWidth(),
                                                                                                                                 Get_ScreenHeight(),
                                                                                                                                 _guid);
            _renderMgr->RegisterWorldSpaceRenderOperation(_guid, worldSpaceRenderOperation);
        };
        
        thread_concurrency_dispatch<std::string, std::shared_ptr<CRenderMgr> >(get_thread_concurrency_main_queue(), function, worldSpaceRenderOperationTemplate->m_guid, m_renderMgr);
    }
    
    for(auto screenSpaceRenderOperationTemplate : gameTransitionTemplate->m_screenSpaceRenderOperationsTemplates)
    {
        std::shared_ptr<SMaterialTemplate> screenSpaceRenderOperationMaterialTemplate = screenSpaceRenderOperationTemplate->m_materialTemplate;
        assert(screenSpaceRenderOperationMaterialTemplate != nullptr);

        std::shared_ptr<CShader> screenSpaceRenderOperationShader = m_resourceAccessor->CreateShader(screenSpaceRenderOperationMaterialTemplate->m_shaderTemplate->m_vsFilename,
                                                                                                     screenSpaceRenderOperationMaterialTemplate->m_shaderTemplate->m_fsFilename);
        assert(screenSpaceRenderOperationShader != nullptr);
        

        
        std::function<void(std::string, std::shared_ptr<CRenderMgr>, std::shared_ptr<CResourceAccessor>, std::shared_ptr<CShader>, std::shared_ptr<SMaterialTemplate>)> function = [](std::string _guid, std::shared_ptr<CRenderMgr> _renderMgr,std::shared_ptr<CResourceAccessor> _resourceAccessor, std::shared_ptr<CShader> _shader, std::shared_ptr<SMaterialTemplate> _screenSpaceRenderOperationMaterialTemplate)
        {
            std::shared_ptr<CMaterial> screenSpaceRenderOperationMaterial = std::make_shared<CMaterial>(_shader);
            screenSpaceRenderOperationMaterial->Serialize(_screenSpaceRenderOperationMaterialTemplate, _resourceAccessor, _renderMgr);
            
            std::shared_ptr<CRenderOperationScreenSpace> screenSpaceRenderOperation = std::make_shared<CRenderOperationScreenSpace>(Get_ScreenWidth(),
                                                                                                                                    Get_ScreenHeight(),
                                                                                                                                    _guid,
                                                                                                                                    screenSpaceRenderOperationMaterial);
            _renderMgr->RegisterScreenSpaceRenderOperation(_guid, screenSpaceRenderOperation);
        };
        
        thread_concurrency_dispatch<std::string, std::shared_ptr<CRenderMgr>, std::shared_ptr<CResourceAccessor>, std::shared_ptr<CShader>, std::shared_ptr<SMaterialTemplate>>(get_thread_concurrency_main_queue(), function, screenSpaceRenderOperationTemplate->m_guid, m_renderMgr, m_resourceAccessor, screenSpaceRenderOperationShader, screenSpaceRenderOperationMaterialTemplate);
    }
    
    std::shared_ptr<SMaterialTemplate> outputRenderOperationMaterialTemplate = gameTransitionTemplate->m_outputRenderOperationTemplate->m_materialTemplate;
    assert(outputRenderOperationMaterialTemplate != nullptr);
    
    std::shared_ptr<CShader> outputRenderOperationShader = m_resourceAccessor->CreateShader(outputRenderOperationMaterialTemplate->m_shaderTemplate->m_vsFilename,
                                                                                            outputRenderOperationMaterialTemplate->m_shaderTemplate->m_fsFilename);
    assert(outputRenderOperationShader != nullptr);
    
    std::function<void(IGameTransition*, std::shared_ptr<CRenderMgr>, std::shared_ptr<CResourceAccessor>, std::shared_ptr<CShader>, std::shared_ptr<SMaterialTemplate>)> function = [](IGameTransition* _gameTransition, std::shared_ptr<CRenderMgr> _renderMgr,std::shared_ptr<CResourceAccessor> _resourceAccessor, std::shared_ptr<CShader> _shader, std::shared_ptr<SMaterialTemplate> _outputRenderOperationMaterialTemplate)
    {
        std::shared_ptr<CMaterial> outputRenderOperationMaterial = std::make_shared<CMaterial>(_shader);
        outputRenderOperationMaterial->Serialize(_outputRenderOperationMaterialTemplate, _resourceAccessor, _renderMgr);
        _renderMgr->RegisterOutputRenderOperation(outputRenderOperationMaterial);
        assert(_gameTransition != nullptr);
        _gameTransition->_OnLoaded();
    };
    
    thread_concurrency_dispatch<IGameTransition*, std::shared_ptr<CRenderMgr>, std::shared_ptr<CResourceAccessor>, std::shared_ptr<CShader>, std::shared_ptr<SMaterialTemplate>>(get_thread_concurrency_main_queue(), function, this, m_renderMgr, m_resourceAccessor, outputRenderOperationShader, outputRenderOperationMaterialTemplate);
}
