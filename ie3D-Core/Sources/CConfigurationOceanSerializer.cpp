//
//  CConfigurationOceanSerializer.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 7/24/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CConfigurationOceanSerializer.h"
#include "CConfigurationMaterialSerializer.h"
#include "CConfigurationGameObjects.h"

CConfigurationOceanSerializer::CConfigurationOceanSerializer(void)
{
    
}

CConfigurationOceanSerializer::~CConfigurationOceanSerializer(void)
{
    
}

ISharedConfiguration CConfigurationOceanSerializer::serialize(const std::string& filename)
{
    pugi::xml_document document;
    pugi::xml_parse_result result = IConfigurationSerializer::openXMLDocument(document, filename);
    assert(result.status == pugi::status_ok);
    
    std::shared_ptr<CConfigurationOcean> oceanConfiguration = std::make_shared<CConfigurationOcean>();
    pugi::xml_node mainNode = document.child(oceanConfiguration->kOceanMainNode.c_str());
    
    f32 sizeX = mainNode.attribute(oceanConfiguration->kOceanSizeXAttribute.c_str()).as_float();
    oceanConfiguration->setAttribute(getConfigurationAttributeKey(oceanConfiguration->kOceanMainNode,
                                                                  oceanConfiguration->kOceanSizeXAttribute),
                                     std::make_shared<CConfigurationAttribute>(sizeX));
    
    f32 sizeY = mainNode.attribute(oceanConfiguration->kOceanSizeYAttribute.c_str()).as_float();
    oceanConfiguration->setAttribute(getConfigurationAttributeKey(oceanConfiguration->kOceanMainNode,
                                                                  oceanConfiguration->kOceanSizeYAttribute),
                                     std::make_shared<CConfigurationAttribute>(sizeY));
    
    f32 altitude = mainNode.attribute(oceanConfiguration->kOceanAltitudeAttribute.c_str()).as_float();
    oceanConfiguration->setAttribute(getConfigurationAttributeKey(oceanConfiguration->kOceanMainNode,
                                                                  oceanConfiguration->kOceanAltitudeAttribute),
                                     std::make_shared<CConfigurationAttribute>(altitude));
    
    f32 waveGenerationInterval = mainNode.attribute(oceanConfiguration->kOceanWaveGenerationIntervalAttribute.c_str()).as_float();
    oceanConfiguration->setAttribute(getConfigurationAttributeKey(oceanConfiguration->kOceanMainNode,
                                                                  oceanConfiguration->kOceanWaveGenerationIntervalAttribute),
                                     std::make_shared<CConfigurationAttribute>(waveGenerationInterval));
    
    CConfigurationMaterialSerializer::serializeGameObjectMaterialsNode(oceanConfiguration,
                                                                       mainNode);
    return oceanConfiguration;
}

void CConfigurationOceanSerializer::deserialize(const std::string& filename,
                                                ISharedConfigurationRef configuration)
{
    
}