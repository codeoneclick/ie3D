//
//  CConfigurationModelSerializer.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/28/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CConfigurationModelSerializer.h"
#include "CConfigurationMaterialSerializer.h"
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
    pugi::xml_node mainNode = document.child(modelConfiguration->kModelMainNode.c_str());
    
    std::string meshFilename = mainNode.attribute(modelConfiguration->kModelMeshFilenameAttribute.c_str()).as_string();
    modelConfiguration->setAttribute(getConfigurationAttributeKey(modelConfiguration->kModelMainNode,
                                                                  modelConfiguration->kModelMeshFilenameAttribute),
                                     std::make_shared<CConfigurationAttribute>(meshFilename));
    
    bool isBatching = mainNode.attribute(modelConfiguration->kModelMeshIsBatchingAttribute.c_str()).as_bool();
    modelConfiguration->setAttribute(getConfigurationAttributeKey(modelConfiguration->kModelMainNode,
                                                                  modelConfiguration->kModelMeshIsBatchingAttribute),
                                     std::make_shared<CConfigurationAttribute>(isBatching));
    

    pugi::xml_node animationsNode = mainNode.child(modelConfiguration->kModelAnimationsConfigurationsNode.c_str());
    for (pugi::xml_node animationNode = animationsNode.child(modelConfiguration->kModelAnimationConfigurationNode.c_str());
         animationNode;
         animationNode = animationNode.next_sibling(modelConfiguration->kModelAnimationConfigurationNode.c_str()))
    {
        std::shared_ptr<CConfigurationAnimation> animationConfiguration = std::make_shared<CConfigurationAnimation>();
        
        std::string filename = animationNode.attribute(animationConfiguration->kAnimationFilenameAttribute.c_str()).as_string();
        animationConfiguration->setAttribute(getConfigurationAttributeKey(animationConfiguration->kAnimationMainNode,
                                                                          animationConfiguration->kAnimationFilenameAttribute),
                                             std::make_shared<CConfigurationAttribute>(filename));
        
        std::string name = animationNode.attribute(animationConfiguration->kAnimationNameAttribute.c_str()).as_string();
        animationConfiguration->setAttribute(getConfigurationAttributeKey(animationConfiguration->kAnimationMainNode,
                                                                          animationConfiguration->kAnimationNameAttribute),
                                             std::make_shared<CConfigurationAttribute>(name));
        
        modelConfiguration->setConfiguration(getConfigurationAttributeKey(modelConfiguration->kModelMainNode,
                                                                          modelConfiguration->kModelAnimationsConfigurationsNode),
                                             animationConfiguration);
    }

    
    CConfigurationMaterialSerializer::serializeGameObjectMaterialsNode(modelConfiguration,
                                                                       mainNode);
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
