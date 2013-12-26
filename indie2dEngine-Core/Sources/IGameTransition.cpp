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
#include "CBatchingMgr.h"
#include "CSceneUpdateMgr.h"
#include "CCollisionMgr.h"
#include "CRenderOperationWorldSpace.h"
#include "CRenderOperationScreenSpace.h"
#include "CCommonOS.h"
#include "CTemplateGameObjects.h"
#include "IGraphicsContext.h"
#include "IInputContext.h"

IGameTransition::IGameTransition(const std::string& _filename, std::shared_ptr<IGraphicsContext> _graphicsContext, std::shared_ptr<IInputContext> _inputContext, std::shared_ptr<CResourceAccessor> _resourceAccessor, std::shared_ptr<CTemplateAccessor> _templateAccessor) :
IFabricator(_templateAccessor, _resourceAccessor),
CSceneFabricator(_templateAccessor, _resourceAccessor),
CGUIFabricator(_templateAccessor, _resourceAccessor),
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

void IGameTransition::_OnTemplateLoaded(std::shared_ptr<I_RO_TemplateCommon> _template)
{
    assert(m_renderMgr != nullptr);
    assert(m_resourceAccessor != nullptr);
    
    std::shared_ptr<CGameTransitionTemplate> gameTransitionTemplate = std::static_pointer_cast<CGameTransitionTemplate>(_template);
    assert(gameTransitionTemplate != nullptr);
    
    for(const auto& iterator : gameTransitionTemplate->Get_WorldSpaceRenderOperationsTemplates())
    {
        std::shared_ptr<CWorldSpaceRenderOperationTemplate> worldSpaceRenderOperationTemplate = std::static_pointer_cast<CWorldSpaceRenderOperationTemplate>(iterator);
        std::shared_ptr<CRenderOperationWorldSpace> worldSpaceRenderOperation =
        std::make_shared<CRenderOperationWorldSpace>(worldSpaceRenderOperationTemplate->Get_ScreenWidth(),
                                                     worldSpaceRenderOperationTemplate->Get_ScreenHeight(),
                                                     worldSpaceRenderOperationTemplate->Get_ClearColor(),
                                                     worldSpaceRenderOperationTemplate->Get_Guid(),
                                                     worldSpaceRenderOperationTemplate->Get_Index());
        m_renderMgr->RegisterWorldSpaceRenderOperation(worldSpaceRenderOperationTemplate->Get_Guid(), worldSpaceRenderOperation);
    }
    
    for(const auto& iterator : gameTransitionTemplate->Get_ScreenSpaceRenderOperationsTemplates())
    {
        std::shared_ptr<CScreenSpaceRenderOperationTemplate> screenSpaceRenderOperationTemplate = std::static_pointer_cast<CScreenSpaceRenderOperationTemplate>(iterator);
        std::shared_ptr<CMaterialTemplate> screenSpaceRenderOperationMaterialTemplate = std::static_pointer_cast<CMaterialTemplate>(screenSpaceRenderOperationTemplate->Get_MaterialTemplate());
        assert(screenSpaceRenderOperationMaterialTemplate != nullptr);
        
        std::shared_ptr<CMaterial> screenSpaceRenderOperationMaterial = std::make_shared<CMaterial>(screenSpaceRenderOperationMaterialTemplate->Get_RenderOperationName());
        screenSpaceRenderOperationMaterial->Serialize(screenSpaceRenderOperationMaterialTemplate, m_resourceAccessor, m_renderMgr);
        
        std::shared_ptr<CRenderOperationScreenSpace> screenSpaceRenderOperation =
        std::make_shared<CRenderOperationScreenSpace>(screenSpaceRenderOperationTemplate->Get_ScreenWidth(),
                                                      screenSpaceRenderOperationTemplate->Get_ScreenHeight(),
                                                      screenSpaceRenderOperationTemplate->Get_Guid(),
                                                      screenSpaceRenderOperationMaterial);
        m_renderMgr->RegisterScreenSpaceRenderOperation( screenSpaceRenderOperationTemplate->Get_Guid(), screenSpaceRenderOperation);
    }
    
    std::shared_ptr<COutputRenderOperationTemplate> outputRenderOperationTemplate = std::static_pointer_cast<COutputRenderOperationTemplate>(gameTransitionTemplate->Get_OutputRenderOperationTemplate());
    std::shared_ptr<CMaterialTemplate> outputRenderOperationMaterialTemplate = std::static_pointer_cast<CMaterialTemplate>(outputRenderOperationTemplate->Get_MaterialTemplate());
    assert(outputRenderOperationMaterialTemplate != nullptr);
    
    std::shared_ptr<CMaterial> outputRenderOperationMaterial = std::make_shared<CMaterial>(outputRenderOperationMaterialTemplate->Get_RenderOperationName());
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
