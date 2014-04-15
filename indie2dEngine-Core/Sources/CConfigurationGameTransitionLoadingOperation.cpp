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
#include "CConfigurationGameObjects.h"

CConfigurationGameTransitionLoadingOperation::CConfigurationGameTransitionLoadingOperation(void)
{
    
}

CConfigurationGameTransitionLoadingOperation::~CConfigurationGameTransitionLoadingOperation(void)
{
    
}

ISharedConfiguration CConfigurationGameTransitionLoadingOperation::serialize(const std::string& filename)
{
    std::shared_ptr<CConfigurationGameTransitionSerializer> gameTransitionConfigurationSerializer = std::make_shared<CConfigurationGameTransitionSerializer>();
    std::shared_ptr<CConfigurationGameTransition> gameTransitionConfiguration = std::static_pointer_cast<CConfigurationGameTransition>(gameTransitionConfigurationSerializer->serialize(filename));
    assert(gameTransitionConfiguration != nullptr);
    
    std::vector<std::string> worldSpaceOperationsConfigurationsFilenames = gameTransitionConfiguration->getWSRenderOperationsConfigurationsFilenames();
    for(const auto& iterator : worldSpaceOperationsConfigurationsFilenames)
    {

        std::shared_ptr<CConfigurationWSRenderOperationLoadingOperation> worldSpaceRenderOperationConfigurationLoadingOperation = std::make_shared<CConfigurationWSRenderOperationLoadingOperation>();
        std::shared_ptr<CConfigurationWSRenderOperation> worldSpaceRenderOperationConfiguration = std::static_pointer_cast<CConfigurationWSRenderOperation>(worldSpaceRenderOperationConfigurationLoadingOperation->serialize(iterator));
        gameTransitionConfiguration->setConfiguration(getConfigurationAttributeKey(gameTransitionConfiguration->kGameTransitionMainNode,
                                                                      gameTransitionConfiguration->kGameTransitionWSRenderOperationsConfigurationsNode,
                                                                      gameTransitionConfiguration->kGameTransitionWSRenderOperationConfigurationNode,
                                                                      gameTransitionConfiguration->kGameTransitionWSRenderOperationConfigurationFilenameAttribute),
                                             worldSpaceRenderOperationConfiguration);
    }
    
    std::vector<std::string> screenSpaceOperationsConfigurationsFilenames = gameTransitionConfiguration->getSSRenderOperationsConfigurationsFilenames();
    for(const auto& iterator : screenSpaceOperationsConfigurationsFilenames)
    {
        
        std::shared_ptr<CConfigurationSSRenderOperationLoadingOperation> screenSpaceRenderOperationConfigurationLoadingOperation = std::make_shared<CConfigurationSSRenderOperationLoadingOperation>();
        std::shared_ptr<CConfigurationSSRenderOperation> screenSpaceRenderOperationConfiguration = std::static_pointer_cast<CConfigurationSSRenderOperation>(screenSpaceRenderOperationConfigurationLoadingOperation->serialize(iterator));
        gameTransitionConfiguration->setConfiguration(getConfigurationAttributeKey(gameTransitionConfiguration->kGameTransitionMainNode,
                                                                      gameTransitionConfiguration->kGameTransitionSSRenderOperationsConfigurationsNode,
                                                                      gameTransitionConfiguration->kGameTransitionSSRenderOperationConfigurationNode,
                                                                      gameTransitionConfiguration->kGameTransitionSSRenderOperationConfigurationFilenameAttribute),
                                             screenSpaceRenderOperationConfiguration);
    }
    
    std::shared_ptr<CConfigurationORenderOperationLoadingOperation> outputRenderOperationConfigurationLoadingOperation = std::make_shared<CConfigurationORenderOperationLoadingOperation>();
    std::shared_ptr<CConfigurationORenderOperation> outputRenderOperationConfiguration = std::static_pointer_cast<CConfigurationORenderOperation>(outputRenderOperationConfigurationLoadingOperation->serialize(gameTransitionConfiguration->getORenderOperationConfigurationFilename()));
    assert(outputRenderOperationConfiguration != nullptr);
    gameTransitionConfiguration->setConfiguration(getConfigurationAttributeKey(gameTransitionConfiguration->kGameTransitionMainNode,
                                                                  gameTransitionConfiguration->kGameTransitionORenderOperationConfigurationNode,
                                                                  gameTransitionConfiguration->kGameTransitionORenderOperationConfigurationFilenameAttribute),
                                         outputRenderOperationConfiguration);
    return gameTransitionConfiguration;
}

