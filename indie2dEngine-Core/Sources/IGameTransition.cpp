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

IGameTransition::IGameTransition(const std::string& _filename, std::shared_ptr<IGraphicsContext> _graphicsContext, std::shared_ptr<IInputContext> _inputContext, std::shared_ptr<CResourceAccessor> _resourceAccessor, std::shared_ptr<CConfigurationAccessor> _templateAccessor) :
IFabricator(_templateAccessor, _resourceAccessor),
CSceneFabricator(_templateAccessor, _resourceAccessor),
//CGUIFabricator(_templateAccessor, _resourceAccessor),
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

void IGameTransition::onConfigurationLoaded(ISharedConfigurationRef configuration, bool success)
{
    assert(m_renderMgr != nullptr);
    assert(m_resourceAccessor != nullptr);
    
    std::shared_ptr<CConfigurationGameTransition> gameTransitionTemplate = std::static_pointer_cast<CConfigurationGameTransition>(configuration);
    assert(gameTransitionTemplate != nullptr);
    
    for(const auto& iterator : gameTransitionTemplate->getWSRenderOperationsConfigurations())
    {
        std::shared_ptr<CConfigurationWSRenderOperation> worldSpaceRenderOperationTemplate = std::static_pointer_cast<CConfigurationWSRenderOperation>(iterator);
        std::shared_ptr<CRenderOperationWorldSpace> worldSpaceRenderOperation =
        std::make_shared<CRenderOperationWorldSpace>(worldSpaceRenderOperationTemplate->getScreenWidth(),
                                                     worldSpaceRenderOperationTemplate->getScreenHeight(),
                                                     worldSpaceRenderOperationTemplate->getClearColor(),
                                                     worldSpaceRenderOperationTemplate->getGuid(),
                                                     worldSpaceRenderOperationTemplate->getIndex());
        m_renderMgr->RegisterWorldSpaceRenderOperation(worldSpaceRenderOperationTemplate->getGuid(), worldSpaceRenderOperation);
    }
    
    for(const auto& iterator : gameTransitionTemplate->getSSRenderOperationsConfigurations())
    {
        std::shared_ptr<CConfigurationSSRenderOperation> screenSpaceRenderOperationTemplate = std::static_pointer_cast<CConfigurationSSRenderOperation>(iterator);
        std::shared_ptr<CConfigurationMaterial> screenSpaceRenderOperationMaterialTemplate = std::static_pointer_cast<CConfigurationMaterial>(screenSpaceRenderOperationTemplate->getMaterialConfiguration());
        assert(screenSpaceRenderOperationMaterialTemplate != nullptr);
        
        std::shared_ptr<CMaterial> screenSpaceRenderOperationMaterial = std::make_shared<CMaterial>();
        
        assert(screenSpaceRenderOperationMaterialTemplate != nullptr);
        assert(m_screenSpaceTextureAccessor != nullptr);
        assert(m_resourceAccessor != nullptr);
        
        screenSpaceRenderOperationMaterial->setCulling(screenSpaceRenderOperationMaterialTemplate->isCulling());
        screenSpaceRenderOperationMaterial->setCullingMode(screenSpaceRenderOperationMaterialTemplate->getCullingMode());
        
        screenSpaceRenderOperationMaterial->setBlending(screenSpaceRenderOperationMaterialTemplate->isBlending());
        screenSpaceRenderOperationMaterial->setBlendingFunctionSource(screenSpaceRenderOperationMaterialTemplate->getBlendingFunctionSource());
        screenSpaceRenderOperationMaterial->setBlendingFunctionDestination(screenSpaceRenderOperationMaterialTemplate->getBlendingFunctionDestination());
        
        screenSpaceRenderOperationMaterial->setDepthTest(screenSpaceRenderOperationMaterialTemplate->isDepthTest());
        screenSpaceRenderOperationMaterial->setDepthMask(screenSpaceRenderOperationMaterialTemplate->isDepthMask());
        
        screenSpaceRenderOperationMaterial->setClipping(screenSpaceRenderOperationMaterialTemplate->isClipping());
        screenSpaceRenderOperationMaterial->setClippingPlane(screenSpaceRenderOperationMaterialTemplate->getClippingPlane());
        
        screenSpaceRenderOperationMaterial->setReflecting(screenSpaceRenderOperationMaterialTemplate->isReflecting());
        screenSpaceRenderOperationMaterial->setShadowing(screenSpaceRenderOperationMaterialTemplate->isShadowing());
        screenSpaceRenderOperationMaterial->setDebugging(screenSpaceRenderOperationMaterialTemplate->isDebugging());
        
        for(const auto& iterator : screenSpaceRenderOperationMaterialTemplate->getTexturesConfigurations())
        {
            CSharedConfigurationTexture textureConfiguration = std::static_pointer_cast<CConfigurationTexture>(iterator);
            assert(textureConfiguration != nullptr);
            
            CSharedTexture texture = textureConfiguration->getFilename().length() != 0 ?
            m_resourceAccessor->getTexture(textureConfiguration->getFilename()) :
            m_screenSpaceTextureAccessor->Get_RenderOperationTexture(textureConfiguration->getRenderOperationName());
            //texture->registerLoadingHandler(shared_from_this());
            assert(texture != nullptr);
            texture->setWrapMode(textureConfiguration->getWrapMode());
            assert(textureConfiguration->getSamplerIndex() >= 0 &&
                   textureConfiguration->getSamplerIndex() < E_SHADER_SAMPLER_MAX);
            screenSpaceRenderOperationMaterial->setTexture(texture, static_cast<E_SHADER_SAMPLER>(textureConfiguration->getSamplerIndex()));
        }
        
        CSharedConfigurationShader shaderConfiguration = std::static_pointer_cast<CConfigurationShader>(screenSpaceRenderOperationMaterialTemplate->getShaderConfiguration());
        assert(shaderConfiguration != nullptr);
        CSharedShader shader = m_resourceAccessor->getShader(shaderConfiguration->getVSFilename(),
                                                             shaderConfiguration->getFSFilename());
        assert(shader != nullptr);
        screenSpaceRenderOperationMaterial->setShader(shader);
        //shader->registerLoadingHandler(shared_from_this());

        
        //screenSpaceRenderOperationMaterial->Serialize(screenSpaceRenderOperationMaterialTemplate, m_resourceAccessor, m_renderMgr);
        
        std::shared_ptr<CRenderOperationScreenSpace> screenSpaceRenderOperation =
        std::make_shared<CRenderOperationScreenSpace>(screenSpaceRenderOperationTemplate->getScreenWidth(),
                                                      screenSpaceRenderOperationTemplate->getScreenHeight(),
                                                      screenSpaceRenderOperationTemplate->getGuid(),
                                                      screenSpaceRenderOperationMaterial);
        m_renderMgr->RegisterScreenSpaceRenderOperation( screenSpaceRenderOperationTemplate->getGuid(), screenSpaceRenderOperation);
    }
    
    std::shared_ptr<CConfigurationORenderOperation> outputRenderOperationTemplate = std::static_pointer_cast<CConfigurationORenderOperation>(gameTransitionTemplate->getORenderOperationConfiguration());
    std::shared_ptr<CConfigurationMaterial> outputRenderOperationMaterialTemplate = std::static_pointer_cast<CConfigurationMaterial>(outputRenderOperationTemplate->getMaterialConfiguration());
    assert(outputRenderOperationMaterialTemplate != nullptr);
    
    std::shared_ptr<CMaterial> outputRenderOperationMaterial = std::make_shared<CMaterial>();
    
    assert(outputRenderOperationMaterialTemplate != nullptr);
    assert(m_screenSpaceTextureAccessor != nullptr);
	assert(m_resourceAccessor != nullptr);
    
    outputRenderOperationMaterial->setCulling(outputRenderOperationMaterialTemplate->isCulling());
    outputRenderOperationMaterial->setCullingMode(outputRenderOperationMaterialTemplate->getCullingMode());
    
    outputRenderOperationMaterial->setBlending(outputRenderOperationMaterialTemplate->isBlending());
    outputRenderOperationMaterial->setBlendingFunctionSource(outputRenderOperationMaterialTemplate->getBlendingFunctionSource());
    outputRenderOperationMaterial->setBlendingFunctionDestination(outputRenderOperationMaterialTemplate->getBlendingFunctionDestination());
    
    outputRenderOperationMaterial->setDepthTest(outputRenderOperationMaterialTemplate->isDepthTest());
    outputRenderOperationMaterial->setDepthMask(outputRenderOperationMaterialTemplate->isDepthMask());
    
    outputRenderOperationMaterial->setClipping(outputRenderOperationMaterialTemplate->isClipping());
    outputRenderOperationMaterial->setClippingPlane(outputRenderOperationMaterialTemplate->getClippingPlane());
    
    outputRenderOperationMaterial->setReflecting(outputRenderOperationMaterialTemplate->isReflecting());
    outputRenderOperationMaterial->setShadowing(outputRenderOperationMaterialTemplate->isShadowing());
    outputRenderOperationMaterial->setDebugging(outputRenderOperationMaterialTemplate->isDebugging());
    
    for(const auto& iterator : outputRenderOperationMaterialTemplate->getTexturesConfigurations())
    {
        CSharedConfigurationTexture textureConfiguration = std::static_pointer_cast<CConfigurationTexture>(iterator);
        assert(textureConfiguration != nullptr);
        
        CSharedTexture texture = textureConfiguration->getFilename().length() != 0 ?
        m_resourceAccessor->getTexture(textureConfiguration->getFilename()) :
        m_screenSpaceTextureAccessor->Get_RenderOperationTexture(textureConfiguration->getRenderOperationName());
        //texture->registerLoadingHandler(shared_from_this());
        assert(texture != nullptr);
        texture->setWrapMode(textureConfiguration->getWrapMode());
        assert(textureConfiguration->getSamplerIndex() >= 0 &&
               textureConfiguration->getSamplerIndex() < E_SHADER_SAMPLER_MAX);
        outputRenderOperationMaterial->setTexture(texture, static_cast<E_SHADER_SAMPLER>(textureConfiguration->getSamplerIndex()));
    }
    
    CSharedConfigurationShader shaderConfiguration = std::static_pointer_cast<CConfigurationShader>(outputRenderOperationMaterialTemplate->getShaderConfiguration());
    assert(shaderConfiguration != nullptr);
    CSharedShader shader = m_resourceAccessor->getShader(shaderConfiguration->getVSFilename(),
                                                         shaderConfiguration->getFSFilename());
    assert(shader != nullptr);
    outputRenderOperationMaterial->setShader(shader);
    //shader->registerLoadingHandler(shared_from_this());
    
    //outputRenderOperationMaterial->Serialize(outputRenderOperationMaterialTemplate, m_resourceAccessor, m_renderMgr);
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
