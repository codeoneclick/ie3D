//
//  IGameTransition.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 6/6/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "IGameTransition.h"
#include "CResourceAccessor.h"
#include "CConfigurationAccessor.h"
#include "CMaterial.h"
#include "CShader.h"
#include "CTexture.h"
#include "IGameObject.h"
#include "CGameLoopExecutor.h"
#include "CRenderMgr.h"
#include "CBatchingMgr.h"
#include "CSceneUpdateMgr.h"
#include "CCollisionMgr.h"
#include "CRenderOperationWorldSpace.h"
#include "CRenderOperationScreenSpace.h"
#include "CCommonOS.h"
#include "CConfigurationGameObjects.h"
#include "IGraphicsContext.h"
#include "IInputContext.h"
#include "CResourceLoader.h"

IGameTransition::IGameTransition(const std::string& _filename, std::shared_ptr<IGraphicsContext> _graphicsContext, std::shared_ptr<IInputContext> _inputContext, std::shared_ptr<CResourceAccessor> _resourceAccessor, std::shared_ptr<CConfigurationAccessor> configurationAccessor) :
IFabricator(configurationAccessor, _resourceAccessor),
CSceneFabricator(configurationAccessor, _resourceAccessor),
m_guid(_filename),
m_scene(nullptr),
m_isLoaded(false)
{
    assert(_graphicsContext != nullptr);
    assert(_inputContext != nullptr);
    m_renderMgr = std::make_shared<CRenderMgr>(_graphicsContext);
	_Set_ScreenSpaceTextureAccessor(m_renderMgr);
    std::shared_ptr<CBatchingMgr> batchingMgr = std::make_shared<CBatchingMgr>(m_renderMgr);
    m_renderMgr->Set_BatchingMgr(batchingMgr);
    m_sceneUpdateMgr = std::make_shared<CSceneUpdateMgr>();
    m_collisionMgr = std::make_shared<CCollisionMgr>();
    
    m_inputContext = _inputContext;
    m_inputContext->addGestureRecognizerHandler(std::static_pointer_cast<IGestureRecognizerHandler>(m_collisionMgr));
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
    ConnectToGameLoop(m_resourceAccessor->getResourceLoader());
}

void IGameTransition::_OnDeactivate(void)
{
    DisconnectFromGameLoop(m_renderMgr);
    DisconnectFromGameLoop(m_sceneUpdateMgr);
    DisconnectFromGameLoop(m_resourceAccessor->getResourceLoader());
}

void IGameTransition::_OnLoaded(void)
{
    assert(false);
}

void IGameTransition::_OnGameLoopUpdate(f32 _deltatime)
{
    assert(false);
}

void IGameTransition::onConfigurationLoaded(ISharedConfigurationRef configuration, bool success)
{
    assert(m_renderMgr != nullptr);
    assert(m_resourceAccessor != nullptr);
    
    std::shared_ptr<CConfigurationGameTransition> gameTransitionConfiguration = std::static_pointer_cast<CConfigurationGameTransition>(configuration);
    assert(gameTransitionConfiguration != nullptr);
    
    for(const auto& iterator : gameTransitionConfiguration->getWSRenderOperationsConfigurations())
    {
        std::shared_ptr<CConfigurationWSRenderOperation> worldSpaceRenderOperationConfiguration = std::static_pointer_cast<CConfigurationWSRenderOperation>(iterator);
        std::shared_ptr<CRenderOperationWorldSpace> worldSpaceRenderOperation =
        std::make_shared<CRenderOperationWorldSpace>(worldSpaceRenderOperationConfiguration->getScreenWidth(),
                                                     worldSpaceRenderOperationConfiguration->getScreenHeight(),
                                                     worldSpaceRenderOperationConfiguration->getClearColor(),
                                                     worldSpaceRenderOperationConfiguration->getGuid(),
                                                     worldSpaceRenderOperationConfiguration->getIndex());
        m_renderMgr->RegisterWorldSpaceRenderOperation(worldSpaceRenderOperationConfiguration->getGuid(), worldSpaceRenderOperation);
    }
    
    for(const auto& iterator : gameTransitionConfiguration->getSSRenderOperationsConfigurations())
    {
        std::shared_ptr<CConfigurationSSRenderOperation> screenSpaceRenderOperationConfiguration = std::static_pointer_cast<CConfigurationSSRenderOperation>(iterator);
        std::shared_ptr<CConfigurationMaterial> screenSpaceRenderOperationMaterialConfiguration = std::static_pointer_cast<CConfigurationMaterial>(screenSpaceRenderOperationConfiguration->getMaterialConfiguration());
        assert(screenSpaceRenderOperationMaterialConfiguration != nullptr);
        
        std::shared_ptr<CMaterial> screenSpaceRenderOperationMaterial = std::make_shared<CMaterial>();
        
        assert(screenSpaceRenderOperationMaterialConfiguration != nullptr);
        assert(m_screenSpaceTextureAccessor != nullptr);
        assert(m_resourceAccessor != nullptr);
        
        CMaterial::setupMaterial(screenSpaceRenderOperationMaterial,
                                 screenSpaceRenderOperationMaterialConfiguration,
                                 m_resourceAccessor,
                                 m_screenSpaceTextureAccessor);
        
        std::shared_ptr<CRenderOperationScreenSpace> screenSpaceRenderOperation =
        std::make_shared<CRenderOperationScreenSpace>(screenSpaceRenderOperationConfiguration->getScreenWidth(),
                                                      screenSpaceRenderOperationConfiguration->getScreenHeight(),
                                                      screenSpaceRenderOperationConfiguration->getGuid(),
                                                      screenSpaceRenderOperationMaterial);
        m_renderMgr->RegisterScreenSpaceRenderOperation( screenSpaceRenderOperationConfiguration->getGuid(), screenSpaceRenderOperation);
    }
    
    std::shared_ptr<CConfigurationORenderOperation> outputRenderOperationConfiguration = std::static_pointer_cast<CConfigurationORenderOperation>(gameTransitionConfiguration->getORenderOperationConfiguration());
    std::shared_ptr<CConfigurationMaterial> outputRenderOperationMaterialConfiguration = std::static_pointer_cast<CConfigurationMaterial>(outputRenderOperationConfiguration->getMaterialConfiguration());
    assert(outputRenderOperationMaterialConfiguration != nullptr);
    
    std::shared_ptr<CMaterial> outputRenderOperationMaterial = std::make_shared<CMaterial>();
    
    assert(outputRenderOperationMaterialConfiguration != nullptr);
    assert(m_screenSpaceTextureAccessor != nullptr);
	assert(m_resourceAccessor != nullptr);
    
    CMaterial::setupMaterial(outputRenderOperationMaterial,
                             outputRenderOperationMaterialConfiguration,
                             m_resourceAccessor,
                             m_screenSpaceTextureAccessor);
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
        numTriangles += 0;//gameObject->Get_NumTriangles();
    }
    return numTriangles;
}

ui32 IGameTransition::getWindowWidth(void) const
{
    return m_renderMgr->getWidth();
}

ui32 IGameTransition::getWindowHeight(void) const
{
    return m_renderMgr->getHeight();
}

