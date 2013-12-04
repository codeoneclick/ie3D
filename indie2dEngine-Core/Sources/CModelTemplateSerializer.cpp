//
//  CModelTemplateSerializer.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/28/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CModelTemplateSerializer.h"
#include "CCommonOS.h"
#include "ITemplate.h"

CModelTemplateSerializer::CModelTemplateSerializer(void)
{
    
}

CModelTemplateSerializer::~CModelTemplateSerializer(void)
{
    
}

std::shared_ptr<ITemplate> CModelTemplateSerializer::Serialize(const std::string& _filename)
{
    std::string path(Get_BundlePath());
    path.append(_filename);
    pugi::xml_document document;
    pugi::xml_parse_result result = ITemplateSerializer::_LoadDocument(document, path);
    assert(result.status == pugi::status_ok);
    pugi::xml_node node = document.child("model");
    
    std::shared_ptr<SModelTemplate> modelTemplate = std::make_shared<SModelTemplate>();
    modelTemplate->m_meshFilename = node.child("mesh").attribute("filename").as_string();
    modelTemplate->m_isBatching = node.child("mesh").attribute("is_batching").as_bool();
    modelTemplate->m_skeletonFilename = node.child("skeleton").attribute("filename").as_string();
    
    pugi::xml_node materials_node = node.child("materials");
    for (pugi::xml_node material = materials_node.child("material"); material; material = material.next_sibling("material"))
    {
        modelTemplate->m_materialsFilenames.push_back(material.attribute("filename").as_string());
    }
    
    pugi::xml_node animations_node = node.child("animations");
    for (pugi::xml_node animation = animations_node.child("animation"); animation; animation = animation.next_sibling("animation"))
    {
        modelTemplate->m_sequencesFilenames.push_back(animation.attribute("filename").as_string());
    }
    
    return modelTemplate;
}

std::shared_ptr<ITemplate> CModelTemplateSerializer::Serialize(const std::string& _host, ui32 _port, const std::string& _filename)
{
    return nullptr;
}