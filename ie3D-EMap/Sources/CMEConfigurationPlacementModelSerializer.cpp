//
//  CMEConfigurationPlacementModelSerializer.cpp
//  ie3D-EMap
//
//  Created by sergey.sergeev on 10/27/14.
//
//

#include "CMEConfigurationPlacementModelSerializer.h"
#include "CMEConfigurationGameObjects.h"

CMEConfigurationPlacementModelSerializer::CMEConfigurationPlacementModelSerializer(void)
{
    
}

CMEConfigurationPlacementModelSerializer::~CMEConfigurationPlacementModelSerializer(void)
{
    
}

ISharedConfiguration CMEConfigurationPlacementModelSerializer::serialize(const std::string& filename)
{
    pugi::xml_document document;
    pugi::xml_parse_result result = IConfigurationSerializer::openXMLDocument(document, filename);
    assert(result.status == pugi::status_ok);
    
    std::shared_ptr<CMEConfigurationPlacementModel> placementModelConfiguration = std::make_shared<CMEConfigurationPlacementModel>();
    pugi::xml_node node = document.child(placementModelConfiguration->kPlacementModelMainNode.c_str());
    
    pugi::xml_node materialsNode = node.child(placementModelConfiguration->kGameObjectMaterialsConfigurationsNode.c_str());
    for (pugi::xml_node material = materialsNode.child(placementModelConfiguration->kGameObjectMaterialConfigurationNode.c_str());
         material;
         material = material.next_sibling(placementModelConfiguration->kGameObjectMaterialConfigurationNode.c_str()))
    {
        std::string filename = material.attribute(placementModelConfiguration->kGameObjectMaterialFilenameAttribute.c_str()).as_string();
        placementModelConfiguration->setAttribute(getConfigurationAttributeKey(placementModelConfiguration->kGameObjectMaterialsConfigurationsNode,
                                                                               placementModelConfiguration->kGameObjectMaterialConfigurationNode,
                                                                               placementModelConfiguration->kGameObjectMaterialFilenameAttribute),
                                                  std::make_shared<CConfigurationAttribute>(filename));
    }
    return placementModelConfiguration;
}

void CMEConfigurationPlacementModelSerializer::deserialize(const std::string&,
                                                           ISharedConfigurationRef)
{
    
}