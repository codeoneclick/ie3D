//
//  CWorldSpaceOperationTemplateSerializer.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 6/7/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CWorldSpaceRenderOperationTemplateSerializer.h"
#include "CCommonOS.h"
#include "ITemplate.h"

CWorldSpaceRenderOperationTemplateSerializer::CWorldSpaceRenderOperationTemplateSerializer(void)
{
    
}

CWorldSpaceRenderOperationTemplateSerializer::~CWorldSpaceRenderOperationTemplateSerializer(void)
{
    
}

std::shared_ptr<ITemplate> CWorldSpaceRenderOperationTemplateSerializer::Serialize(const std::string& _filename)
{
    std::string path(Get_BundlePath());
    path.append(_filename);
    pugi::xml_document document;
    pugi::xml_parse_result result = document.load_file(path.c_str());
    assert(result.status == pugi::status_ok);
    pugi::xml_node node = document.child("operation");
    
    std::shared_ptr<SWorldSpaceRenderOperationTemplate> worldSpaceRenderOperationTemplate = std::make_shared<SWorldSpaceRenderOperationTemplate>();
    worldSpaceRenderOperationTemplate->m_guid = node.attribute("guid").as_string();
    
    return worldSpaceRenderOperationTemplate;
}

std::shared_ptr<ITemplate> CWorldSpaceRenderOperationTemplateSerializer::Serialize(const std::string& _host, ui32 _port, const std::string& _filename)
{
    return nullptr;
}