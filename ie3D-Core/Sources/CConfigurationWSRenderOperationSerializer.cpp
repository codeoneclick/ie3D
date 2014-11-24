//
//  CWorldSpaceOperationTemplateSerializer.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 6/7/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CConfigurationWSRenderOperationSerializer.h"
#include "CConfigurationGameObjects.h"

CConfigurationWSRenderOperationSerializer::CConfigurationWSRenderOperationSerializer(void)
{
    
}

CConfigurationWSRenderOperationSerializer::~CConfigurationWSRenderOperationSerializer(void)
{
    
}

ISharedConfiguration CConfigurationWSRenderOperationSerializer::serialize(const std::string& filename)
{
    pugi::xml_document document;
    pugi::xml_parse_result result = IConfigurationSerializer::openXMLDocument(document, filename);
    assert(result.status == pugi::status_ok);
    
    std::shared_ptr<CConfigurationWSRenderOperation> worldSpaceRenderOperationConfiguration = std::make_shared<CConfigurationWSRenderOperation>();
    pugi::xml_node node = document.child(worldSpaceRenderOperationConfiguration->kWSRenderOperationMainNode.c_str());
    
    std::string guid = node.attribute(worldSpaceRenderOperationConfiguration->kWSRenderOperationGuidAttribute.c_str()).as_string();
    worldSpaceRenderOperationConfiguration->setAttribute(getConfigurationAttributeKey(worldSpaceRenderOperationConfiguration->kWSRenderOperationMainNode,
                                                                                 worldSpaceRenderOperationConfiguration->kWSRenderOperationGuidAttribute),
                                                    std::make_shared<CConfigurationAttribute>(guid));
    
    i8 areDrawBoundingBoxes = static_cast<i8>(node.attribute(worldSpaceRenderOperationConfiguration->kWSRenderOperationAreDrawBoundingBoxes.c_str()).as_bool());
    worldSpaceRenderOperationConfiguration->setAttribute(getConfigurationAttributeKey(worldSpaceRenderOperationConfiguration->kWSRenderOperationMainNode,
                                                                                      worldSpaceRenderOperationConfiguration->kWSRenderOperationAreDrawBoundingBoxes),
                                                         std::make_shared<CConfigurationAttribute>(areDrawBoundingBoxes));
    
    i8 isOcclusionQueryEnabled = static_cast<i8>(node.attribute(worldSpaceRenderOperationConfiguration->kWSRenderOperationIsOcclusionQueryEnabledAttribute.c_str()).as_bool());
    worldSpaceRenderOperationConfiguration->setAttribute(getConfigurationAttributeKey(worldSpaceRenderOperationConfiguration->kWSRenderOperationMainNode,
                                                                                      worldSpaceRenderOperationConfiguration->kWSRenderOperationIsOcclusionQueryEnabledAttribute),
                                                         std::make_shared<CConfigurationAttribute>(isOcclusionQueryEnabled));

    ui32 index = node.attribute(worldSpaceRenderOperationConfiguration->kWSRenderOperationIndexAttribute.c_str()).as_uint();
    worldSpaceRenderOperationConfiguration->setAttribute(getConfigurationAttributeKey(worldSpaceRenderOperationConfiguration->kWSRenderOperationMainNode,
                                                                                 worldSpaceRenderOperationConfiguration->kWSRenderOperationIndexAttribute),
                                                    std::make_shared<CConfigurationAttribute>(index));
    
    ui32 screenWidth = node.attribute(worldSpaceRenderOperationConfiguration->kWSRenderOperationScreenWidthAttribute.c_str()).as_uint();
    worldSpaceRenderOperationConfiguration->setAttribute(getConfigurationAttributeKey(worldSpaceRenderOperationConfiguration->kWSRenderOperationMainNode,
                                                                                 worldSpaceRenderOperationConfiguration->kWSRenderOperationScreenWidthAttribute),
                                                    std::make_shared<CConfigurationAttribute>(screenWidth));
    
    ui32 screenHeight = node.attribute(worldSpaceRenderOperationConfiguration->kWSRenderOperationScreenHeightAttribute.c_str()).as_uint();
    worldSpaceRenderOperationConfiguration->setAttribute(getConfigurationAttributeKey(worldSpaceRenderOperationConfiguration->kWSRenderOperationMainNode,
                                                                                 worldSpaceRenderOperationConfiguration->kWSRenderOperationScreenHeightAttribute),
                                                    std::make_shared<CConfigurationAttribute>(screenHeight));
    
    f32 clearColorR = node.attribute(worldSpaceRenderOperationConfiguration->kWSRenderOperationClearColorRAttribute.c_str()).as_float();
    worldSpaceRenderOperationConfiguration->setAttribute(getConfigurationAttributeKey(worldSpaceRenderOperationConfiguration->kWSRenderOperationMainNode,
                                                                              worldSpaceRenderOperationConfiguration->kWSRenderOperationClearColorRAttribute),
                                                     std::make_shared<CConfigurationAttribute>(clearColorR));
    
    f32 clearColorG = node.attribute(worldSpaceRenderOperationConfiguration->kWSRenderOperationClearColorGAttribute.c_str()).as_float();
    worldSpaceRenderOperationConfiguration->setAttribute(getConfigurationAttributeKey(worldSpaceRenderOperationConfiguration->kWSRenderOperationMainNode,
                                                                              worldSpaceRenderOperationConfiguration->kWSRenderOperationClearColorGAttribute),
                                                     std::make_shared<CConfigurationAttribute>(clearColorG));
    
    f32 clearColorB = node.attribute(worldSpaceRenderOperationConfiguration->kWSRenderOperationClearColorBAttribute.c_str()).as_float();
    worldSpaceRenderOperationConfiguration->setAttribute(getConfigurationAttributeKey(worldSpaceRenderOperationConfiguration->kWSRenderOperationMainNode,
                                                                              worldSpaceRenderOperationConfiguration->kWSRenderOperationClearColorBAttribute),
                                                     std::make_shared<CConfigurationAttribute>(clearColorB));
    
    f32 clearColorA = node.attribute(worldSpaceRenderOperationConfiguration->kWSRenderOperationClearColorAAttribute.c_str()).as_float();
    worldSpaceRenderOperationConfiguration->setAttribute(getConfigurationAttributeKey(worldSpaceRenderOperationConfiguration->kWSRenderOperationMainNode,
                                                                              worldSpaceRenderOperationConfiguration->kWSRenderOperationClearColorAAttribute),
                                                     std::make_shared<CConfigurationAttribute>(clearColorA));
    return worldSpaceRenderOperationConfiguration;
}

void CConfigurationWSRenderOperationSerializer::deserialize(const std::string& filename,
                                                            ISharedConfigurationRef configuration)
{
    
}