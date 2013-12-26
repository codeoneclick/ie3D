//
//  CMaterialTemplateLoadingOperation.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/29/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CMaterialTemplateLoadingOperation.h"
#include "CMaterialTemplateSerializer.h"

CMaterialTemplateLoadingOperation::CMaterialTemplateLoadingOperation(void)
{
    
}

CMaterialTemplateLoadingOperation::~CMaterialTemplateLoadingOperation(void)
{
    
}

std::shared_ptr<I_RO_TemplateCommon> CMaterialTemplateLoadingOperation::Serialize(const std::string& _filename)
{
    std::shared_ptr<CMaterialTemplateSerializer> serializer = std::make_shared<CMaterialTemplateSerializer>();
    return serializer->Serialize(_filename);
}