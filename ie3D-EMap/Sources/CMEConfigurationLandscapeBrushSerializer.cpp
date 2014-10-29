//
//  CMEConfigurationLandscapeBrushSerializer.cpp
//  ie3D-Demo
//
//  Created by Sergey Sergeev on 6/7/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#include "CMEConfigurationLandscapeBrushSerializer.h"
#include "CMEConfigurationGameObjects.h"
#include "CConfigurationMaterialSerializer.h"

CMEConfigurationLandscapeBrushSerializer::CMEConfigurationLandscapeBrushSerializer(void)
{
    
}

CMEConfigurationLandscapeBrushSerializer::~CMEConfigurationLandscapeBrushSerializer(void)
{
    
}

ISharedConfiguration CMEConfigurationLandscapeBrushSerializer::serialize(const std::string& filename)
{
    pugi::xml_document document;
    pugi::xml_parse_result result = IConfigurationSerializer::openXMLDocument(document, filename);
    assert(result.status == pugi::status_ok);
    
    std::shared_ptr<CMEConfigurationLandscapeBrush> landscapeBrushConfiguration = std::make_shared<CMEConfigurationLandscapeBrush>();
    pugi::xml_node mainNode = document.child(landscapeBrushConfiguration->kLandscapeBrushMainNode.c_str());
    
    f32 sizeX = mainNode.attribute(landscapeBrushConfiguration->kLandscapeBrushSizeXAttribute.c_str()).as_float();
    landscapeBrushConfiguration->setAttribute(getConfigurationAttributeKey(landscapeBrushConfiguration->kLandscapeBrushMainNode,
                                                                           landscapeBrushConfiguration->kLandscapeBrushSizeXAttribute),
                                              std::make_shared<CConfigurationAttribute>(sizeX));
    
    f32 sizeY = mainNode.attribute(landscapeBrushConfiguration->kLandscapeBrushSizeYAttribute.c_str()).as_float();
    landscapeBrushConfiguration->setAttribute(getConfigurationAttributeKey(landscapeBrushConfiguration->kLandscapeBrushMainNode,
                                                                           landscapeBrushConfiguration->kLandscapeBrushSizeYAttribute),
                                              std::make_shared<CConfigurationAttribute>(sizeY));
    
    CConfigurationMaterialSerializer::serializeGameObjectMaterialsNode(landscapeBrushConfiguration, mainNode);
    return landscapeBrushConfiguration;
}

void CMEConfigurationLandscapeBrushSerializer::deserialize(const std::string&,
                                                          ISharedConfigurationRef)
{
    
}