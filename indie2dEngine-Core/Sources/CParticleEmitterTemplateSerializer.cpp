//
//  CParticleEmitterTemplateSerializer.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/28/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CParticleEmitterTemplateSerializer.h"
#include "CTemplateGameObjects.h"

CParticleEmitterTemplateSerializer::CParticleEmitterTemplateSerializer(void)
{
    
}

CParticleEmitterTemplateSerializer::~CParticleEmitterTemplateSerializer(void)
{
    
}

std::shared_ptr<I_RO_TemplateCommon> CParticleEmitterTemplateSerializer::Serialize(const std::string& _filename)
{
    pugi::xml_document document;
    pugi::xml_parse_result result = ITemplateSerializer::_LoadDocument(document, _filename);
    assert(result.status == pugi::status_ok);
    
    std::shared_ptr<CParticleEmitterTemplate> particleEmitterTemplate = std::make_shared<CParticleEmitterTemplate>();
    
    pugi::xml_node node = document.child(particleEmitterTemplate->kParticleEmitterMainNode.c_str());
    
    ui32 numParticles = node.attribute(particleEmitterTemplate->kParticleEmitterNumParticlesAttribute.c_str()).as_uint();
    particleEmitterTemplate->Set_Attribute(Get_TemplateAttributeKey(particleEmitterTemplate->kParticleEmitterMainNode,
                                                                    particleEmitterTemplate->kParticleEmitterNumParticlesAttribute),
                                           E_TEMPLATE_META_TYPE_UI32,
                                           &numParticles);
    
    ui32 duration = node.attribute(particleEmitterTemplate->kParticleEmitterDurationAttribute.c_str()).as_uint();
    particleEmitterTemplate->Set_Attribute(Get_TemplateAttributeKey(particleEmitterTemplate->kParticleEmitterMainNode,
                                                                    particleEmitterTemplate->kParticleEmitterDurationAttribute),
                                           E_TEMPLATE_META_TYPE_UI32,
                                           &duration);
    
    f32 durationRandomness = node.attribute(particleEmitterTemplate->kParticleEmitterDurationRandomnessAttribute.c_str()).as_float();
    particleEmitterTemplate->Set_Attribute(Get_TemplateAttributeKey(particleEmitterTemplate->kParticleEmitterMainNode,
                                                                    particleEmitterTemplate->kParticleEmitterDurationRandomnessAttribute),
                                           E_TEMPLATE_META_TYPE_F32,
                                           &durationRandomness);

    f32 velocitySensitivity = node.attribute(particleEmitterTemplate->kParticleEmitterVelocitySensitivityAttribute.c_str()).as_float();
    particleEmitterTemplate->Set_Attribute(Get_TemplateAttributeKey(particleEmitterTemplate->kParticleEmitterMainNode,
                                                                    particleEmitterTemplate->kParticleEmitterVelocitySensitivityAttribute),
                                           E_TEMPLATE_META_TYPE_F32,
                                           &velocitySensitivity);
    
    f32 minHorizontalVelocity = node.attribute(particleEmitterTemplate->kParticleEmitterMinHorizontalVelocityAttribute.c_str()).as_float();
    particleEmitterTemplate->Set_Attribute(Get_TemplateAttributeKey(particleEmitterTemplate->kParticleEmitterMainNode,
                                                                    particleEmitterTemplate->kParticleEmitterMinHorizontalVelocityAttribute),
                                           E_TEMPLATE_META_TYPE_F32,
                                           &minHorizontalVelocity);
    
    f32 maxHorizontalVelocity = node.attribute(particleEmitterTemplate->kParticleEmitterMaxHorizontalVelocityAttribute.c_str()).as_float();
    particleEmitterTemplate->Set_Attribute(Get_TemplateAttributeKey(particleEmitterTemplate->kParticleEmitterMainNode,
                                                                    particleEmitterTemplate->kParticleEmitterMaxHorizontalVelocityAttribute),
                                           E_TEMPLATE_META_TYPE_F32,
                                           &maxHorizontalVelocity);
    
    f32 minVerticalVelocity = node.attribute(particleEmitterTemplate->kParticleEmitterMinVerticalVelocityAttribute.c_str()).as_float();
    particleEmitterTemplate->Set_Attribute(Get_TemplateAttributeKey(particleEmitterTemplate->kParticleEmitterMainNode,
                                                                    particleEmitterTemplate->kParticleEmitterMinVerticalVelocityAttribute),
                                           E_TEMPLATE_META_TYPE_F32,
                                           &minVerticalVelocity);
    
    f32 maxVerticalVelocity = node.attribute(particleEmitterTemplate->kParticleEmitterMaxVerticalVelocityAttribute.c_str()).as_float();
    particleEmitterTemplate->Set_Attribute(Get_TemplateAttributeKey(particleEmitterTemplate->kParticleEmitterMainNode,
                                                                    particleEmitterTemplate->kParticleEmitterMaxVerticalVelocityAttribute),
                                           E_TEMPLATE_META_TYPE_F32,
                                           &maxVerticalVelocity);
    
    f32 endVelocity = node.attribute(particleEmitterTemplate->kParticleEmitterEndVelocityAttribute.c_str()).as_float();
    particleEmitterTemplate->Set_Attribute(Get_TemplateAttributeKey(particleEmitterTemplate->kParticleEmitterMainNode,
                                                                    particleEmitterTemplate->kParticleEmitterEndVelocityAttribute),
                                           E_TEMPLATE_META_TYPE_F32,
                                           &endVelocity);
    
    f32 gravityX = node.attribute(particleEmitterTemplate->kParticleEmitterGravityXAttribute.c_str()).as_float();
    particleEmitterTemplate->Set_Attribute(Get_TemplateAttributeKey(particleEmitterTemplate->kParticleEmitterMainNode,
                                                                    particleEmitterTemplate->kParticleEmitterGravityXAttribute),
                                           E_TEMPLATE_META_TYPE_F32,
                                           &gravityX);
    
    f32 gravityY = node.attribute(particleEmitterTemplate->kParticleEmitterGravityYAttribute.c_str()).as_float();
    particleEmitterTemplate->Set_Attribute(Get_TemplateAttributeKey(particleEmitterTemplate->kParticleEmitterMainNode,
                                                                    particleEmitterTemplate->kParticleEmitterGravityYAttribute),
                                           E_TEMPLATE_META_TYPE_F32,
                                           &gravityY);
    
    f32 gravityZ = node.attribute(particleEmitterTemplate->kParticleEmitterGravityZAttribute.c_str()).as_float();
    particleEmitterTemplate->Set_Attribute(Get_TemplateAttributeKey(particleEmitterTemplate->kParticleEmitterMainNode,
                                                                    particleEmitterTemplate->kParticleEmitterGravityZAttribute),
                                           E_TEMPLATE_META_TYPE_F32,
                                           &gravityZ);
    
    ui8 sourceColorR = static_cast<ui8>(node.attribute(particleEmitterTemplate->kParticleEmitterSourceColorRAttribute.c_str()).as_uint());
    particleEmitterTemplate->Set_Attribute(Get_TemplateAttributeKey(particleEmitterTemplate->kParticleEmitterMainNode,
                                                                    particleEmitterTemplate->kParticleEmitterSourceColorRAttribute),
                                           E_TEMPLATE_META_TYPE_UI8,
                                           &sourceColorR);
    
    ui8 sourceColorG = static_cast<ui8>(node.attribute(particleEmitterTemplate->kParticleEmitterSourceColorGAttribute.c_str()).as_uint());
    particleEmitterTemplate->Set_Attribute(Get_TemplateAttributeKey(particleEmitterTemplate->kParticleEmitterMainNode,
                                                                    particleEmitterTemplate->kParticleEmitterSourceColorGAttribute),
                                           E_TEMPLATE_META_TYPE_UI8,
                                           &sourceColorG);
    
    ui8 sourceColorB = static_cast<ui8>(node.attribute(particleEmitterTemplate->kParticleEmitterSourceColorBAttribute.c_str()).as_uint());
    particleEmitterTemplate->Set_Attribute(Get_TemplateAttributeKey(particleEmitterTemplate->kParticleEmitterMainNode,
                                                                    particleEmitterTemplate->kParticleEmitterSourceColorBAttribute),
                                           E_TEMPLATE_META_TYPE_UI8,
                                           &sourceColorB);
    
    ui8 sourceColorA = static_cast<ui8>(node.attribute(particleEmitterTemplate->kParticleEmitterSourceColorAAttribute.c_str()).as_uint());
    particleEmitterTemplate->Set_Attribute(Get_TemplateAttributeKey(particleEmitterTemplate->kParticleEmitterMainNode,
                                                                    particleEmitterTemplate->kParticleEmitterSourceColorAAttribute),
                                           E_TEMPLATE_META_TYPE_UI8,
                                           &sourceColorA);
    
    ui8 destinationColorR = static_cast<ui8>(node.attribute(particleEmitterTemplate->kParticleEmitterDestinationColorRAttribute.c_str()).as_uint());
    particleEmitterTemplate->Set_Attribute(Get_TemplateAttributeKey(particleEmitterTemplate->kParticleEmitterMainNode,
                                                                    particleEmitterTemplate->kParticleEmitterDestinationColorRAttribute),
                                           E_TEMPLATE_META_TYPE_UI8,
                                           &destinationColorR);
    
    ui8 destinationColorG = static_cast<ui8>(node.attribute(particleEmitterTemplate->kParticleEmitterDestinationColorGAttribute.c_str()).as_uint());
    particleEmitterTemplate->Set_Attribute(Get_TemplateAttributeKey(particleEmitterTemplate->kParticleEmitterMainNode,
                                                                    particleEmitterTemplate->kParticleEmitterDestinationColorGAttribute),
                                           E_TEMPLATE_META_TYPE_UI8,
                                           &destinationColorG);
    
    ui8 destinationColorB = static_cast<ui8>(node.attribute(particleEmitterTemplate->kParticleEmitterDestinationColorBAttribute.c_str()).as_uint());
    particleEmitterTemplate->Set_Attribute(Get_TemplateAttributeKey(particleEmitterTemplate->kParticleEmitterMainNode,
                                                                    particleEmitterTemplate->kParticleEmitterDestinationColorBAttribute),
                                           E_TEMPLATE_META_TYPE_UI8,
                                           &destinationColorB);
    
    ui8 destinationColorA = static_cast<ui8>(node.attribute(particleEmitterTemplate->kParticleEmitterDestinationColorAAttribute.c_str()).as_uint());
    particleEmitterTemplate->Set_Attribute(Get_TemplateAttributeKey(particleEmitterTemplate->kParticleEmitterMainNode,
                                                                    particleEmitterTemplate->kParticleEmitterDestinationColorAAttribute),
                                           E_TEMPLATE_META_TYPE_UI8,
                                           &destinationColorA);

    f32 sourceSizeX = node.attribute(particleEmitterTemplate->kParticleEmitterSourceSizeXAttribute.c_str()).as_float();
    particleEmitterTemplate->Set_Attribute(Get_TemplateAttributeKey(particleEmitterTemplate->kParticleEmitterMainNode,
                                                                    particleEmitterTemplate->kParticleEmitterSourceSizeXAttribute),
                                           E_TEMPLATE_META_TYPE_F32,
                                           &sourceSizeX);
    
    f32 sourceSizeY = node.attribute(particleEmitterTemplate->kParticleEmitterSourceSizeYAttribute.c_str()).as_float();
    particleEmitterTemplate->Set_Attribute(Get_TemplateAttributeKey(particleEmitterTemplate->kParticleEmitterMainNode,
                                                                    particleEmitterTemplate->kParticleEmitterSourceSizeYAttribute),
                                           E_TEMPLATE_META_TYPE_F32,
                                           &sourceSizeY);
    
    f32 destinationSizeX = node.attribute(particleEmitterTemplate->kParticleEmitterDestinationSizeXAttribute.c_str()).as_float();
    particleEmitterTemplate->Set_Attribute(Get_TemplateAttributeKey(particleEmitterTemplate->kParticleEmitterMainNode,
                                                                    particleEmitterTemplate->kParticleEmitterDestinationSizeXAttribute),
                                           E_TEMPLATE_META_TYPE_F32,
                                           &destinationSizeX);
    
    f32 destinationSizeY = node.attribute(particleEmitterTemplate->kParticleEmitterDestinationSizeYAttribute.c_str()).as_float();
    particleEmitterTemplate->Set_Attribute(Get_TemplateAttributeKey(particleEmitterTemplate->kParticleEmitterMainNode,
                                                                    particleEmitterTemplate->kParticleEmitterDestinationSizeYAttribute),
                                           E_TEMPLATE_META_TYPE_F32,
                                           &destinationSizeY);
    
    ui32 minEmittInterval = node.attribute(particleEmitterTemplate->kParticleEmitterMinEmittIntervalAttribute.c_str()).as_uint();
    particleEmitterTemplate->Set_Attribute(Get_TemplateAttributeKey(particleEmitterTemplate->kParticleEmitterMainNode,
                                                                    particleEmitterTemplate->kParticleEmitterMinEmittIntervalAttribute),
                                           E_TEMPLATE_META_TYPE_UI32,
                                           &minEmittInterval);
    
    ui32 maxEmittInterval = node.attribute(particleEmitterTemplate->kParticleEmitterMaxEmittIntervalAttribute.c_str()).as_uint();
    particleEmitterTemplate->Set_Attribute(Get_TemplateAttributeKey(particleEmitterTemplate->kParticleEmitterMainNode,
                                                                    particleEmitterTemplate->kParticleEmitterMaxEmittIntervalAttribute),
                                           E_TEMPLATE_META_TYPE_UI32,
                                           &maxEmittInterval);
    
    pugi::xml_node materialsNode = node.child(particleEmitterTemplate->kGameObjectMaterialsTemplatesNode.c_str());
    for (pugi::xml_node material = materialsNode.child(particleEmitterTemplate->kGameObjectMaterialTemplateNode.c_str());
         material;
         material = material.next_sibling(particleEmitterTemplate->kGameObjectMaterialTemplateNode.c_str()))
    {
        std::string filename = material.attribute(particleEmitterTemplate->kGameObjectMaterialFilenameAttribute.c_str()).as_string();
        particleEmitterTemplate->Set_Attribute(Get_TemplateAttributeKey(particleEmitterTemplate->kGameObjectMaterialsTemplatesNode,
                                                                        particleEmitterTemplate->kGameObjectMaterialTemplateNode,
                                                                        particleEmitterTemplate->kGameObjectMaterialFilenameAttribute),
                                               E_TEMPLATE_META_TYPE_STRING,
                                               &filename);
    }
    
    return particleEmitterTemplate;
}

std::shared_ptr<I_RO_TemplateCommon> CParticleEmitterTemplateSerializer::Serialize(const std::string& _host, ui32 _port, const std::string& _filename)
{
    return nullptr;
}

void CParticleEmitterTemplateSerializer::Deserialize(const std::string& _filename, std::shared_ptr<I_RO_TemplateCommon> _template)
{
    
}
