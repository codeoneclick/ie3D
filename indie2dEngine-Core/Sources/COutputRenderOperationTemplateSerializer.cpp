//
//  COutputRenderOperationTemplateSerializer.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 6/7/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "COutputRenderOperationTemplateSerializer.h"
#include "ITemplate.h"

COutputRenderOperationTemplateSerializer::COutputRenderOperationTemplateSerializer(void)
{
    
}

COutputRenderOperationTemplateSerializer::~COutputRenderOperationTemplateSerializer(void)
{
    
}

std::shared_ptr<ITemplate> COutputRenderOperationTemplateSerializer::Serialize(const std::string& _filename)
{
    pugi::xml_document document;
    pugi::xml_parse_result result = ITemplateSerializer::_LoadDocument(document, _filename);
    assert(result.status == pugi::status_ok);
    pugi::xml_node node = document.child("operation");
    
    std::shared_ptr<SOutputRenderOperationTemplate> outputRenderOperationTemplate = std::make_shared<SOutputRenderOperationTemplate>();
    outputRenderOperationTemplate->m_guid = node.attribute("guid").as_string();
    
    pugi::xml_node material_node = node.child("material");
    outputRenderOperationTemplate->m_materialTemplateFilename = material_node.attribute("filename").as_string();
    return outputRenderOperationTemplate;
}

std::shared_ptr<ITemplate> COutputRenderOperationTemplateSerializer::Serialize(const std::string& _host, ui32 _port, const std::string& _filename)
{
    return nullptr;
}

void COutputRenderOperationTemplateSerializer::Deserialize(const std::string& _filename, std::shared_ptr<ITemplate> _template)
{
    
}