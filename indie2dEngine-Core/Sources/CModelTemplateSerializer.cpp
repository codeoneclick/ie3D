//
//  CModelTemplateSerializer.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/28/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CModelTemplateSerializer.h"
#include "ITemplate.h"

CModelTemplateSerializer::CModelTemplateSerializer(void)
{
    
}

CModelTemplateSerializer::~CModelTemplateSerializer(void)
{
    
}

std::shared_ptr<ITemplate> CModelTemplateSerializer::Serialize(const std::string& _filename)
{
    pugi::xml_document document;
    pugi::xml_parse_result result = ITemplateSerializer::_LoadDocument(document, _filename);
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

void CModelTemplateSerializer::Deserialize(const std::string& _filename, std::shared_ptr<ITemplate> _template)
{
    std::shared_ptr<SModelTemplate> modelTemplate = std::static_pointer_cast<SModelTemplate>(_template);
    pugi::xml_document document;
    document.load("<model name=\"\">\
                   <mesh filename=\"\" is_batching=\"\"/>\
                   <skeleton filename=\"\"/>\
                   <animations>\
                   <animation name=\"\" filename=\"\"/>\
                   </animations>\
                   <materials>\
                   <material filename=\"\"/>\
                   </materials>\
                   </model>");
    pugi::xml_node node = document.child("model");
    node.attribute("name").set_value(modelTemplate->m_meshFilename.c_str());
    node.child("mesh").attribute("filename").set_value(modelTemplate->m_meshFilename.c_str());
    node.child("mesh").attribute("is_batching").set_value(0);
    node.child("skeleton").attribute("filename").set_value(modelTemplate->m_skeletonFilename.c_str());
    for(auto sequence : modelTemplate->m_sequencesFilenames)
    {
        node.child("animations").append_child("animation");
        node.child("animations").child("animation").append_attribute("filename");
        node.child("animations").child("animation").attribute("filename").set_value(sequence.c_str());
    }
    for(auto material : modelTemplate->m_materialsFilenames)
    {
        node.child("materials").append_child("material");
        node.child("materials").child("material").append_attribute("filename");
        node.child("materials").child("material").attribute("filename").set_value(material.c_str());
    }
}
