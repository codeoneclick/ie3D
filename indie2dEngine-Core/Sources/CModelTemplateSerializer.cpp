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
    pugi::xml_parse_result result = document.load_file(path.c_str());
    assert(result.status == pugi::status_ok);
    pugi::xml_node node = document.child("model");
    
    std::shared_ptr<SModelTemplate> modelTemplate = std::make_shared<SModelTemplate>();
    modelTemplate->m_meshFilename = node.child("mesh").attribute("value").as_string();
    
    pugi::xml_node materials_node = node.child("materials");
    for (pugi::xml_node material = materials_node.child("material"); material; material = material.next_sibling("material"))
    {
        modelTemplate->m_materialsFilenames.push_back(material.attribute("filename").as_string());
    }
    return modelTemplate;
}