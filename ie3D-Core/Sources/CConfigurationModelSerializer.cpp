//
//  CConfigurationModelSerializer.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/28/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CConfigurationModelSerializer.h"
#include "CConfigurationGameObjects.h"

CConfigurationModelSerializer::CConfigurationModelSerializer(void)
{
    
}

CConfigurationModelSerializer::~CConfigurationModelSerializer(void)
{
    
}

ISharedConfiguration CConfigurationModelSerializer::serialize(const std::string& filename)
{
    pugi::xml_document document;
    pugi::xml_parse_result result = IConfigurationSerializer::openXMLDocument(document, filename);
    assert(result.status == pugi::status_ok);
    
    std::shared_ptr<CConfigurationModel> modelConfiguration = std::make_shared<CConfigurationModel>();
    pugi::xml_node node = document.child(modelConfiguration->kModelMainNode.c_str());
    
    std::string meshFilename = node.attribute(modelConfiguration->kModelMeshFilenameAttribute.c_str()).as_string();
    modelConfiguration->setAttribute(getConfigurationAttributeKey(modelConfiguration->kModelMainNode,
                                                             modelConfiguration->kModelMeshFilenameAttribute),
                                std::make_shared<CConfigurationAttribute>(meshFilename));
    
    bool isBatching = node.attribute(modelConfiguration->kModelMeshIsBatchingAttribute.c_str()).as_bool();
    modelConfiguration->setAttribute(getConfigurationAttributeKey(modelConfiguration->kModelMainNode,
                                                             modelConfiguration->kModelMeshIsBatchingAttribute),
                                std::make_shared<CConfigurationAttribute>(isBatching));
    
    pugi::xml_node materialsNode = node.child(modelConfiguration->kGameObjectMaterialsConfigurationsNode.c_str());
    for (pugi::xml_node material = materialsNode.child(modelConfiguration->kGameObjectMaterialConfigurationNode.c_str());
         material;
         material = material.next_sibling(modelConfiguration->kGameObjectMaterialConfigurationNode.c_str()))
    {        std::string filename = material.attribute(modelConfiguration->kGameObjectMaterialFilenameAttribute.c_str()).as_string();
        modelConfiguration->setAttribute(getConfigurationAttributeKey(modelConfiguration->kGameObjectMaterialsConfigurationsNode,
                                                                 modelConfiguration->kGameObjectMaterialConfigurationNode,
                                                                 modelConfiguration->kGameObjectMaterialFilenameAttribute),
                                    std::make_shared<CConfigurationAttribute>(filename));
    }
    
    return modelConfiguration;
}

void CConfigurationModelSerializer::deserialize(const std::string& filename,
                                                ISharedConfigurationRef configuration)
{
    /*std::shared_ptr<SmodelConfiguration> modelConfiguration = std::static_pointer_cast<SmodelConfiguration>(_template);
     pugi::xml_document document;
     document.load("<model name=\"\">\
     <mesh filename=\"\" is_batching=\"\"/>\
     <skeleton filename=\"\"/>\
     <animations>\
     <animation name=\"\" filename=\"\"/>\
     </animations>\
     <materials>\
     <material filename=\"\"/>\
     </materials>\
     </model>");
     pugi::xml_node node = document.child("model");
     node.attribute("name").set_value(modelConfiguration->m_meshFilename.c_str());
     node.child("mesh").attribute("filename").set_value(modelConfiguration->m_meshFilename.c_str());
     node.child("mesh").attribute("is_batching").set_value(0);
     node.child("skeleton").attribute("filename").set_value(modelConfiguration->m_skeletonFilename.c_str());
     for(auto sequence : modelConfiguration->m_sequencesFilenames)
     {
     node.child("animations").append_child("animation");
     node.child("animations").child("animation").append_attribute("filename");
     node.child("animations").child("animation").attribute("filename").set_value(sequence.c_str());
     }
     for(auto material : modelConfiguration->m_materialsFilenames)
     {
     node.child("materials").append_child("material");
     node.child("materials").child("material").append_attribute("filename");
     node.child("materials").child("material").attribute("filename").set_value(material.c_str());
     }*/
}
