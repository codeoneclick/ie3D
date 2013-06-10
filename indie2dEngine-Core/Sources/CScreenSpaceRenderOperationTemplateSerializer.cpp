//
//  CScreenSpaceOperationTemplateSerializer.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 6/7/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CScreenSpaceRenderOperationTemplateSerializer.h"
#include "CCommonOS.h"
#include "ITemplate.h"

CScreenSpaceRenderOperationTemplateSerializer::CScreenSpaceRenderOperationTemplateSerializer(void)
{
    
}

CScreenSpaceRenderOperationTemplateSerializer::~CScreenSpaceRenderOperationTemplateSerializer(void)
{
    
}

std::shared_ptr<ITemplate> CScreenSpaceRenderOperationTemplateSerializer::Serialize(const std::string& _filename)
{
    std::string path(Get_BundlePath());
    path.append(_filename);
    pugi::xml_document document;
    pugi::xml_parse_result result = document.load_file(path.c_str());
    assert(result.status == pugi::status_ok);
    pugi::xml_node node = document.child("operation");
    
    std::shared_ptr<SScreenSpaceRenderOperationTemplate> screenSpaceRenderOperationTemplate = std::make_shared<SScreenSpaceRenderOperationTemplate>();
    screenSpaceRenderOperationTemplate->m_guid = node.attribute("guid").as_string();
    
    pugi::xml_node material_node = node.child("material");
    screenSpaceRenderOperationTemplate->m_materialTemplateFilename = material_node.attribute("filename").as_string();
    return screenSpaceRenderOperationTemplate;
}

std::shared_ptr<ITemplate> CScreenSpaceRenderOperationTemplateSerializer::Serialize(const std::string& _host, ui32 _port, const std::string& _filename)
{
    return nullptr;
}