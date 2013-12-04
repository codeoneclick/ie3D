//
//  CGameTransitionTemplateSerializer.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 6/7/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CGameTransitionTemplateSerializer.h"
#include "CCommonOS.h"
#include "ITemplate.h"

CGameTransitionTemplateSerializer::CGameTransitionTemplateSerializer(void)
{
    
}

CGameTransitionTemplateSerializer::~CGameTransitionTemplateSerializer(void)
{
    
}

std::shared_ptr<ITemplate> CGameTransitionTemplateSerializer::Serialize(const std::string& _filename)
{
    std::string path(Get_BundlePath());
    path.append(_filename);
    pugi::xml_document document;
    pugi::xml_parse_result result = ITemplateSerializer::_LoadDocument(document, path);
    assert(result.status == pugi::status_ok);
    pugi::xml_node node = document.child("transition");
    
    std::shared_ptr<SGameTransitionTemplate> gameTransitionTemplate = std::make_shared<SGameTransitionTemplate>();
    gameTransitionTemplate->m_guid = node.attribute("guid").as_string();
    
    pugi::xml_node outputRenderOperationNode = node.child("output.render.operation");
    gameTransitionTemplate->m_outputRenderOperationTemplateFilename = outputRenderOperationNode.attribute("filename").as_string();
    
    pugi::xml_node worldSpaceRenderOperationsNode = node.child("world.space.render.operations");
    for (pugi::xml_node material = worldSpaceRenderOperationsNode.child("world.space.render.operation"); material; material = material.next_sibling("world.space.render.operation"))
    {
        gameTransitionTemplate->m_worldSpaceRenderOperationsTemplatesFilenames.push_back(material.attribute("filename").as_string());
    }
    
    pugi::xml_node screenSpaceRenderOperationsNode = node.child("screen.space.render.operations");
    for (pugi::xml_node material = screenSpaceRenderOperationsNode.child("screen.space.render.operation"); material; material = material.next_sibling("screen.space.render.operation"))
    {
        gameTransitionTemplate->m_screenSpaceRenderOperationsTemplatesFilenames.push_back(material.attribute("filename").as_string());
    }
    
    return gameTransitionTemplate;
}

std::shared_ptr<ITemplate> CGameTransitionTemplateSerializer::Serialize(const std::string& _host, ui32 _port, const std::string& _filename)
{
    return nullptr;
}