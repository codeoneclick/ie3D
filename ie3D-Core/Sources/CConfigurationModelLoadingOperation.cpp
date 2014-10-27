//
//  CConfigurationModelLoadingOperation.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/29/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CConfigurationModelLoadingOperation.h"
#include "CConfigurationModelSerializer.h"
#include "CConfigurationMaterialLoadingOperation.h"
#include "CConfigurationGameObjects.h"

CConfigurationModelLoadingOperation::CConfigurationModelLoadingOperation(void)
{
    
}

CConfigurationModelLoadingOperation::~CConfigurationModelLoadingOperation(void)
{
    
}

ISharedConfiguration CConfigurationModelLoadingOperation::serialize(const std::string& filename)
{
    std::shared_ptr<CConfigurationModelSerializer> modelSerializer = std::make_shared<CConfigurationModelSerializer>();
    std::shared_ptr<CConfigurationModel> modelConfiguration = std::static_pointer_cast<CConfigurationModel>(modelSerializer->serialize(filename));
    assert(modelConfiguration != nullptr);
    CConfigurationMaterialLoadingOperation::serializeGameObjectMaterialsConfigurations(modelConfiguration);
    return modelConfiguration;
}