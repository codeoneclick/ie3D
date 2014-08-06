//
//  CEConfigurationComplexModelSerializer.cpp
//  ie3D-Engine
//
//  Created by sergey.sergeev on 8/6/14.
//  Copyright (c) 2014 sergey.sergeev. All rights reserved.
//

#include "CEConfigurationComplexModelSerializer.h"
#include "CEConfigurationGameObjects.h"

CEConfigurationComplexModelSerializer::CEConfigurationComplexModelSerializer(void)
{
    
}

CEConfigurationComplexModelSerializer::~CEConfigurationComplexModelSerializer(void)
{
    
}

ISharedConfiguration CEConfigurationComplexModelSerializer::serialize(const std::string& filename)
{
    pugi::xml_document document;
    pugi::xml_parse_result result = IConfigurationSerializer::openXMLDocument(document, filename);
    assert(result.status == pugi::status_ok);
    
    std::shared_ptr<CEConfigurationComplexModel> complexModelConfiguration = std::make_shared<CEConfigurationComplexModel>();
    pugi::xml_node node = document.child(complexModelConfiguration->kComplexModelMainNode.c_str());
    
    pugi::xml_node modelsNode = node.child(complexModelConfiguration->kComplexModelsConfigurationsNode.c_str());
    for (pugi::xml_node model = modelsNode.child(complexModelConfiguration->kComplexModelConfigurationsNode.c_str());
         model;
         model = model.next_sibling(complexModelConfiguration->kComplexModelConfigurationsNode.c_str()))
    {
        std::string filename = model.attribute(complexModelConfiguration->kComplexModelFilenameAttribute.c_str()).as_string();
        std::string part = model.attribute(complexModelConfiguration->kComplexModelPartAttribute.c_str()).as_string();
        
        complexModelConfiguration->setAttribute(getConfigurationAttributeKey(complexModelConfiguration->kComplexModelsConfigurationsNode,
                                                                             complexModelConfiguration->kComplexModelConfigurationsNode,
                                                                             complexModelConfiguration->kComplexModelFilenameAttribute),
                                                std::make_shared<CConfigurationAttribute>(filename));
        
        complexModelConfiguration->setAttribute(getConfigurationAttributeKey(complexModelConfiguration->kComplexModelsConfigurationsNode,
                                                                             complexModelConfiguration->kComplexModelConfigurationsNode,
                                                                             complexModelConfiguration->kComplexModelPartAttribute),
                                                std::make_shared<CConfigurationAttribute>(part));
    }
    
    return complexModelConfiguration;
}

void CEConfigurationComplexModelSerializer::deserialize(const std::string&,
                                                        ISharedConfigurationRef)
{
    
}