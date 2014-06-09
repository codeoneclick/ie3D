//
//  CMEConfigurationSelectionAreaSerializer.cpp
//  ie3D-Demo
//
//  Created by Sergey Sergeev on 6/7/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#include "CMEConfigurationSelectionAreaSerializer.h"
#include "CMEConfigurationGameObjects.h"

CMEConfigurationSelectionAreaSerializer::CMEConfigurationSelectionAreaSerializer(void)
{
    
}

CMEConfigurationSelectionAreaSerializer::~CMEConfigurationSelectionAreaSerializer(void)
{
    
}

ISharedConfiguration CMEConfigurationSelectionAreaSerializer::serialize(const std::string& filename)
{
    pugi::xml_document document;
    pugi::xml_parse_result result = IConfigurationSerializer::openXMLDocument(document, filename);
    assert(result.status == pugi::status_ok);
    
    std::shared_ptr<CMEConfigurationSelectionArea> selectionAreaConfiguration = std::make_shared<CMEConfigurationSelectionArea>();
    pugi::xml_node node = document.child(selectionAreaConfiguration->kSelectionAreaMainNode.c_str());
    
    f32 sizeX = node.attribute(selectionAreaConfiguration->kSelectionAreaSizeXAttribute.c_str()).as_float();
    selectionAreaConfiguration->setAttribute(getConfigurationAttributeKey(selectionAreaConfiguration->kSelectionAreaMainNode,
                                                                          selectionAreaConfiguration->kSelectionAreaSizeXAttribute),
                                             std::make_shared<CConfigurationAttribute>(sizeX));
    
    f32 sizeY = node.attribute(selectionAreaConfiguration->kSelectionAreaSizeYAttribute.c_str()).as_float();
    selectionAreaConfiguration->setAttribute(getConfigurationAttributeKey(selectionAreaConfiguration->kSelectionAreaMainNode,
                                                                          selectionAreaConfiguration->kSelectionAreaSizeYAttribute),
                                             std::make_shared<CConfigurationAttribute>(sizeY));
    
    pugi::xml_node materialsNode = node.child(selectionAreaConfiguration->kGameObjectMaterialsConfigurationsNode.c_str());
    for (pugi::xml_node material = materialsNode.child(selectionAreaConfiguration->kGameObjectMaterialConfigurationNode.c_str());
         material;
         material = material.next_sibling(selectionAreaConfiguration->kGameObjectMaterialConfigurationNode.c_str()))
    {
        std::string filename = material.attribute(selectionAreaConfiguration->kGameObjectMaterialFilenameAttribute.c_str()).as_string();
        selectionAreaConfiguration->setAttribute(getConfigurationAttributeKey(selectionAreaConfiguration->kGameObjectMaterialsConfigurationsNode,
                                                                              selectionAreaConfiguration->kGameObjectMaterialConfigurationNode,
                                                                              selectionAreaConfiguration->kGameObjectMaterialFilenameAttribute),
                                                 std::make_shared<CConfigurationAttribute>(filename));
    }
    return selectionAreaConfiguration;
}

void CMEConfigurationSelectionAreaSerializer::deserialize(const std::string&,
                                                          ISharedConfigurationRef)
{
    
}