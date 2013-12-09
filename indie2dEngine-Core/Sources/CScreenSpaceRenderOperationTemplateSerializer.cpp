//
//  CScreenSpaceOperationTemplateSerializer.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 6/7/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CScreenSpaceRenderOperationTemplateSerializer.h"
#include "ITemplate.h"

CScreenSpaceRenderOperationTemplateSerializer::CScreenSpaceRenderOperationTemplateSerializer(void)
{
    
}

CScreenSpaceRenderOperationTemplateSerializer::~CScreenSpaceRenderOperationTemplateSerializer(void)
{
    
}

std::shared_ptr<ITemplate> CScreenSpaceRenderOperationTemplateSerializer::Serialize(const std::string& _filename)
{
    pugi::xml_document document;
    pugi::xml_parse_result result = ITemplateSerializer::_LoadDocument(document, _filename);
    assert(result.status == pugi::status_ok);
    pugi::xml_node node = document.child("operation");
    
    std::shared_ptr<SScreenSpaceRenderOperationTemplate> screenSpaceRenderOperationTemplate = std::make_shared<SScreenSpaceRenderOperationTemplate>();
    screenSpaceRenderOperationTemplate->m_guid = node.attribute("guid").as_string();
    screenSpaceRenderOperationTemplate->m_screenWidth = node.attribute("screenWidth").as_int();
    screenSpaceRenderOperationTemplate->m_screenHeight = node.attribute("screenHeight").as_int();
    
    pugi::xml_node material_node = node.child("material");
    screenSpaceRenderOperationTemplate->m_materialTemplateFilename = material_node.attribute("filename").as_string();
    return screenSpaceRenderOperationTemplate;
}

std::shared_ptr<ITemplate> CScreenSpaceRenderOperationTemplateSerializer::Serialize(const std::string& _host, ui32 _port, const std::string& _filename)
{
    return nullptr;
}

void CScreenSpaceRenderOperationTemplateSerializer::Deserialize(const std::string& _filename, std::shared_ptr<ITemplate> _template)
{
    
}