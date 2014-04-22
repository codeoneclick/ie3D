//
//  CConfigurationORenderOperationSerializer.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 6/7/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CConfigurationORenderOperationSerializer.h"
#include "CConfigurationGameObjects.h"

CConfigurationORenderOperationSerializer::CConfigurationORenderOperationSerializer(void)
{
    
}

CConfigurationORenderOperationSerializer::~CConfigurationORenderOperationSerializer(void)
{
    
}

ISharedConfiguration CConfigurationORenderOperationSerializer::serialize(const std::string& filename)
{
    pugi::xml_document document;
    pugi::xml_parse_result result = IConfigurationSerializer::openXMLDocument(document, filename);
    assert(result.status == pugi::status_ok);
    
    std::shared_ptr<CConfigurationORenderOperation> outputRenderOperationConfiguration = std::make_shared<CConfigurationORenderOperation>();
    pugi::xml_node node = document.child(outputRenderOperationConfiguration->kORenderOperationMainNode.c_str());
    
    std::string guid = node.attribute(outputRenderOperationConfiguration->kORenderOperationGuidAttribute.c_str()).as_string();
    outputRenderOperationConfiguration->setAttribute(getConfigurationAttributeKey(outputRenderOperationConfiguration->kORenderOperationMainNode,
                                                                             outputRenderOperationConfiguration->kORenderOperationGuidAttribute),
                                                std::make_shared<CConfigurationAttribute>(guid));
    
    std::string materialFilename = node.attribute(outputRenderOperationConfiguration->kORenderOperationMaterialConfigurationFilenameAttribute.c_str()).as_string();
    outputRenderOperationConfiguration->setAttribute(getConfigurationAttributeKey(outputRenderOperationConfiguration->kORenderOperationMainNode,
                                                                          outputRenderOperationConfiguration->kORenderOperationMaterialConfigurationFilenameAttribute),
                                                 std::make_shared<CConfigurationAttribute>(materialFilename));

    return outputRenderOperationConfiguration;
}

void CConfigurationORenderOperationSerializer::deserialize(const std::string& filename,
                                                           ISharedConfigurationRef configuration)
{
    
}