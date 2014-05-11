//
//  CConfigurationSkyBoxSerializer.cpp
//  ie3D-Core
//
//  Created by Sergey Sergeev on 5/6/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#include "CConfigurationSkyBoxSerializer.h"
#include "CConfigurationGameObjects.h"

CConfigurationSkyBoxSerializer::CConfigurationSkyBoxSerializer(void)
{
    
}

CConfigurationSkyBoxSerializer::~CConfigurationSkyBoxSerializer(void)
{
    
}

ISharedConfiguration CConfigurationSkyBoxSerializer::serialize(const std::string& filename)
{
    pugi::xml_document document;
    pugi::xml_parse_result result = IConfigurationSerializer::openXMLDocument(document, filename);
    assert(result.status == pugi::status_ok);
    
    std::shared_ptr<CConfigurationSkyBox> skyBoxConfiguration = std::make_shared<CConfigurationSkyBox>();
    pugi::xml_node node = document.child(skyBoxConfiguration->kSkyBoxMainNode.c_str());
    
    std::string xPositiveTextureFilename = node.attribute(skyBoxConfiguration->kSkyBoxXPositiveTextureFilenameAttribute.c_str()).as_string();
    skyBoxConfiguration->setAttribute(getConfigurationAttributeKey(skyBoxConfiguration->kSkyBoxMainNode,
                                                                   skyBoxConfiguration->kSkyBoxXPositiveTextureFilenameAttribute),
                                      std::make_shared<CConfigurationAttribute>(xPositiveTextureFilename));
    
    std::string xNegativeTextureFilename = node.attribute(skyBoxConfiguration->kSkyBoxXNegativeTextureFilenameAttribute.c_str()).as_string();
    skyBoxConfiguration->setAttribute(getConfigurationAttributeKey(skyBoxConfiguration->kSkyBoxMainNode,
                                                                   skyBoxConfiguration->kSkyBoxXNegativeTextureFilenameAttribute),
                                      std::make_shared<CConfigurationAttribute>(xNegativeTextureFilename));
    
    std::string yPositiveTextureFilename = node.attribute(skyBoxConfiguration->kSkyBoxYPositiveTextureFilenameAttribute.c_str()).as_string();
    skyBoxConfiguration->setAttribute(getConfigurationAttributeKey(skyBoxConfiguration->kSkyBoxMainNode,
                                                                   skyBoxConfiguration->kSkyBoxYPositiveTextureFilenameAttribute),
                                      std::make_shared<CConfigurationAttribute>(yPositiveTextureFilename));
    
    std::string yNegativeTextureFilename = node.attribute(skyBoxConfiguration->kSkyBoxYNegativeTextureFilenameAttribute.c_str()).as_string();
    skyBoxConfiguration->setAttribute(getConfigurationAttributeKey(skyBoxConfiguration->kSkyBoxMainNode,
                                                                   skyBoxConfiguration->kSkyBoxYNegativeTextureFilenameAttribute),
                                      std::make_shared<CConfigurationAttribute>(yNegativeTextureFilename));
    
    std::string zPositiveTextureFilename = node.attribute(skyBoxConfiguration->kSkyBoxZPositiveTextureFilenameAttribute.c_str()).as_string();
    skyBoxConfiguration->setAttribute(getConfigurationAttributeKey(skyBoxConfiguration->kSkyBoxMainNode,
                                                                   skyBoxConfiguration->kSkyBoxZPositiveTextureFilenameAttribute),
                                      std::make_shared<CConfigurationAttribute>(zPositiveTextureFilename));
    
    std::string zNegativeTextureFilename = node.attribute(skyBoxConfiguration->kSkyBoxZNegativeTextureFilenameAttribute.c_str()).as_string();
    skyBoxConfiguration->setAttribute(getConfigurationAttributeKey(skyBoxConfiguration->kSkyBoxMainNode,
                                                                   skyBoxConfiguration->kSkyBoxZNegativeTextureFilenameAttribute),
                                      std::make_shared<CConfigurationAttribute>(zNegativeTextureFilename));
    
    pugi::xml_node materialsNode = node.child(skyBoxConfiguration->kGameObjectMaterialsConfigurationsNode.c_str());
    for (pugi::xml_node material = materialsNode.child(skyBoxConfiguration->kGameObjectMaterialConfigurationNode.c_str());
         material;
         material = material.next_sibling(skyBoxConfiguration->kGameObjectMaterialConfigurationNode.c_str()))
    {
        std::string filename = material.attribute(skyBoxConfiguration->kGameObjectMaterialFilenameAttribute.c_str()).as_string();
        skyBoxConfiguration->setAttribute(getConfigurationAttributeKey(skyBoxConfiguration->kGameObjectMaterialsConfigurationsNode,
                                                                       skyBoxConfiguration->kGameObjectMaterialConfigurationNode,
                                                                       skyBoxConfiguration->kGameObjectMaterialFilenameAttribute),
                                          std::make_shared<CConfigurationAttribute>(filename));
    }
    return skyBoxConfiguration;
}

void CConfigurationSkyBoxSerializer::deserialize(const std::string& filename,
                                                 ISharedConfigurationRef configuration)
{
    
}