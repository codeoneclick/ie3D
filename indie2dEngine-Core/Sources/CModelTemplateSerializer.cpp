//
//  CModelTemplateSerializer.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/28/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CModelTemplateSerializer.h"
#include "IGameObjectTemplateSerializer.h"
#include "CCommonOS.h"

CModelTemplateSerializer::CModelTemplateSerializer(void)
{
    
}

CModelTemplateSerializer::~CModelTemplateSerializer(void)
{
    
}

std::shared_ptr<SModelTemplate> CModelTemplateSerializer::Deserialize(const std::string& _filename)
{
    std::string path(Get_BundlePath());
    path.append(_filename);
    pugi::xml_document document;
    pugi::xml_parse_result result = document.load_file(path.c_str());
    assert(result.status == pugi::status_ok);
    pugi::xml_node settings_node = document.child("settings");
    
    std::shared_ptr<SModelTemplate> modelTemplate = std::make_shared<SModelTemplate>();
    IGameObjectTemplateSerializer gameObjectTemplateSerializer;
    modelTemplate->m_materialsTemplates = gameObjectTemplateSerializer.Deserialize(settings_node);
    modelTemplate->m_meshFilename = settings_node.child("mesh").attribute("value").as_string();
    return modelTemplate;
}