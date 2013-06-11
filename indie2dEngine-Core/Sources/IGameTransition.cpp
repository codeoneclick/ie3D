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

#ifdef __APPLE__
#include "CGraphicsContext_iOS.h"
#else
#endif

const std::string g_vsScreenShader = "CommonScreen.vert";
const std::string g_fsScreenShader = "CommonScreen.frag";

IGameTransition::IGameTransition(const std::string& _filename, std::shared_ptr<IGraphicsContext> _graphicsContext, std::shared_ptr<CResourceAccessor> _resourceAccessor, std::shared_ptr<CTemplateAccessor> _templateAccessor) :
CSceneFabricator(_templateAccessor, _resourceAccessor)
{
    assert(_graphicsContext != nullptr);
    assert(_resourceAccessor != nullptr);
    assert(_templateAccessor != nullptr);
    
    _templateAccessor->LoadGameTransitionTemplate(_filename, shared_from_this());
    
    m_renderMgr = std::make_shared<CRenderMgr>(_graphicsContext);
    m_sceneUpdateMgr = std::make_shared<CSceneUpdateMgr>();
    
    /*std::shared_ptr<CShader> shader = m_resourceFabricator->CreateShader(g_vsScreenShader, g_fsScreenShader);
    
    std::shared_ptr<CMaterial> material = std::make_shared<CMaterial>(shader);
    material->Set_RenderState(E_RENDER_STATE_CULL_MODE, false);
    material->Set_RenderState(E_RENDER_STATE_DEPTH_MASK, true);
    material->Set_RenderState(E_RENDER_STATE_DEPTH_TEST, false);
    material->Set_RenderState(E_RENDER_STATE_BLEND_MODE, false);
    
    material->Set_CullFaceMode(GL_FRONT);
    material->Set_BlendFunctionSource(GL_SRC_ALPHA);
    material->Set_BlendFunctionDest(GL_ONE_MINUS_SRC_ALPHA);
    
    m_renderMgr = std::make_shared<CRenderMgr>(glContext, material);
    std::shared_ptr<CRenderOperationWorldSpace> worldSpaceRenderOperation = std::make_shared<CRenderOperationWorldSpace>(Get_ScreenWidth(),
                                                                                                                         Get_ScreenHeight(),
                                                                                                                         "common.world.space.render.operation");
    m_renderMgr->RegisterWorldSpaceRenderOperation("common.world.space.render.operation", worldSpaceRenderOperation);
    
    m_sceneUpdateMgr = std::make_shared<CSceneUpdateMgr>();
    
    ConnectToGameLoop(m_renderMgr);
    ConnectToGameLoop(m_sceneUpdateMgr);*/
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
    }
    
    std::shared_ptr<SMaterialTemplate> outputRenderOperationMaterialTemplate = gameTransitionTemplate->m_outputRenderOperationTemplate->m_materialTemplate;
    assert(outputRenderOperationMaterialTemplate != nullptr);
    
    std::shared_ptr<CShader> outputRenderOperationShader = m_resourceAccessor->CreateShader(outputRenderOperationMaterialTemplate->m_shaderTemplate->m_vsFilename,
                                                                                            outputRenderOperationMaterialTemplate->m_shaderTemplate->m_fsFilename);
    assert(outputRenderOperationShader != nullptr);
    
    std::shared_ptr<CMaterial> outputRenderOperationMaterial = std::make_shared<CMaterial>(outputRenderOperationShader);
    outputRenderOperationMaterial->Serialize(outputRenderOperationMaterialTemplate, m_resourceAccessor, m_renderMgr);
    
    m_renderMgr->RegisterOutputRenderOperation(outputRenderOperationMaterial);

    ConnectToGameLoop(m_renderMgr);
    ConnectToGameLoop(m_sceneUpdateMgr);
}
