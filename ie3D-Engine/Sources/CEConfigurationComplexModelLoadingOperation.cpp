//
//  CEConfigurationComplexModelLoadingOperation.cpp
//  ie3D-Engine
//
//  Created by sergey.sergeev on 8/6/14.
//  Copyright (c) 2014 sergey.sergeev. All rights reserved.
//

#include "CEConfigurationComplexModelLoadingOperation.h"
#include "CConfigurationModelLoadingOperation.h"
#include "CEConfigurationComplexModelSerializer.h"
#include "CEConfigurationGameObjects.h"

CEConfigurationComplexModelLoadingOperation::CEConfigurationComplexModelLoadingOperation(void)
{
    
}

CEConfigurationComplexModelLoadingOperation::~CEConfigurationComplexModelLoadingOperation(void)
{
    
}

ISharedConfiguration CEConfigurationComplexModelLoadingOperation::serialize(const std::string& filename)
{
    std::shared_ptr<CEConfigurationComplexModelSerializer> complexModelSerializer = std::make_shared<CEConfigurationComplexModelSerializer>();
    std::shared_ptr<CEConfigurationComplexModel> complexModelConfiguration = std::static_pointer_cast<CEConfigurationComplexModel>(complexModelSerializer->serialize(filename));
    assert(complexModelConfiguration != nullptr);
    std::vector<std::string> modelsConfigurationsFilenames = complexModelConfiguration->getModelsConfigurationsFilenames();
    for(const auto& iterator : modelsConfigurationsFilenames)
    {
        std::shared_ptr<CConfigurationModelLoadingOperation> modelLoadingOperation = std::make_shared<CConfigurationModelLoadingOperation>();
        std::shared_ptr<CConfigurationModel> modelConfiguration = std::static_pointer_cast<CConfigurationModel>(modelLoadingOperation->serialize(iterator));
        assert(modelConfiguration != nullptr);
        complexModelConfiguration->setConfiguration(getConfigurationAttributeKey(complexModelConfiguration->kComplexModelsConfigurationsNode,
                                                                                 complexModelConfiguration->kComplexModelConfigurationsNode,
                                                                                 complexModelConfiguration->kComplexModelFilenameAttribute),
                                                    modelConfiguration);
    }
    return complexModelConfiguration;
}