//
//  CEConfigurationCustomModelLoadingOperation.cpp
//  ie3D-Engine
//
//  Created by sergey.sergeev on 10/28/14.
//  Copyright (c) 2014 sergey.sergeev. All rights reserved.
//

#include "CEConfigurationCustomModelLoadingOperation.h"
#include "CEConfigurationCustomModelSerializer.h"
#include "CEConfigurationGameObjects.h"
#include "CConfigurationMaterialLoadingOperation.h"

CEConfigurationCustomModelLoadingOperation::CEConfigurationCustomModelLoadingOperation(void)
{
    
}

CEConfigurationCustomModelLoadingOperation::~CEConfigurationCustomModelLoadingOperation(void)
{
    
}

ISharedConfiguration CEConfigurationCustomModelLoadingOperation::serialize(const std::string& filename)
{
    std::shared_ptr<CEConfigurationCustomModelSerializer> customModelSerializer = std::make_shared<CEConfigurationCustomModelSerializer>();
    std::shared_ptr<CEConfigurationCustomModel> customModelConfiguration = std::static_pointer_cast<CEConfigurationCustomModel>(customModelSerializer->serialize(filename));
    assert(customModelConfiguration != nullptr);
    CConfigurationMaterialLoadingOperation::serializeGameObjectMaterialsConfigurations(customModelConfiguration);
    return customModelConfiguration;
}