//
//  CConfigurationGameTransitionSerializer.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 6/7/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CConfigurationGameTransitionSerializer.h"
#include "CConfigurationGameObjects.h"

CConfigurationGameTransitionSerializer::CConfigurationGameTransitionSerializer(void)
{
    
}

CConfigurationGameTransitionSerializer::~CConfigurationGameTransitionSerializer(void)
{
    
}

ISharedConfiguration CConfigurationGameTransitionSerializer::serialize(const std::string& filename)
{
    pugi::xml_document document;
    pugi::xml_parse_result result = IConfigurationSerializer::openXMLDocument(document, filename);
    assert(result.status == pugi::status_ok);
    
    std::shared_ptr<CConfigurationGameTransition> gameTransitionConfiguration = std::make_shared<CConfigurationGameTransition>();
    pugi::xml_node node = document.child(gameTransitionConfiguration->kGameTransitionMainNode.c_str());
    
    std::string guid = node.attribute(gameTransitionConfiguration->kGameTransitionGuidAttribute.c_str()).as_string();
    gameTransitionConfiguration->setAttribute(getConfigurationAttributeKey(gameTransitionConfiguration->kGameTransitionMainNode,
                                                                      gameTransitionConfiguration->kGameTransitionGuidAttribute),
                                         std::make_shared<CConfigurationAttribute>(guid));
    
    pugi::xpath_node xpathnode = document.select_single_node("/transition/output_render_operation");
    std::string techniqueName = xpathnode.node().attribute("filename").as_string();
    
    pugi::xml_node outputRenderOperationNode = node.child(gameTransitionConfiguration->kGameTransitionORenderOperationConfigurationNode.c_str());
    std::string outputRenderOperationFilename = outputRenderOperationNode.attribute(gameTransitionConfiguration->kGameTransitionORenderOperationConfigurationFilenameAttribute.c_str()).as_string();
    
    gameTransitionConfiguration->setAttribute(getConfigurationAttributeKey(gameTransitionConfiguration->kGameTransitionMainNode,
                                                                      gameTransitionConfiguration->kGameTransitionORenderOperationConfigurationNode,
                                                                      gameTransitionConfiguration->kGameTransitionORenderOperationConfigurationFilenameAttribute),
                                         std::make_shared<CConfigurationAttribute>(outputRenderOperationFilename));
    
    pugi::xml_node worldSpaceRenderOperationsNode = node.child(gameTransitionConfiguration->kGameTransitionWSRenderOperationsConfigurationsNode.c_str());
    for (pugi::xml_node material = worldSpaceRenderOperationsNode.child(gameTransitionConfiguration->kGameTransitionWSRenderOperationConfigurationNode.c_str());
         material;
         material = material.next_sibling(gameTransitionConfiguration->kGameTransitionWSRenderOperationConfigurationNode.c_str()))
    {
        std::string worldSpaceRenderOperationFilename = material.attribute(gameTransitionConfiguration->kGameTransitionWSRenderOperationConfigurationFilenameAttribute.c_str()).as_string();
        gameTransitionConfiguration->setAttribute(getConfigurationAttributeKey(gameTransitionConfiguration->kGameTransitionMainNode,
                                                                          gameTransitionConfiguration->kGameTransitionWSRenderOperationsConfigurationsNode,
                                                                          gameTransitionConfiguration->kGameTransitionWSRenderOperationConfigurationNode,
                                                                          gameTransitionConfiguration->kGameTransitionWSRenderOperationConfigurationFilenameAttribute),
                                             std::make_shared<CConfigurationAttribute>(worldSpaceRenderOperationFilename));
        
    }
    
    pugi::xml_node screenSpaceRenderOperationsNode = node.child(gameTransitionConfiguration->kGameTransitionSSRenderOperationsConfigurationsNode.c_str());
    for (pugi::xml_node material = screenSpaceRenderOperationsNode.child(gameTransitionConfiguration->kGameTransitionSSRenderOperationConfigurationNode.c_str());
         material;
         material = material.next_sibling(gameTransitionConfiguration->kGameTransitionSSRenderOperationConfigurationNode.c_str()))
    {
        std::string screenSpaceRenderOperationFilename = material.attribute(gameTransitionConfiguration->kGameTransitionSSRenderOperationConfigurationFilenameAttribute.c_str()).as_string();
        
        gameTransitionConfiguration->setAttribute(getConfigurationAttributeKey(gameTransitionConfiguration->kGameTransitionMainNode,
                                                                          gameTransitionConfiguration->kGameTransitionSSRenderOperationsConfigurationsNode,
                                                                          gameTransitionConfiguration->kGameTransitionSSRenderOperationConfigurationNode,
                                                                          gameTransitionConfiguration->kGameTransitionSSRenderOperationConfigurationFilenameAttribute),
                                             std::make_shared<CConfigurationAttribute>(screenSpaceRenderOperationFilename));
    }
    
    return gameTransitionConfiguration;
}

void CConfigurationGameTransitionSerializer::deserialize(const std::string& filename,
                                                         ISharedConfigurationRef configuration)
{
    
}