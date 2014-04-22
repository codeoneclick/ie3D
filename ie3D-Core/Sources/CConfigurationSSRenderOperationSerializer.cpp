//
//  CScreenSpaceOperationTemplateSerializer.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 6/7/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CConfigurationSSRenderOperationSerializer.h"
#include "CConfigurationGameObjects.h"

CConfigurationSSRenderOperationSerializer::CConfigurationSSRenderOperationSerializer(void)
{
    
}

CConfigurationSSRenderOperationSerializer::~CConfigurationSSRenderOperationSerializer(void)
{
    
}

ISharedConfiguration CConfigurationSSRenderOperationSerializer::serialize(const std::string& filename)
{
    pugi::xml_document document;
    pugi::xml_parse_result result = IConfigurationSerializer::openXMLDocument(document, filename);
    assert(result.status == pugi::status_ok);
    
    std::shared_ptr<CConfigurationSSRenderOperation> screenSpaceRenderOperationConfiguration = std::make_shared<CConfigurationSSRenderOperation>();
    pugi::xml_node node = document.child(screenSpaceRenderOperationConfiguration->kSSRenderOperationMainNode.c_str());
    
    std::string guid = node.attribute(screenSpaceRenderOperationConfiguration->kSSRenderOperationGuidAttribute.c_str()).as_string();
    screenSpaceRenderOperationConfiguration->setAttribute(getConfigurationAttributeKey(screenSpaceRenderOperationConfiguration->kSSRenderOperationMainNode,
                                                                               screenSpaceRenderOperationConfiguration->kSSRenderOperationGuidAttribute),
                                                      std::make_shared<CConfigurationAttribute>(guid));

    ui32 screenWidth = node.attribute(screenSpaceRenderOperationConfiguration->kSSRenderOperationScreenWidthAttribute.c_str()).as_uint();
    screenSpaceRenderOperationConfiguration->setAttribute(getConfigurationAttributeKey(screenSpaceRenderOperationConfiguration->kSSRenderOperationMainNode,
                                                                               screenSpaceRenderOperationConfiguration->kSSRenderOperationScreenWidthAttribute),
                                                      std::make_shared<CConfigurationAttribute>(screenWidth));

    
    ui32 screenHeight = node.attribute(screenSpaceRenderOperationConfiguration->kSSRenderOperationScreenHeightAttribute.c_str()).as_uint();
    screenSpaceRenderOperationConfiguration->setAttribute(getConfigurationAttributeKey(screenSpaceRenderOperationConfiguration->kSSRenderOperationMainNode,
                                                                               screenSpaceRenderOperationConfiguration->kSSRenderOperationScreenHeightAttribute),
                                                      std::make_shared<CConfigurationAttribute>(screenHeight));
    
    std::string materialFilename = node.attribute(screenSpaceRenderOperationConfiguration->kSSRenderOperationMaterialConfigurationFilenameAttribute.c_str()).as_string();
    screenSpaceRenderOperationConfiguration->setAttribute(getConfigurationAttributeKey(screenSpaceRenderOperationConfiguration->kSSRenderOperationMainNode,
                                                                               screenSpaceRenderOperationConfiguration->kSSRenderOperationMaterialConfigurationFilenameAttribute),
                                                      std::make_shared<CConfigurationAttribute>(materialFilename));
    return screenSpaceRenderOperationConfiguration;
}

void CConfigurationSSRenderOperationSerializer::deserialize(const std::string& filename,
                                                            ISharedConfigurationRef configuration)
{
    
}