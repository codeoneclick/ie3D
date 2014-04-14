//
//  CConfigurationGameTransitionLoadingOperation.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 6/7/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CConfigurationGameTransitionLoadingOperation.h"
#include "CConfigurationWSRenderOperationLoadingOperation.h"
#include "CConfigurationSSRenderOperationLoadingOperation.h"
#include "CConfigurationORenderOperationLoadingOperation.h"
#include "CConfigurationGameTransitionSerializer.h"
#include "CTemplateGameObjects.h"

CConfigurationGameTransitionLoadingOperation::CConfigurationGameTransitionLoadingOperation(void)
{
    
}

CConfigurationGameTransitionLoadingOperation::~CConfigurationGameTransitionLoadingOperation(void)
{
    
}

std::shared_ptr<I_RO_TemplateCommon> CConfigurationGameTransitionLoadingOperation::Serialize(const std::string& _filename)
{
    std::shared_ptr<CConfigurationGameTransitionSerializer> gameTransitionTemplateSerializer = std::make_shared<CConfigurationGameTransitionSerializer>();
    std::shared_ptr<CConfigurationGameTransition> gameTransitionTemplate = std::static_pointer_cast<CConfigurationGameTransition>(gameTransitionTemplateSerializer->Serialize(_filename));
    assert(gameTransitionTemplate != nullptr);
    
    std::vector<std::string> worldSpaceOperationsTemplatesFilenames = gameTransitionTemplate->Get_WorldSpaceRenderOperationsTemplatesFilenames();
    for(const auto& iterator : worldSpaceOperationsTemplatesFilenames)
    {

        std::shared_ptr<CConfigurationWSRenderOperationLoadingOperation> worldSpaceRenderOperationTemplateLoadingOperation = std::make_shared<CConfigurationWSRenderOperationLoadingOperation>();
        std::shared_ptr<CConfigurationWSRenderOperation> worldSpaceRenderOperationTemplate = std::static_pointer_cast<CConfigurationWSRenderOperation>(worldSpaceRenderOperationTemplateLoadingOperation->Serialize(iterator));
        gameTransitionTemplate->Set_Template(getConfigurationAttributeKey(gameTransitionTemplate->kGameTransitionMainNode,
                                                                      gameTransitionTemplate->kGameTransitionWorldSpaceRenderOperationsTemplatesNode,
                                                                      gameTransitionTemplate->kGameTransitionWorldSpaceRenderOperationTemplateNode,
                                                                      gameTransitionTemplate->kGameTransitionWorldSpaceRenderOperationTemplateFilenameAttribute),
                                             worldSpaceRenderOperationTemplate);
    }
    
    std::vector<std::string> screenSpaceOperationsTemplatesFilenames = gameTransitionTemplate->Get_ScreenSpaceRenderOperationsTemplatesFilenames();
    for(const auto& iterator : screenSpaceOperationsTemplatesFilenames)
    {
        
        std::shared_ptr<CConfigurationSSRenderOperationLoadingOperation> screenSpaceRenderOperationTemplateLoadingOperation = std::make_shared<CConfigurationSSRenderOperationLoadingOperation>();
        std::shared_ptr<CConfigurationSSRenderOperation> screenSpaceRenderOperationTemplate = std::static_pointer_cast<CConfigurationSSRenderOperation>(screenSpaceRenderOperationTemplateLoadingOperation->Serialize(iterator));
        gameTransitionTemplate->Set_Template(getConfigurationAttributeKey(gameTransitionTemplate->kGameTransitionMainNode,
                                                                      gameTransitionTemplate->kGameTransitionScreenSpaceRenderOperationsTemplatesNode,
                                                                      gameTransitionTemplate->kGameTransitionScreenSpaceRenderOperationTemplateNode,
                                                                      gameTransitionTemplate->kGameTransitionScreenSpaceRenderOperationTemplateFilenameAttribute),
                                             screenSpaceRenderOperationTemplate);
    }
    
    std::shared_ptr<CConfigurationORenderOperationLoadingOperation> outputRenderOperationTemplateLoadingOperation = std::make_shared<CConfigurationORenderOperationLoadingOperation>();
    std::shared_ptr<CConfigurationORenderOperation> outputRenderOperationTemplate = std::static_pointer_cast<CConfigurationORenderOperation>(outputRenderOperationTemplateLoadingOperation->Serialize(gameTransitionTemplate->Get_OutputRenderOperationTemplateFilename()));
    assert(outputRenderOperationTemplate != nullptr);
    gameTransitionTemplate->Set_Template(getConfigurationAttributeKey(gameTransitionTemplate->kGameTransitionMainNode,
                                                                  gameTransitionTemplate->kGameTransitionOutputRenderOperationTemplateNode,
                                                                  gameTransitionTemplate->kGameTransitionOutputRenderOperationTemplateFilenameAttribute),
                                         outputRenderOperationTemplate);
    return gameTransitionTemplate;
}

