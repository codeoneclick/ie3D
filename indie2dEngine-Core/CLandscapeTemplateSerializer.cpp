//
//  CLandscapeTemplateSerializer.cpp
//  indieEngineCore
//
//  Created by Sergey Sergeev on 11/25/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CLandscapeTemplateSerializer.h"
#include "CCommonOS.h"
#include "ITemplate.h"

CLandscapeTemplateSerializer::CLandscapeTemplateSerializer(void)
{
    
}

CLandscapeTemplateSerializer::~CLandscapeTemplateSerializer(void)
{
    
}

std::shared_ptr<ITemplate> CLandscapeTemplateSerializer::Serialize(const std::string& _filename)
{
    std::string path(Get_BundlePath());
    path.append(_filename);
    pugi::xml_document document;
    pugi::xml_parse_result result = document.load_file(path.c_str());
    assert(result.status == pugi::status_ok);
    pugi::xml_node node = document.child("landscape");
    
    std::shared_ptr<SLandscapeTemplate> landscapeTemplate = std::make_shared<SLandscapeTemplate>();
    landscapeTemplate->m_width = node.child("width").attribute("value").as_float();
    landscapeTemplate->m_height = node.child("height").attribute("value").as_float();
    landscapeTemplate->m_heightmapDataFileName = node.child("heightmap_data").attribute("filename").as_string();
    landscapeTemplate->m_splattingDataFileName = node.child("splatting_data").attribute("filename").as_string();
    landscapeTemplate->m_splattingDiffuseMaterialFilename = node.child("splatting_diffuse_material").attribute("filename").as_string();
    landscapeTemplate->m_splattingNormalMaterialFilename = node.child("splatting_normal_material").attribute("filename").as_string();
    
    pugi::xml_node materials_node = node.child("materials");
    for (pugi::xml_node material = materials_node.child("material"); material; material = material.next_sibling("material"))
    {
        landscapeTemplate->m_materialsFilenames.push_back(material.attribute("filename").as_string());
    }
    
    landscapeTemplate->m_edgesBound_x = node.child("edge_bound_x").attribute("value").as_float();
    landscapeTemplate->m_edgesBound_y = node.child("edge_bound_y").attribute("value").as_float();
    
    materials_node = node.child("edges_materials");
    for (pugi::xml_node material = materials_node.child("material"); material; material = material.next_sibling("material"))
    {
        landscapeTemplate->m_edgesMaterialsFilenames.push_back(material.attribute("filename").as_string());
    }
    
    return landscapeTemplate;
}

std::shared_ptr<ITemplate> CLandscapeTemplateSerializer::Serialize(const std::string& _host, ui32 _port, const std::string& _filename)
{
    return nullptr;
}
