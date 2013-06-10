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
#include "ITemplate.h"

CGameTransitionTemplateLoadingOperation::CGameTransitionTemplateLoadingOperation(void)
{
    
}

CGameTransitionTemplateLoadingOperation::~CGameTransitionTemplateLoadingOperation(void)
{
    
}

std::shared_ptr<ITemplate> CGameTransitionTemplateLoadingOperation::Serialize(const std::string& _filename)
{
    std::shared_ptr<CGameTransitionTemplateSerializer> gameTransitionTemplateSerializer = std::make_shared<CGameTransitionTemplateSerializer>();
    std::shared_ptr<SGameTransitionTemplate> gameTransitionTemplate = std::static_pointer_cast<SGameTransitionTemplate>(gameTransitionTemplateSerializer->Serialize(_filename));
    assert(gameTransitionTemplate != nullptr);
    for(auto iterator : gameTransitionTemplate->m_worldSpaceRenderOperationsTemplatesFilenames)
    {
        std::shared_ptr<CWorldSpaceRenderOperationTemplateLoadingOperation> worldSpaceRenderOperationTemplateLoadingOperation = std::make_shared<CWorldSpaceRenderOperationTemplateLoadingOperation>();
        std::shared_ptr<SWorldSpaceRenderOperationTemplate> worldSpaceRenderOperationTemplate = std::static_pointer_cast<SWorldSpaceRenderOperationTemplate>(worldSpaceRenderOperationTemplateLoadingOperation->Serialize(iterator));
        assert(worldSpaceRenderOperationTemplate != nullptr);
        gameTransitionTemplate->m_worldSpaceRenderOperationsTemplates.push_back(worldSpaceRenderOperationTemplate);
    }
    for(auto iterator : gameTransitionTemplate->m_screenSpaceRenderOperationsTemplatesFilenames)
    {
        std::shared_ptr<CScreenSpaceRenderOperationTemplateLoadingOperation> screenSpaceRenderOperationTemplateLoadingOperation = std::make_shared<CScreenSpaceRenderOperationTemplateLoadingOperation>();
        std::shared_ptr<SScreenSpaceRenderOperationTemplate> screenSpaceRenderOperationTemplate = std::static_pointer_cast<SScreenSpaceRenderOperationTemplate>(screenSpaceRenderOperationTemplateLoadingOperation->Serialize(iterator));
        assert(screenSpaceRenderOperationTemplate != nullptr);
        gameTransitionTemplate->m_screenSpaceRenderOperationsTemplates.push_back(screenSpaceRenderOperationTemplate);
    }
    
    std::shared_ptr<COutputRenderOperationTemplateLoadingOperation> outputRenderOperationTemplateLoadingOperation = std::make_shared<COutputRenderOperationTemplateLoadingOperation>();
    std::shared_ptr<SOutputRenderOperationTemplate> outputRenderOperationTemplate = std::static_pointer_cast<SOutputRenderOperationTemplate>(outputRenderOperationTemplateLoadingOperation->Serialize(gameTransitionTemplate->m_outputRenderOperationTemplateFilename));
    assert(outputRenderOperationTemplate != nullptr);
    gameTransitionTemplate->m_outputRenderOperationTemplate = outputRenderOperationTemplate;
    return gameTransitionTemplate;
}

