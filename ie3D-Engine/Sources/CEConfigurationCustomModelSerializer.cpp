//
//  CEConfigurationCustomModelSerializer.cpp
//  ie3D-Engine
//
//  Created by sergey.sergeev on 10/28/14.
//  Copyright (c) 2014 sergey.sergeev. All rights reserved.
//

#include "CEConfigurationCustomModelSerializer.h"
#include "CEConfigurationGameObjects.h"
#include "CConfigurationMaterialSerializer.h"

CEConfigurationCustomModelSerializer::CEConfigurationCustomModelSerializer(void)
{
    
}

CEConfigurationCustomModelSerializer::~CEConfigurationCustomModelSerializer(void)
{
    
}

ISharedConfiguration CEConfigurationCustomModelSerializer::serialize(const std::string& filename)
{
    pugi::xml_document document;
    pugi::xml_parse_result result = IConfigurationSerializer::openXMLDocument(document, filename);
    assert(result.status == pugi::status_ok);
    
    std::shared_ptr<CEConfigurationCustomModel> customModelConfiguration = std::make_shared<CEConfigurationCustomModel>();
    pugi::xml_node mainNode = document.child(customModelConfiguration->kCustomModelMainNode.c_str());
    
    CConfigurationMaterialSerializer::serializeGameObjectMaterialsNode(customModelConfiguration,
                                                                       mainNode);
    return customModelConfiguration;
}

void CEConfigurationCustomModelSerializer::deserialize(const std::string&,
                                                       ISharedConfigurationRef)
{
    
}