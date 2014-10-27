//
//  CConfigurationSkyBoxSerializer.cpp
//  ie3D-Core
//
//  Created by Sergey Sergeev on 5/6/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#include "CConfigurationSkyBoxSerializer.h"
#include "CConfigurationMaterialSerializer.h"
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
    pugi::xml_node mainNode = document.child(skyBoxConfiguration->kSkyBoxMainNode.c_str());
    
    std::string xPositiveTextureFilename = mainNode.attribute(skyBoxConfiguration->kSkyBoxXPositiveTextureFilenameAttribute.c_str()).as_string();
    skyBoxConfiguration->setAttribute(getConfigurationAttributeKey(skyBoxConfiguration->kSkyBoxMainNode,
                                                                   skyBoxConfiguration->kSkyBoxXPositiveTextureFilenameAttribute),
                                      std::make_shared<CConfigurationAttribute>(xPositiveTextureFilename));
    
    std::string xNegativeTextureFilename = mainNode.attribute(skyBoxConfiguration->kSkyBoxXNegativeTextureFilenameAttribute.c_str()).as_string();
    skyBoxConfiguration->setAttribute(getConfigurationAttributeKey(skyBoxConfiguration->kSkyBoxMainNode,
                                                                   skyBoxConfiguration->kSkyBoxXNegativeTextureFilenameAttribute),
                                      std::make_shared<CConfigurationAttribute>(xNegativeTextureFilename));
    
    std::string yPositiveTextureFilename = mainNode.attribute(skyBoxConfiguration->kSkyBoxYPositiveTextureFilenameAttribute.c_str()).as_string();
    skyBoxConfiguration->setAttribute(getConfigurationAttributeKey(skyBoxConfiguration->kSkyBoxMainNode,
                                                                   skyBoxConfiguration->kSkyBoxYPositiveTextureFilenameAttribute),
                                      std::make_shared<CConfigurationAttribute>(yPositiveTextureFilename));
    
    std::string yNegativeTextureFilename = mainNode.attribute(skyBoxConfiguration->kSkyBoxYNegativeTextureFilenameAttribute.c_str()).as_string();
    skyBoxConfiguration->setAttribute(getConfigurationAttributeKey(skyBoxConfiguration->kSkyBoxMainNode,
                                                                   skyBoxConfiguration->kSkyBoxYNegativeTextureFilenameAttribute),
                                      std::make_shared<CConfigurationAttribute>(yNegativeTextureFilename));
    
    std::string zPositiveTextureFilename = mainNode.attribute(skyBoxConfiguration->kSkyBoxZPositiveTextureFilenameAttribute.c_str()).as_string();
    skyBoxConfiguration->setAttribute(getConfigurationAttributeKey(skyBoxConfiguration->kSkyBoxMainNode,
                                                                   skyBoxConfiguration->kSkyBoxZPositiveTextureFilenameAttribute),
                                      std::make_shared<CConfigurationAttribute>(zPositiveTextureFilename));
    
    std::string zNegativeTextureFilename = mainNode.attribute(skyBoxConfiguration->kSkyBoxZNegativeTextureFilenameAttribute.c_str()).as_string();
    skyBoxConfiguration->setAttribute(getConfigurationAttributeKey(skyBoxConfiguration->kSkyBoxMainNode,
                                                                   skyBoxConfiguration->kSkyBoxZNegativeTextureFilenameAttribute),
                                      std::make_shared<CConfigurationAttribute>(zNegativeTextureFilename));
    
    CConfigurationMaterialSerializer::serializeGameObjectMaterialsNode(skyBoxConfiguration,
                                                                       mainNode);
    return skyBoxConfiguration;
}

void CConfigurationSkyBoxSerializer::deserialize(const std::string& filename,
                                                 ISharedConfigurationRef configuration)
{
    
}