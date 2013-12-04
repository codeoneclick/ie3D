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
   pugi::xml_parse_result result = ITemplateSerializer::_LoadDocument(document, path);
    assert(result.status == pugi::status_ok);
    pugi::xml_node node = document.child("operation");
    
    std::shared_ptr<SWorldSpaceRenderOperationTemplate> worldSpaceRenderOperationTemplate = std::make_shared<SWorldSpaceRenderOperationTemplate>();
    worldSpaceRenderOperationTemplate->m_guid = node.attribute("guid").as_string();
    worldSpaceRenderOperationTemplate->m_index = node.attribute("index").as_uint();
    worldSpaceRenderOperationTemplate->m_screenWidth = node.attribute("screenWidth").as_int();
    worldSpaceRenderOperationTemplate->m_screenHeight = node.attribute("screenHeight").as_int();
    worldSpaceRenderOperationTemplate->m_clearColor.r = node.attribute("clear_color_r").as_float();
    worldSpaceRenderOperationTemplate->m_clearColor.g = node.attribute("clear_color_g").as_float();
    worldSpaceRenderOperationTemplate->m_clearColor.b = node.attribute("clear_color_b").as_float();
    worldSpaceRenderOperationTemplate->m_clearColor.a = node.attribute("clear_color_a").as_float();
    return worldSpaceRenderOperationTemplate;
}

std::shared_ptr<ITemplate> CWorldSpaceRenderOperationTemplateSerializer::Serialize(const std::string& _host, ui32 _port, const std::string& _filename)
{
    return nullptr;
}