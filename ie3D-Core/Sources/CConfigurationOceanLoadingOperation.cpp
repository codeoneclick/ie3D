//
//  COceanTeamplateLoadingOperation.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 7/24/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CConfigurationOceanLoadingOperation.h"
#include "CConfigurationMaterialLoadingOperation.h"
#include "CConfigurationOceanSerializer.h"
#include "CConfigurationGameObjects.h"

CConfigurationOceanLoadingOperation::CConfigurationOceanLoadingOperation(void)
{
    
}

CConfigurationOceanLoadingOperation::~CConfigurationOceanLoadingOperation(void)
{
    
}

ISharedConfiguration CConfigurationOceanLoadingOperation::serialize(const std::string& filename)
{
    std::shared_ptr<CConfigurationOceanSerializer> oceanSerializer = std::make_shared<CConfigurationOceanSerializer>();
    std::shared_ptr<CConfigurationOcean> oceanConfiguration = std::static_pointer_cast<CConfigurationOcean>(oceanSerializer->serialize(filename));
    assert(oceanConfiguration != nullptr);
    CConfigurationMaterialLoadingOperation::serializeGameObjectMaterialsConfigurations(oceanConfiguration);
    return oceanConfiguration;
}