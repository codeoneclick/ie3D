//
//  CScreenSpaceOperationTemplateSerializer.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 6/7/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CConfigurationSSRenderOperationSerializer.h"
#include "CTemplateGameObjects.h"

CConfigurationSSRenderOperationSerializer::CConfigurationSSRenderOperationSerializer(void)
{
    
}

CConfigurationSSRenderOperationSerializer::~CConfigurationSSRenderOperationSerializer(void)
{
    
}

std::shared_ptr<I_RO_TemplateCommon> CConfigurationSSRenderOperationSerializer::Serialize(const std::string& _filename)
{
    pugi::xml_document document;
    pugi::xml_parse_result result = ITemplateSerializer::_LoadDocument(document, _filename);
    assert(result.status == pugi::status_ok);
    
    std::shared_ptr<CConfigurationSSRenderOperation> screenSpaceRenderOperationTemplate = std::make_shared<CConfigurationSSRenderOperation>();
    pugi::xml_node node = document.child(screenSpaceRenderOperationTemplate->kScreenSpaceRenderOperationMainNode.c_str());
    
    std::string guid = node.attribute(screenSpaceRenderOperationTemplate->kScreenSpaceRenderOperationGuidAttribute.c_str()).as_string();
    screenSpaceRenderOperationTemplate->Set_Attribute(Get_TemplateAttributeKey(screenSpaceRenderOperationTemplate->kScreenSpaceRenderOperationMainNode,
                                                                               screenSpaceRenderOperationTemplate->kScreenSpaceRenderOperationGuidAttribute),
                                                      E_TEMPLATE_META_TYPE_STRING,
                                                      &guid);

    ui32 screenWidth = node.attribute(screenSpaceRenderOperationTemplate->kScreenSpaceRenderOperationScreenWidthAttribute.c_str()).as_uint();
    screenSpaceRenderOperationTemplate->Set_Attribute(Get_TemplateAttributeKey(screenSpaceRenderOperationTemplate->kScreenSpaceRenderOperationMainNode,
                                                                               screenSpaceRenderOperationTemplate->kScreenSpaceRenderOperationScreenWidthAttribute),
                                                      E_TEMPLATE_META_TYPE_UI32,
                                                      &screenWidth);

    
    ui32 screenHeight = node.attribute(screenSpaceRenderOperationTemplate->kScreenSpaceRenderOperationScreenHeightAttribute.c_str()).as_uint();
    screenSpaceRenderOperationTemplate->Set_Attribute(Get_TemplateAttributeKey(screenSpaceRenderOperationTemplate->kScreenSpaceRenderOperationMainNode,
                                                                               screenSpaceRenderOperationTemplate->kScreenSpaceRenderOperationScreenHeightAttribute),
                                                      E_TEMPLATE_META_TYPE_UI32,
                                                      &screenHeight);
    
    std::string materialFilename = node.attribute(screenSpaceRenderOperationTemplate->kScreenSpaceRenderOperationMaterialTemplateFilenameAttribute.c_str()).as_string();
    screenSpaceRenderOperationTemplate->Set_Attribute(Get_TemplateAttributeKey(screenSpaceRenderOperationTemplate->kScreenSpaceRenderOperationMainNode,
                                                                               screenSpaceRenderOperationTemplate->kScreenSpaceRenderOperationMaterialTemplateFilenameAttribute),
                                                      E_TEMPLATE_META_TYPE_STRING,
                                                      &materialFilename);
    return screenSpaceRenderOperationTemplate;
}

std::shared_ptr<I_RO_TemplateCommon> CConfigurationSSRenderOperationSerializer::Serialize(const std::string& _host, ui32 _port, const std::string& _filename)
{
    return nullptr;
}

void CConfigurationSSRenderOperationSerializer::Deserialize(const std::string& _filename, std::shared_ptr<I_RO_TemplateCommon> _template)
{
    
}