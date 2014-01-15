//
//  CWorldSpaceOperationTemplateSerializer.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 6/7/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CWorldSpaceRenderOperationTemplateSerializer.h"
#include "CTemplateGameObjects.h"

CWorldSpaceRenderOperationTemplateSerializer::CWorldSpaceRenderOperationTemplateSerializer(void)
{
    
}

CWorldSpaceRenderOperationTemplateSerializer::~CWorldSpaceRenderOperationTemplateSerializer(void)
{
    
}

std::shared_ptr<I_RO_TemplateCommon> CWorldSpaceRenderOperationTemplateSerializer::Serialize(const std::string& _filename)
{
    pugi::xml_document document;
    pugi::xml_parse_result result = ITemplateSerializer::_LoadDocument(document, _filename);
    assert(result.status == pugi::status_ok);
    
    std::shared_ptr<CWorldSpaceRenderOperationTemplate> worldSpaceRenderOperationTemplate = std::make_shared<CWorldSpaceRenderOperationTemplate>();
    pugi::xml_node node = document.child(worldSpaceRenderOperationTemplate->kWorldSpaceRenderOperationMainNode.c_str());
    
    std::string guid = node.attribute(worldSpaceRenderOperationTemplate->kWorldSpaceRenderOperationGuidAttribute.c_str()).as_string();
    worldSpaceRenderOperationTemplate->Set_Attribute(Get_TemplateAttributeKey(worldSpaceRenderOperationTemplate->kWorldSpaceRenderOperationMainNode,
                                                                               worldSpaceRenderOperationTemplate->kWorldSpaceRenderOperationGuidAttribute),
                                                      E_TEMPLATE_META_TYPE_STRING,
                                                      &guid);
    
    ui32 index = node.attribute(worldSpaceRenderOperationTemplate->kWorldSpaceRenderOperationIndexAttribute.c_str()).as_uint();
    worldSpaceRenderOperationTemplate->Set_Attribute(Get_TemplateAttributeKey(worldSpaceRenderOperationTemplate->kWorldSpaceRenderOperationMainNode,
                                                                              worldSpaceRenderOperationTemplate->kWorldSpaceRenderOperationIndexAttribute),
                                                     E_TEMPLATE_META_TYPE_UI32,
                                                     &index);
    
    ui32 screenWidth = node.attribute(worldSpaceRenderOperationTemplate->kWorldSpaceRenderOperationScreenWidthAttribute.c_str()).as_uint();
    worldSpaceRenderOperationTemplate->Set_Attribute(Get_TemplateAttributeKey(worldSpaceRenderOperationTemplate->kWorldSpaceRenderOperationMainNode,
                                                                              worldSpaceRenderOperationTemplate->kWorldSpaceRenderOperationScreenWidthAttribute),
                                                     E_TEMPLATE_META_TYPE_UI32,
                                                     &screenWidth);
    
    ui32 screenHeight = node.attribute(worldSpaceRenderOperationTemplate->kWorldSpaceRenderOperationScreenHeightAttribute.c_str()).as_uint();
    worldSpaceRenderOperationTemplate->Set_Attribute(Get_TemplateAttributeKey(worldSpaceRenderOperationTemplate->kWorldSpaceRenderOperationMainNode,
                                                                              worldSpaceRenderOperationTemplate->kWorldSpaceRenderOperationScreenHeightAttribute),
                                                     E_TEMPLATE_META_TYPE_UI32,
                                                     &screenHeight);
    
    f32 clearColorR = node.attribute(worldSpaceRenderOperationTemplate->kWorldSpaceRenderOperationClearColorRAttribute.c_str()).as_uint();
    worldSpaceRenderOperationTemplate->Set_Attribute(Get_TemplateAttributeKey(worldSpaceRenderOperationTemplate->kWorldSpaceRenderOperationMainNode,
                                                                              worldSpaceRenderOperationTemplate->kWorldSpaceRenderOperationClearColorRAttribute),
                                                     E_TEMPLATE_META_TYPE_F32,
                                                     &clearColorR);
    
    f32 clearColorG = node.attribute(worldSpaceRenderOperationTemplate->kWorldSpaceRenderOperationClearColorGAttribute.c_str()).as_uint();
    worldSpaceRenderOperationTemplate->Set_Attribute(Get_TemplateAttributeKey(worldSpaceRenderOperationTemplate->kWorldSpaceRenderOperationMainNode,
                                                                              worldSpaceRenderOperationTemplate->kWorldSpaceRenderOperationClearColorGAttribute),
                                                     E_TEMPLATE_META_TYPE_F32,
                                                     &clearColorG);
    
    f32 clearColorB = node.attribute(worldSpaceRenderOperationTemplate->kWorldSpaceRenderOperationClearColorBAttribute.c_str()).as_uint();
    worldSpaceRenderOperationTemplate->Set_Attribute(Get_TemplateAttributeKey(worldSpaceRenderOperationTemplate->kWorldSpaceRenderOperationMainNode,
                                                                              worldSpaceRenderOperationTemplate->kWorldSpaceRenderOperationClearColorBAttribute),
                                                     E_TEMPLATE_META_TYPE_F32,
                                                     &clearColorB);
    
    f32 clearColorA = node.attribute(worldSpaceRenderOperationTemplate->kWorldSpaceRenderOperationClearColorAAttribute.c_str()).as_uint();
    worldSpaceRenderOperationTemplate->Set_Attribute(Get_TemplateAttributeKey(worldSpaceRenderOperationTemplate->kWorldSpaceRenderOperationMainNode,
                                                                              worldSpaceRenderOperationTemplate->kWorldSpaceRenderOperationClearColorAAttribute),
                                                     E_TEMPLATE_META_TYPE_F32,
                                                     &clearColorA);
    return worldSpaceRenderOperationTemplate;
}

std::shared_ptr<I_RO_TemplateCommon> CWorldSpaceRenderOperationTemplateSerializer::Serialize(const std::string& _host, ui32 _port, const std::string& _filename)
{
    return nullptr;
}

void CWorldSpaceRenderOperationTemplateSerializer::Deserialize(const std::string& _filename, std::shared_ptr<I_RO_TemplateCommon> _template)
{
    
}