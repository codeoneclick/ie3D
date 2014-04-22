//
//  CConfigurationParticleEmitterSerializer.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/28/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CConfigurationParticleEmitterSerializer.h"
#include "CConfigurationGameObjects.h"

CConfigurationParticleEmitterSerializer::CConfigurationParticleEmitterSerializer(void)
{
    
}

CConfigurationParticleEmitterSerializer::~CConfigurationParticleEmitterSerializer(void)
{
    
}

ISharedConfiguration CConfigurationParticleEmitterSerializer::serialize(const std::string& filename)
{
    pugi::xml_document document;
    pugi::xml_parse_result result = IConfigurationSerializer::openXMLDocument(document, filename);
    assert(result.status == pugi::status_ok);
    
    std::shared_ptr<CConfigurationParticleEmitter> particleEmitterConfiguration = std::make_shared<CConfigurationParticleEmitter>();
    
    pugi::xml_node node = document.child(particleEmitterConfiguration->kParticleEmitterMainNode.c_str());
    
    ui32 numParticles = node.attribute(particleEmitterConfiguration->kParticleEmitterNumParticlesAttribute.c_str()).as_uint();
    particleEmitterConfiguration->setAttribute(getConfigurationAttributeKey(particleEmitterConfiguration->kParticleEmitterMainNode,
                                                                    particleEmitterConfiguration->kParticleEmitterNumParticlesAttribute),
                                           std::make_shared<CConfigurationAttribute>(numParticles));
    
    ui32 duration = node.attribute(particleEmitterConfiguration->kParticleEmitterDurationAttribute.c_str()).as_uint();
    particleEmitterConfiguration->setAttribute(getConfigurationAttributeKey(particleEmitterConfiguration->kParticleEmitterMainNode,
                                                                    particleEmitterConfiguration->kParticleEmitterDurationAttribute),
                                           std::make_shared<CConfigurationAttribute>(duration));
    
    f32 durationRandomness = node.attribute(particleEmitterConfiguration->kParticleEmitterDurationRandomnessAttribute.c_str()).as_float();
    particleEmitterConfiguration->setAttribute(getConfigurationAttributeKey(particleEmitterConfiguration->kParticleEmitterMainNode,
                                                                    particleEmitterConfiguration->kParticleEmitterDurationRandomnessAttribute),
                                           std::make_shared<CConfigurationAttribute>(durationRandomness));

    f32 velocitySensitivity = node.attribute(particleEmitterConfiguration->kParticleEmitterVelocitySensitivityAttribute.c_str()).as_float();
    particleEmitterConfiguration->setAttribute(getConfigurationAttributeKey(particleEmitterConfiguration->kParticleEmitterMainNode,
                                                                    particleEmitterConfiguration->kParticleEmitterVelocitySensitivityAttribute),
                                           std::make_shared<CConfigurationAttribute>(velocitySensitivity));
    
    f32 minHorizontalVelocity = node.attribute(particleEmitterConfiguration->kParticleEmitterMinHorizontalVelocityAttribute.c_str()).as_float();
    particleEmitterConfiguration->setAttribute(getConfigurationAttributeKey(particleEmitterConfiguration->kParticleEmitterMainNode,
                                                                    particleEmitterConfiguration->kParticleEmitterMinHorizontalVelocityAttribute),
                                           std::make_shared<CConfigurationAttribute>(minHorizontalVelocity));
    
    f32 maxHorizontalVelocity = node.attribute(particleEmitterConfiguration->kParticleEmitterMaxHorizontalVelocityAttribute.c_str()).as_float();
    particleEmitterConfiguration->setAttribute(getConfigurationAttributeKey(particleEmitterConfiguration->kParticleEmitterMainNode,
                                                                    particleEmitterConfiguration->kParticleEmitterMaxHorizontalVelocityAttribute),
                                           std::make_shared<CConfigurationAttribute>(maxHorizontalVelocity));
    
    f32 minVerticalVelocity = node.attribute(particleEmitterConfiguration->kParticleEmitterMinVerticalVelocityAttribute.c_str()).as_float();
    particleEmitterConfiguration->setAttribute(getConfigurationAttributeKey(particleEmitterConfiguration->kParticleEmitterMainNode,
                                                                    particleEmitterConfiguration->kParticleEmitterMinVerticalVelocityAttribute),
                                           std::make_shared<CConfigurationAttribute>(minVerticalVelocity));
    
    f32 maxVerticalVelocity = node.attribute(particleEmitterConfiguration->kParticleEmitterMaxVerticalVelocityAttribute.c_str()).as_float();
    particleEmitterConfiguration->setAttribute(getConfigurationAttributeKey(particleEmitterConfiguration->kParticleEmitterMainNode,
                                                                    particleEmitterConfiguration->kParticleEmitterMaxVerticalVelocityAttribute),
                                           std::make_shared<CConfigurationAttribute>(maxVerticalVelocity));
    
    f32 endVelocity = node.attribute(particleEmitterConfiguration->kParticleEmitterEndVelocityAttribute.c_str()).as_float();
    particleEmitterConfiguration->setAttribute(getConfigurationAttributeKey(particleEmitterConfiguration->kParticleEmitterMainNode,
                                                                    particleEmitterConfiguration->kParticleEmitterEndVelocityAttribute),
                                           std::make_shared<CConfigurationAttribute>(endVelocity));
    
    f32 gravityX = node.attribute(particleEmitterConfiguration->kParticleEmitterGravityXAttribute.c_str()).as_float();
    particleEmitterConfiguration->setAttribute(getConfigurationAttributeKey(particleEmitterConfiguration->kParticleEmitterMainNode,
                                                                    particleEmitterConfiguration->kParticleEmitterGravityXAttribute),
                                           std::make_shared<CConfigurationAttribute>(gravityX));
    
    f32 gravityY = node.attribute(particleEmitterConfiguration->kParticleEmitterGravityYAttribute.c_str()).as_float();
    particleEmitterConfiguration->setAttribute(getConfigurationAttributeKey(particleEmitterConfiguration->kParticleEmitterMainNode,
                                                                    particleEmitterConfiguration->kParticleEmitterGravityYAttribute),
                                           std::make_shared<CConfigurationAttribute>(gravityY));
    
    f32 gravityZ = node.attribute(particleEmitterConfiguration->kParticleEmitterGravityZAttribute.c_str()).as_float();
    particleEmitterConfiguration->setAttribute(getConfigurationAttributeKey(particleEmitterConfiguration->kParticleEmitterMainNode,
                                                                    particleEmitterConfiguration->kParticleEmitterGravityZAttribute),
                                           std::make_shared<CConfigurationAttribute>(gravityZ));
    
    ui8 sourceColorR = static_cast<ui8>(node.attribute(particleEmitterConfiguration->kParticleEmitterSourceColorRAttribute.c_str()).as_uint());
    particleEmitterConfiguration->setAttribute(getConfigurationAttributeKey(particleEmitterConfiguration->kParticleEmitterMainNode,
                                                                    particleEmitterConfiguration->kParticleEmitterSourceColorRAttribute),
                                           std::make_shared<CConfigurationAttribute>(sourceColorR));
    
    ui8 sourceColorG = static_cast<ui8>(node.attribute(particleEmitterConfiguration->kParticleEmitterSourceColorGAttribute.c_str()).as_uint());
    particleEmitterConfiguration->setAttribute(getConfigurationAttributeKey(particleEmitterConfiguration->kParticleEmitterMainNode,
                                                                    particleEmitterConfiguration->kParticleEmitterSourceColorGAttribute),
                                           std::make_shared<CConfigurationAttribute>(sourceColorG));
    
    ui8 sourceColorB = static_cast<ui8>(node.attribute(particleEmitterConfiguration->kParticleEmitterSourceColorBAttribute.c_str()).as_uint());
    particleEmitterConfiguration->setAttribute(getConfigurationAttributeKey(particleEmitterConfiguration->kParticleEmitterMainNode,
                                                                    particleEmitterConfiguration->kParticleEmitterSourceColorBAttribute),
                                           std::make_shared<CConfigurationAttribute>(sourceColorB));
    
    ui8 sourceColorA = static_cast<ui8>(node.attribute(particleEmitterConfiguration->kParticleEmitterSourceColorAAttribute.c_str()).as_uint());
    particleEmitterConfiguration->setAttribute(getConfigurationAttributeKey(particleEmitterConfiguration->kParticleEmitterMainNode,
                                                                    particleEmitterConfiguration->kParticleEmitterSourceColorAAttribute),
                                           std::make_shared<CConfigurationAttribute>(sourceColorA));
    
    ui8 destinationColorR = static_cast<ui8>(node.attribute(particleEmitterConfiguration->kParticleEmitterDestinationColorRAttribute.c_str()).as_uint());
    particleEmitterConfiguration->setAttribute(getConfigurationAttributeKey(particleEmitterConfiguration->kParticleEmitterMainNode,
                                                                    particleEmitterConfiguration->kParticleEmitterDestinationColorRAttribute),
                                           std::make_shared<CConfigurationAttribute>(destinationColorR));
    
    ui8 destinationColorG = static_cast<ui8>(node.attribute(particleEmitterConfiguration->kParticleEmitterDestinationColorGAttribute.c_str()).as_uint());
    particleEmitterConfiguration->setAttribute(getConfigurationAttributeKey(particleEmitterConfiguration->kParticleEmitterMainNode,
                                                                    particleEmitterConfiguration->kParticleEmitterDestinationColorGAttribute),
                                           std::make_shared<CConfigurationAttribute>(destinationColorG));
    
    ui8 destinationColorB = static_cast<ui8>(node.attribute(particleEmitterConfiguration->kParticleEmitterDestinationColorBAttribute.c_str()).as_uint());
    particleEmitterConfiguration->setAttribute(getConfigurationAttributeKey(particleEmitterConfiguration->kParticleEmitterMainNode,
                                                                    particleEmitterConfiguration->kParticleEmitterDestinationColorBAttribute),
                                           std::make_shared<CConfigurationAttribute>(destinationColorB));
    
    ui8 destinationColorA = static_cast<ui8>(node.attribute(particleEmitterConfiguration->kParticleEmitterDestinationColorAAttribute.c_str()).as_uint());
    particleEmitterConfiguration->setAttribute(getConfigurationAttributeKey(particleEmitterConfiguration->kParticleEmitterMainNode,
                                                                    particleEmitterConfiguration->kParticleEmitterDestinationColorAAttribute),
                                           std::make_shared<CConfigurationAttribute>(destinationColorA));

    f32 sourceSizeX = node.attribute(particleEmitterConfiguration->kParticleEmitterSourceSizeXAttribute.c_str()).as_float();
    particleEmitterConfiguration->setAttribute(getConfigurationAttributeKey(particleEmitterConfiguration->kParticleEmitterMainNode,
                                                                    particleEmitterConfiguration->kParticleEmitterSourceSizeXAttribute),
                                           std::make_shared<CConfigurationAttribute>(sourceSizeX));
    
    f32 sourceSizeY = node.attribute(particleEmitterConfiguration->kParticleEmitterSourceSizeYAttribute.c_str()).as_float();
    particleEmitterConfiguration->setAttribute(getConfigurationAttributeKey(particleEmitterConfiguration->kParticleEmitterMainNode,
                                                                    particleEmitterConfiguration->kParticleEmitterSourceSizeYAttribute),
                                           std::make_shared<CConfigurationAttribute>(sourceSizeY));
    
    f32 destinationSizeX = node.attribute(particleEmitterConfiguration->kParticleEmitterDestinationSizeXAttribute.c_str()).as_float();
    particleEmitterConfiguration->setAttribute(getConfigurationAttributeKey(particleEmitterConfiguration->kParticleEmitterMainNode,
                                                                    particleEmitterConfiguration->kParticleEmitterDestinationSizeXAttribute),
                                           std::make_shared<CConfigurationAttribute>(destinationSizeX));
    
    f32 destinationSizeY = node.attribute(particleEmitterConfiguration->kParticleEmitterDestinationSizeYAttribute.c_str()).as_float();
    particleEmitterConfiguration->setAttribute(getConfigurationAttributeKey(particleEmitterConfiguration->kParticleEmitterMainNode,
                                                                    particleEmitterConfiguration->kParticleEmitterDestinationSizeYAttribute),
                                           std::make_shared<CConfigurationAttribute>(destinationSizeY));
    
    ui32 minEmittInterval = node.attribute(particleEmitterConfiguration->kParticleEmitterMinEmittIntervalAttribute.c_str()).as_uint();
    particleEmitterConfiguration->setAttribute(getConfigurationAttributeKey(particleEmitterConfiguration->kParticleEmitterMainNode,
                                                                    particleEmitterConfiguration->kParticleEmitterMinEmittIntervalAttribute),
                                           std::make_shared<CConfigurationAttribute>(minEmittInterval));
    
    ui32 maxEmittInterval = node.attribute(particleEmitterConfiguration->kParticleEmitterMaxEmittIntervalAttribute.c_str()).as_uint();
    particleEmitterConfiguration->setAttribute(getConfigurationAttributeKey(particleEmitterConfiguration->kParticleEmitterMainNode,
                                                                    particleEmitterConfiguration->kParticleEmitterMaxEmittIntervalAttribute),
                                           std::make_shared<CConfigurationAttribute>(maxEmittInterval));
    
    pugi::xml_node materialsNode = node.child(particleEmitterConfiguration->kGameObjectMaterialsConfigurationsNode.c_str());
    for (pugi::xml_node material = materialsNode.child(particleEmitterConfiguration->kGameObjectMaterialConfigurationNode.c_str());
         material;
         material = material.next_sibling(particleEmitterConfiguration->kGameObjectMaterialConfigurationNode.c_str()))
    {
        std::string filename = material.attribute(particleEmitterConfiguration->kGameObjectMaterialFilenameAttribute.c_str()).as_string();
        particleEmitterConfiguration->setAttribute(getConfigurationAttributeKey(particleEmitterConfiguration->kGameObjectMaterialsConfigurationsNode,
                                                                        particleEmitterConfiguration->kGameObjectMaterialConfigurationNode,
                                                                        particleEmitterConfiguration->kGameObjectMaterialFilenameAttribute),
                                               std::make_shared<CConfigurationAttribute>(filename));
    }
    
    return particleEmitterConfiguration;
}

void CConfigurationParticleEmitterSerializer::deserialize(const std::string& filename,
                                                          ISharedConfigurationRef configuration)
{
    
}
