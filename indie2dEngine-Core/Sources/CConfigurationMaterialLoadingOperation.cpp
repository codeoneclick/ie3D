//
//  CConfigurationMaterialLoadingOperation.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/29/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CConfigurationMaterialLoadingOperation.h"
#include "CConfigurationMaterialSerializer.h"

CConfigurationMaterialLoadingOperation::CConfigurationMaterialLoadingOperation(void)
{
    
}

CConfigurationMaterialLoadingOperation::~CConfigurationMaterialLoadingOperation(void)
{
    
}

ISharedConfiguration CConfigurationMaterialLoadingOperation::serialize(const std::string& filename)
{
    std::shared_ptr<CConfigurationMaterialSerializer> serializer = std::make_shared<CConfigurationMaterialSerializer>();
    return serializer->serialize(filename);
}