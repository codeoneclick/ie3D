//
//  CGameTransitionTemplateSerializer.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 6/7/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CGameTransitionTemplateSerializer.h"
#include "CTemplateGameObjects.h"

CGameTransitionTemplateSerializer::CGameTransitionTemplateSerializer(void)
{
    
}

CGameTransitionTemplateSerializer::~CGameTransitionTemplateSerializer(void)
{
    
}

std::shared_ptr<I_RO_TemplateCommon> CGameTransitionTemplateSerializer::Serialize(const std::string& _filename)
{
    pugi::xml_document document;
    pugi::xml_parse_result result = ITemplateSerializer::_LoadDocument(document, _filename);
    assert(result.status == pugi::status_ok);
    
    std::shared_ptr<CGameTransitionTemplate> gameTransitionTemplate = std::make_shared<CGameTransitionTemplate>();
    pugi::xml_node node = document.child(gameTransitionTemplate->kGameTransitionMainNode.c_str());
    
    std::string guid = node.attribute(gameTransitionTemplate->kGameTransitionGuidAttribute.c_str()).as_string();
    gameTransitionTemplate->Set_Attribute(Get_TemplateAttributeKey(gameTransitionTemplate->kGameTransitionMainNode,
                                                                   gameTransitionTemplate->kGameTransitionGuidAttribute),
                                          E_TEMPLATE_META_TYPE_STRING,
                                          &guid);
    
    pugi::xml_node outputRenderOperationNode = node.child(gameTransitionTemplate->kGameTransitionOutputRenderOperationTemplateNode.c_str());
    std::string outputRenderOperationFilename = outputRenderOperationNode.attribute(gameTransitionTemplate->kGameTransitionOutputRenderOperationTemplateFilenameAttribute.c_str()).as_string();
    
    gameTransitionTemplate->Set_Attribute(Get_TemplateAttributeKey(gameTransitionTemplate->kGameTransitionMainNode,
                                                                   gameTransitionTemplate->kGameTransitionOutputRenderOperationTemplateNode,
                                                                   gameTransitionTemplate->kGameTransitionOutputRenderOperationTemplateFilenameAttribute),
                                          E_TEMPLATE_META_TYPE_STRING,
                                          &outputRenderOperationFilename);
    
    pugi::xml_node worldSpaceRenderOperationsNode = node.child(gameTransitionTemplate->kGameTransitionWorldSpaceRenderOperationsTemplatesNode.c_str());
    for (pugi::xml_node material = worldSpaceRenderOperationsNode.child(gameTransitionTemplate->kGameTransitionWorldSpaceRenderOperationTemplateNode.c_str());
         material;
         material = material.next_sibling(gameTransitionTemplate->kGameTransitionWorldSpaceRenderOperationTemplateNode.c_str()))
    {
        std::string worldSpaceRenderOperationFilename = material.attribute(gameTransitionTemplate->kGameTransitionWorldSpaceRenderOperationTemplateFilenameAttribute.c_str()).as_string();
        gameTransitionTemplate->Set_Attribute(Get_TemplateAttributeKey(gameTransitionTemplate->kGameTransitionMainNode,
                                                                       gameTransitionTemplate->kGameTransitionWorldSpaceRenderOperationsTemplatesNode,
                                                                       gameTransitionTemplate->kGameTransitionWorldSpaceRenderOperationTemplateNode,
                                                                       gameTransitionTemplate->kGameTransitionWorldSpaceRenderOperationTemplateFilenameAttribute),
                                              E_TEMPLATE_META_TYPE_STRING,
                                              &worldSpaceRenderOperationFilename);

    }
    
    pugi::xml_node screenSpaceRenderOperationsNode = node.child(gameTransitionTemplate->kGameTransitionScreenSpaceRenderOperationsTemplatesNode.c_str());
    for (pugi::xml_node material = screenSpaceRenderOperationsNode.child(gameTransitionTemplate->kGameTransitionScreenSpaceRenderOperationTemplateNode.c_str());
         material;
         material = material.next_sibling(gameTransitionTemplate->kGameTransitionScreenSpaceRenderOperationTemplateNode.c_str()))
    {
         std::string screenSpaceRenderOperationFilename = material.attribute(gameTransitionTemplate->kGameTransitionScreenSpaceRenderOperationTemplateFilenameAttribute.c_str()).as_string();
        
        gameTransitionTemplate->Set_Attribute(Get_TemplateAttributeKey(gameTransitionTemplate->kGameTransitionMainNode,
                                                                       gameTransitionTemplate->kGameTransitionScreenSpaceRenderOperationsTemplatesNode,
                                                                       gameTransitionTemplate->kGameTransitionScreenSpaceRenderOperationTemplateNode,
                                                                       gameTransitionTemplate->kGameTransitionScreenSpaceRenderOperationTemplateFilenameAttribute),
                                              E_TEMPLATE_META_TYPE_STRING,
                                              &screenSpaceRenderOperationFilename);
    }
    
    return gameTransitionTemplate;
}

std::shared_ptr<I_RO_TemplateCommon> CGameTransitionTemplateSerializer::Serialize(const std::string& _host, ui32 _port, const std::string& _filename)
{
    return nullptr;
}

void CGameTransitionTemplateSerializer::Deserialize(const std::string& _filename, std::shared_ptr<I_RO_TemplateCommon> _template)
{
    
}