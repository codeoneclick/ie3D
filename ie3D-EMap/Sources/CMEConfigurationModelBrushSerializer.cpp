//
//  CMEConfigurationModelBrushSerializer.cpp
//  ie3D-EMap
//
//  Created by sergey.sergeev on 10/27/14.
//
//

#include "CMEConfigurationModelBrushSerializer.h"
#include "CMEConfigurationGameObjects.h"
#include "CConfigurationMaterialSerializer.h"

CMEConfigurationModelBrushSerializer::CMEConfigurationModelBrushSerializer(void)
{
    
}

CMEConfigurationModelBrushSerializer::~CMEConfigurationModelBrushSerializer(void)
{
    
}

ISharedConfiguration CMEConfigurationModelBrushSerializer::serialize(const std::string& filename)
{
    pugi::xml_document document;
    pugi::xml_parse_result result = IConfigurationSerializer::openXMLDocument(document, filename);
    assert(result.status == pugi::status_ok);
    
    std::shared_ptr<CMEConfigurationModelBrush> configuration = std::make_shared<CMEConfigurationModelBrush>();
    pugi::xml_node mainNode = document.child(configuration->kModelBrushMainNode.c_str());
    
    pugi::xml_node elementsNode = mainNode.child(configuration->kModelBrushElementsConfigurationsNode.c_str());
    for (pugi::xml_node model = elementsNode.child(configuration->kModelBrushElementConfigurationNode.c_str());
         model;
         model = model.next_sibling(configuration->kModelBrushElementConfigurationNode.c_str()))
    {
        std::string filename = model.attribute(configuration->kModelBrushElementFilenameAttribute.c_str()).as_string();
        std::string name = model.attribute(configuration->kModelBrushElementNameAttribute.c_str()).as_string();
        
        configuration->setAttribute(getConfigurationAttributeKey(configuration->kModelBrushElementsConfigurationsNode,
                                                                 configuration->kModelBrushElementConfigurationNode,
                                                                 configuration->kModelBrushElementFilenameAttribute),
                                    std::make_shared<CConfigurationAttribute>(filename));
        
        configuration->setAttribute(getConfigurationAttributeKey(configuration->kModelBrushElementsConfigurationsNode,
                                                                 configuration->kModelBrushElementConfigurationNode,
                                                                 configuration->kModelBrushElementNameAttribute),
                                    std::make_shared<CConfigurationAttribute>(name));
    }
    return configuration;
}

void CMEConfigurationModelBrushSerializer::deserialize(const std::string&,
                                                       ISharedConfigurationRef)
{
    
}