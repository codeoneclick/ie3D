//
//  COceanTemplateSerializer.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 7/24/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "COceanTemplateSerializer.h"
#include "CCommonOS.h"
#include "ITemplate.h"

COceanTemplateSerializer::COceanTemplateSerializer(void)
{
    
}

COceanTemplateSerializer::~COceanTemplateSerializer(void)
{

}

std::shared_ptr<ITemplate> COceanTemplateSerializer::Serialize(const std::string& _filename)
{
    std::string path(Get_BundlePath());
    path.append(_filename);
    pugi::xml_document document;
    pugi::xml_parse_result result = document.load_file(path.c_str());
    assert(result.status == pugi::status_ok);
    pugi::xml_node node = document.child("ocean");
    
    std::shared_ptr<SOceanTemplate> oceanTemplate = std::make_shared<SOceanTemplate>();
    oceanTemplate->m_width = node.child("width").attribute("value").as_float();
    oceanTemplate->m_height = node.child("height").attribute("value").as_float();
    oceanTemplate->m_altitude = node.child("altitude").attribute("value").as_float();
    oceanTemplate->m_waveGeneratorInterval = node.child("waveGeneratorInterval").attribute("value").as_float();
    
    pugi::xml_node materials_node = node.child("materials");
    for (pugi::xml_node material = materials_node.child("material"); material; material = material.next_sibling("material"))
    {
        oceanTemplate->m_materialsFilenames.push_back(material.attribute("filename").as_string());
    }
    return oceanTemplate;
}

std::shared_ptr<ITemplate> COceanTemplateSerializer::Serialize(const std::string& _host, ui32 _port, const std::string& _filename)
{
    return nullptr;
}