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
    
    std::shared_ptr<CConfigurationModel> modelConfiguration = std::make_shared<CConfigurationModel>();
    pugi::xml_node mainNode = document.child(modelConfiguration->kModelMainNode.c_str());
    
    CConfigurationMaterialSerializer::serializeGameObjectMaterialsNode(modelConfiguration,
                                                                       mainNode);
    return modelConfiguration;
}

void CEConfigurationCustomModelSerializer::deserialize(const std::string&,
                                                       ISharedConfigurationRef)
{
    
}