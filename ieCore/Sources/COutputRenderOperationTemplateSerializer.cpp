//
//  COutputRenderOperationTemplateSerializer.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 6/7/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "COutputRenderOperationTemplateSerializer.h"
#include "CTemplateGameObjects.h"

COutputRenderOperationTemplateSerializer::COutputRenderOperationTemplateSerializer(void)
{
    
}

COutputRenderOperationTemplateSerializer::~COutputRenderOperationTemplateSerializer(void)
{
    
}

std::shared_ptr<I_RO_TemplateCommon> COutputRenderOperationTemplateSerializer::Serialize(const std::string& _filename)
{
    pugi::xml_document document;
    pugi::xml_parse_result result = ITemplateSerializer::_LoadDocument(document, _filename);
    assert(result.status == pugi::status_ok);
    
    std::shared_ptr<COutputRenderOperationTemplate> outputRenderOperationTemplate = std::make_shared<COutputRenderOperationTemplate>();
    pugi::xml_node node = document.child(outputRenderOperationTemplate->kOutputRenderOperationMainNode.c_str());
    
    std::string guid = node.attribute(outputRenderOperationTemplate->kOutputRenderOperationGuidAttribute.c_str()).as_string();
    outputRenderOperationTemplate->Set_Attribute(Get_TemplateAttributeKey(outputRenderOperationTemplate->kOutputRenderOperationMainNode,
                                                                          outputRenderOperationTemplate->kOutputRenderOperationGuidAttribute),
                                                 E_TEMPLATE_META_TYPE_STRING,
                                                 &guid);
    
    std::string materialFilename = node.attribute(outputRenderOperationTemplate->kOutputRenderOperationMaterialTemplateFilenameAttribute.c_str()).as_string();
    outputRenderOperationTemplate->Set_Attribute(Get_TemplateAttributeKey(outputRenderOperationTemplate->kOutputRenderOperationMainNode,
                                                                          outputRenderOperationTemplate->kOutputRenderOperationMaterialTemplateFilenameAttribute),
                                                 E_TEMPLATE_META_TYPE_STRING,
                                                 &materialFilename);

    return outputRenderOperationTemplate;
}

std::shared_ptr<I_RO_TemplateCommon> COutputRenderOperationTemplateSerializer::Serialize(const std::string& _host, ui32 _port, const std::string& _filename)
{
    return nullptr;
}

void COutputRenderOperationTemplateSerializer::Deserialize(const std::string& _filename, std::shared_ptr<I_RO_TemplateCommon> _template)
{
    
}