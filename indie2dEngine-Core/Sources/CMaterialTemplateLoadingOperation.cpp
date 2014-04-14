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

std::shared_ptr<I_RO_TemplateCommon> CConfigurationMaterialLoadingOperation::Serialize(const std::string& _filename)
{
    std::shared_ptr<CConfigurationMaterialSerializer> serializer = std::make_shared<CConfigurationMaterialSerializer>();
    return serializer->Serialize(_filename);
}