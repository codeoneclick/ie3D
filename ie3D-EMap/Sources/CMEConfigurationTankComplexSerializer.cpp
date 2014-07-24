//
//  CMEConfigurationTankComplexSerializer.cpp
//  ie3D-EMap
//
//  Created by sergey.sergeev on 7/23/14.
//
//

#include "CMEConfigurationTankComplexSerializer.h"
#include "CMEConfigurationGameObjects.h"

CMEConfigurationTankComplexSerializer::CMEConfigurationTankComplexSerializer(void)
{
    
}

CMEConfigurationTankComplexSerializer::~CMEConfigurationTankComplexSerializer(void)
{
    
}

ISharedConfiguration CMEConfigurationTankComplexSerializer::serialize(const std::string& filename)
{
    pugi::xml_document document;
    pugi::xml_parse_result result = IConfigurationSerializer::openXMLDocument(document, filename);
    assert(result.status == pugi::status_ok);
    
    std::shared_ptr<CMEConfigurationTankComplex> tankComplexConfiguration = std::make_shared<CMEConfigurationTankComplex>();
    pugi::xml_node node = document.child(tankComplexConfiguration->kTankComplexMainNode.c_str());
    
    pugi::xml_node modelsNode = node.child(tankComplexConfiguration->kTankComplexModelsConfigurationsNode.c_str());
    for (pugi::xml_node model = modelsNode.child(tankComplexConfiguration->kTankComplexModelConfigurationsNode.c_str());
         model;
         model = model.next_sibling(tankComplexConfiguration->kTankComplexModelConfigurationsNode.c_str()))
    {
        std::string filename = model.attribute(tankComplexConfiguration->kTankComplexModelFilenameAttribute.c_str()).as_string();
        std::string part = model.attribute(tankComplexConfiguration->kTankComplexModelPartAttribute.c_str()).as_string();
        
        tankComplexConfiguration->setAttribute(getConfigurationAttributeKey(tankComplexConfiguration->kTankComplexModelsConfigurationsNode,
                                                                            tankComplexConfiguration->kTankComplexModelConfigurationsNode,
                                                                            tankComplexConfiguration->kTankComplexModelFilenameAttribute),
                                               std::make_shared<CConfigurationAttribute>(filename));
        
        tankComplexConfiguration->setAttribute(getConfigurationAttributeKey(tankComplexConfiguration->kTankComplexModelsConfigurationsNode,
                                                                            tankComplexConfiguration->kTankComplexModelConfigurationsNode,
                                                                            tankComplexConfiguration->kTankComplexModelPartAttribute),
                                               std::make_shared<CConfigurationAttribute>(part));
    }
    
    return tankComplexConfiguration;
}

void CMEConfigurationTankComplexSerializer::deserialize(const std::string&,
                                                        ISharedConfigurationRef)
{
    
}