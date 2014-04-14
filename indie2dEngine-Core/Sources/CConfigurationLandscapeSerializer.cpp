//
//  CConfigurationLandscapeSerializer.cpp
//  indieEngineCore
//
//  Created by Sergey Sergeev on 11/25/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CConfigurationLandscapeSerializer.h"
#include "CConfigurationGameObjects.h"

CConfigurationLandscapeSerializer::CConfigurationLandscapeSerializer(void)
{
    
}

CConfigurationLandscapeSerializer::~CConfigurationLandscapeSerializer(void)
{
    
}

ISharedConfiguration CConfigurationLandscapeSerializer::serialize(const std::string& filename)
{
    pugi::xml_document document;
    pugi::xml_parse_result result = IConfigurationSerializer::openXMLDocument(document, filename);
    assert(result.status == pugi::status_ok);
    
    std::shared_ptr<CConfigurationLandscape> landscapeConfiguration = std::make_shared<CConfigurationLandscape>();
    pugi::xml_node node = document.child(landscapeConfiguration->kLandscapeMainNode.c_str());
    
    f32 sizeX = node.attribute(landscapeConfiguration->kLandscapeSizeXAttribute.c_str()).as_float();
    landscapeConfiguration->setAttribute(getConfigurationAttributeKey(landscapeConfiguration->kLandscapeMainNode,
                                                              landscapeConfiguration->kLandscapeSizeXAttribute),
                                     std::make_shared<CConfigurationAttribute>(sizeX));
    
    f32 sizeY = node.attribute(landscapeConfiguration->kLandscapeSizeYAttribute.c_str()).as_float();
    landscapeConfiguration->setAttribute(getConfigurationAttributeKey(landscapeConfiguration->kLandscapeMainNode,
                                                              landscapeConfiguration->kLandscapeSizeYAttribute),
                                     std::make_shared<CConfigurationAttribute>(sizeY));

    std::string heightmapDataFilename = node.attribute(landscapeConfiguration->kLandscapeHeightmapDataFilenameAttribute.c_str()).as_string();
    landscapeConfiguration->setAttribute(getConfigurationAttributeKey(landscapeConfiguration->kLandscapeMainNode,
                                                              landscapeConfiguration->kLandscapeHeightmapDataFilenameAttribute),
                                     std::make_shared<CConfigurationAttribute>(heightmapDataFilename));
    
    std::string splattingDataFilename = node.attribute(landscapeConfiguration->kLandscapeSplattingDataFilenameAttribute.c_str()).as_string();
    landscapeConfiguration->setAttribute(getConfigurationAttributeKey(landscapeConfiguration->kLandscapeMainNode,
                                                              landscapeConfiguration->kLandscapeSplattingDataFilenameAttribute),
                                     std::make_shared<CConfigurationAttribute>(splattingDataFilename));
    
    std::string splattingDiffuseMaterialFilename = node.attribute(landscapeConfiguration->kLandscapeSplattingDiffuseMaterialFilenameAttribute.c_str()).as_string();
    landscapeConfiguration->setAttribute(getConfigurationAttributeKey(landscapeConfiguration->kLandscapeMainNode,
                                                              landscapeConfiguration->kLandscapeSplattingDiffuseMaterialFilenameAttribute),
                                     std::make_shared<CConfigurationAttribute>(splattingDiffuseMaterialFilename));
    
    std::string splattingNormalMaterialFilename = node.attribute(landscapeConfiguration->kLandscapeSplattingNormalMaterialFilenameAttribute.c_str()).as_string();
    landscapeConfiguration->setAttribute(getConfigurationAttributeKey(landscapeConfiguration->kLandscapeMainNode,
                                                              landscapeConfiguration->kLandscapeSplattingNormalMaterialFilenameAttribute),
                                     std::make_shared<CConfigurationAttribute>(splattingNormalMaterialFilename));
    
    bool isEdgesEnabled = node.attribute(landscapeConfiguration->kLandscapeIsEdgesEnabledAttribute.c_str()).as_bool();
    landscapeConfiguration->setAttribute(getConfigurationAttributeKey(landscapeConfiguration->kLandscapeMainNode,
                                                              landscapeConfiguration->kLandscapeIsEdgesEnabledAttribute),
                                     std::make_shared<CConfigurationAttribute>(isEdgesEnabled));
    
    pugi::xml_node edgesNode = node.child(landscapeConfiguration->kLandscapeEdgesNode.c_str());
    
    f32 edgesSizeX = edgesNode.attribute(landscapeConfiguration->kLandscapeEdgesSizeXAttribute.c_str()).as_float();
    landscapeConfiguration->setAttribute(getConfigurationAttributeKey(landscapeConfiguration->kLandscapeMainNode,
                                                              landscapeConfiguration->kLandscapeEdgesNode,
                                                              landscapeConfiguration->kLandscapeEdgesSizeXAttribute),
                                     std::make_shared<CConfigurationAttribute>(edgesSizeX));
    
    f32 edgesSizeY = edgesNode.attribute(landscapeConfiguration->kLandscapeEdgesSizeYAttribute.c_str()).as_float();
    landscapeConfiguration->setAttribute(getConfigurationAttributeKey(landscapeConfiguration->kLandscapeMainNode,
                                                              landscapeConfiguration->kLandscapeEdgesNode,
                                                              landscapeConfiguration->kLandscapeEdgesSizeYAttribute),
                                     std::make_shared<CConfigurationAttribute>(edgesSizeY));
    
    pugi::xml_node edgesMaterialsNode = edgesNode.child(landscapeConfiguration->kLandscapeEdgesMaterialsConfigurationsNode.c_str());
    for (pugi::xml_node material = edgesMaterialsNode.child(landscapeConfiguration->kLandscapeEdgeMaterialConfigurationNode.c_str());
         material;
         material = material.next_sibling(landscapeConfiguration->kLandscapeEdgeMaterialConfigurationNode.c_str()))
    {
        std::string filename = material.attribute(landscapeConfiguration->kLandscapeEdgeMaterialConfigurationFilenameAttribute.c_str()).as_string();
        landscapeConfiguration->setAttribute(getConfigurationAttributeKey(landscapeConfiguration->kLandscapeMainNode,
                                                                  landscapeConfiguration->kLandscapeEdgesNode,
                                                                  landscapeConfiguration->kLandscapeEdgesMaterialsConfigurationsNode,
                                                                  landscapeConfiguration->kLandscapeEdgeMaterialConfigurationNode,
                                                                  landscapeConfiguration->kLandscapeEdgeMaterialConfigurationFilenameAttribute),
                                         std::make_shared<CConfigurationAttribute>(filename));
    }

    
    pugi::xml_node materialsNode = node.child(landscapeConfiguration->kGameObjectMaterialsConfigurationsNode.c_str());
    for (pugi::xml_node material = materialsNode.child(landscapeConfiguration->kGameObjectMaterialConfigurationNode.c_str());
         material;
         material = material.next_sibling(landscapeConfiguration->kGameObjectMaterialConfigurationNode.c_str()))
    {
        std::string filename = material.attribute(landscapeConfiguration->kGameObjectMaterialFilenameAttribute.c_str()).as_string();
        landscapeConfiguration->setAttribute(getConfigurationAttributeKey(landscapeConfiguration->kGameObjectMaterialsConfigurationsNode,
                                                                        landscapeConfiguration->kGameObjectMaterialConfigurationNode,
                                                                        landscapeConfiguration->kGameObjectMaterialFilenameAttribute),
                                               std::make_shared<CConfigurationAttribute>(filename));
    }
    
    return landscapeConfiguration;
}

void CConfigurationLandscapeSerializer::deserialize(const std::string& filename,
                                                    ISharedConfigurationRef configuration)
{
    
}
