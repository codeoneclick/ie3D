//
//  CGameTransitionTemplateLoadingOperation.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 6/7/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CGameTransitionTemplateLoadingOperation.h"
#include "CWorldSpaceRenderOperationTemplateLoadingOperation.h"
#include "CScreenSpaceRenderOperationTemplateLoadingOperation.h"
#include "COutputRenderOperationTemplateLoadingOperation.h"
#include "CGameTransitionTemplateSerializer.h"
#include "CTemplateGameObjects.h"

CGameTransitionTemplateLoadingOperation::CGameTransitionTemplateLoadingOperation(void)
{
    
}

CGameTransitionTemplateLoadingOperation::~CGameTransitionTemplateLoadingOperation(void)
{
    
}

std::shared_ptr<I_RO_TemplateCommon> CGameTransitionTemplateLoadingOperation::Serialize(const std::string& _filename)
{
    std::shared_ptr<CGameTransitionTemplateSerializer> gameTransitionTemplateSerializer = std::make_shared<CGameTransitionTemplateSerializer>();
    std::shared_ptr<CGameTransitionTemplate> gameTransitionTemplate = std::static_pointer_cast<CGameTransitionTemplate>(gameTransitionTemplateSerializer->Serialize(_filename));
    assert(gameTransitionTemplate != nullptr);
    
    std::vector<std::string> worldSpaceOperationsTemplatesFilenames = gameTransitionTemplate->Get_WorldSpaceRenderOperationsTemplatesFilenames();
    for(const auto& iterator : worldSpaceOperationsTemplatesFilenames)
    {

        std::shared_ptr<CWorldSpaceRenderOperationTemplateLoadingOperation> worldSpaceRenderOperationTemplateLoadingOperation = std::make_shared<CWorldSpaceRenderOperationTemplateLoadingOperation>();
        std::shared_ptr<CWorldSpaceRenderOperationTemplate> worldSpaceRenderOperationTemplate = std::static_pointer_cast<CWorldSpaceRenderOperationTemplate>(worldSpaceRenderOperationTemplateLoadingOperation->Serialize(iterator));
        gameTransitionTemplate->Set_Template(Get_TemplateAttributeKey(gameTransitionTemplate->kGameTransitionMainNode,
                                                                      gameTransitionTemplate->kGameTransitionWorldSpaceRenderOperationsTemplatesNode,
                                                                      gameTransitionTemplate->kGameTransitionWorldSpaceRenderOperationTemplateNode,
                                                                      gameTransitionTemplate->kGameTransitionWorldSpaceRenderOperationTemplateFilenameAttribute),
                                             worldSpaceRenderOperationTemplate);
    }
    
    std::vector<std::string> screenSpaceOperationsTemplatesFilenames = gameTransitionTemplate->Get_ScreenSpaceRenderOperationsTemplatesFilenames();
    for(const auto& iterator : screenSpaceOperationsTemplatesFilenames)
    {
        
        std::shared_ptr<CScreenSpaceRenderOperationTemplateLoadingOperation> screenSpaceRenderOperationTemplateLoadingOperation = std::make_shared<CScreenSpaceRenderOperationTemplateLoadingOperation>();
        std::shared_ptr<CScreenSpaceRenderOperationTemplate> screenSpaceRenderOperationTemplate = std::static_pointer_cast<CScreenSpaceRenderOperationTemplate>(screenSpaceRenderOperationTemplateLoadingOperation->Serialize(iterator));
        gameTransitionTemplate->Set_Template(Get_TemplateAttributeKey(gameTransitionTemplate->kGameTransitionMainNode,
                                                                      gameTransitionTemplate->kGameTransitionScreenSpaceRenderOperationsTemplatesNode,
                                                                      gameTransitionTemplate->kGameTransitionScreenSpaceRenderOperationTemplateNode,
                                                                      gameTransitionTemplate->kGameTransitionScreenSpaceRenderOperationTemplateFilenameAttribute),
                                             screenSpaceRenderOperationTemplate);
    }
    
    std::shared_ptr<COutputRenderOperationTemplateLoadingOperation> outputRenderOperationTemplateLoadingOperation = std::make_shared<COutputRenderOperationTemplateLoadingOperation>();
    std::shared_ptr<COutputRenderOperationTemplate> outputRenderOperationTemplate = std::static_pointer_cast<COutputRenderOperationTemplate>(outputRenderOperationTemplateLoadingOperation->Serialize(gameTransitionTemplate->Get_OutputRenderOperationTemplateFilename()));
    assert(outputRenderOperationTemplate != nullptr);
    gameTransitionTemplate->Set_Template(Get_TemplateAttributeKey(gameTransitionTemplate->kGameTransitionMainNode,
                                                                  gameTransitionTemplate->kGameTransitionOutputRenderOperationTemplateNode,
                                                                  gameTransitionTemplate->kGameTransitionOutputRenderOperationTemplateFilenameAttribute),
                                         outputRenderOperationTemplate);
    return gameTransitionTemplate;
}

