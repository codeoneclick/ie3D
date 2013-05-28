//
//  CParticleEmitterTemplateSerializer.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/28/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CParticleEmitterTemplateSerializer.h"
#include "IGameObjectTemplateSerializer.h"
#include "CCommonOS.h"

CParticleEmitterTemplateSerializer::CParticleEmitterTemplateSerializer(void)
{
    
}

CParticleEmitterTemplateSerializer::~CParticleEmitterTemplateSerializer(void)
{
    
}

std::shared_ptr<SParticleEmitterTemplate> CParticleEmitterTemplateSerializer::Deserialize(const std::string& _filename)
{
    std::string path(Get_BundlePath());
    path.append(_filename);
    pugi::xml_document document;
    pugi::xml_parse_result result = document.load_file(path.c_str());
    assert(result.status == pugi::status_ok);
    pugi::xml_node settings_node = document.child("settings");
    std::shared_ptr<SParticleEmitterTemplate> particleEmitterTemplate = std::make_shared<SParticleEmitterTemplate>();
    
    IGameObjectTemplateSerializer gameObjectTemplateSerializer;
    particleEmitterTemplate->m_materialsTemplates = gameObjectTemplateSerializer.Deserialize(settings_node);
    
    particleEmitterTemplate->m_numParticles = settings_node.child("num_particles").attribute("value").as_int();
    particleEmitterTemplate->m_duration = settings_node.child("duration").attribute("value").as_uint();
    particleEmitterTemplate->m_durationRandomness = settings_node.child("duration_randomness").attribute("value").as_uint();
    particleEmitterTemplate->m_velocitySensitivity = settings_node.child("velocity_sensitivity").attribute("value").as_float();
    particleEmitterTemplate->m_minHorizontalVelocity = settings_node.child("min_horizontal_velocity").attribute("value").as_float();
    particleEmitterTemplate->m_maxHorizontalVelocity = settings_node.child("max_horizontal_velocity").attribute("value").as_float();
    particleEmitterTemplate->m_minVerticalVelocity = settings_node.child("min_vertical_velocity").attribute("value").as_float();
    particleEmitterTemplate->m_maxVerticalVelocity = settings_node.child("max_vertical_velocity").attribute("value").as_float();
    particleEmitterTemplate->m_endVelocity = settings_node.child("end_velocity").attribute("value").as_float();
    particleEmitterTemplate->m_gravity.x = settings_node.child("gravity").attribute("x").as_float();
    particleEmitterTemplate->m_gravity.y = settings_node.child("gravity").attribute("y").as_float();
    particleEmitterTemplate->m_gravity.z = settings_node.child("gravity").attribute("z").as_float();
    particleEmitterTemplate->m_startColor.x = settings_node.child("start_color").attribute("r").as_float();
    particleEmitterTemplate->m_startColor.y = settings_node.child("start_color").attribute("g").as_float();
    particleEmitterTemplate->m_startColor.z = settings_node.child("start_color").attribute("b").as_float();
    particleEmitterTemplate->m_startColor.w = settings_node.child("start_color").attribute("a").as_float();
    particleEmitterTemplate->m_endColor.x = settings_node.child("end_color").attribute("r").as_float();
    particleEmitterTemplate->m_endColor.y = settings_node.child("end_color").attribute("g").as_float();
    particleEmitterTemplate->m_endColor.z = settings_node.child("end_color").attribute("b").as_float();
    particleEmitterTemplate->m_endColor.w = settings_node.child("end_color").attribute("a").as_float();
    particleEmitterTemplate->m_startSize.x = settings_node.child("start_size").attribute("width").as_float();
    particleEmitterTemplate->m_startSize.y = settings_node.child("start_size").attribute("height").as_float();
    particleEmitterTemplate->m_endSize.x = settings_node.child("end_size").attribute("width").as_float();
    particleEmitterTemplate->m_endSize.y = settings_node.child("end_size").attribute("height").as_float();
    particleEmitterTemplate->m_minParticleEmittInterval = settings_node.child("min_particle_emitt_interval").attribute("value").as_uint();
    particleEmitterTemplate->m_maxParticleEmittInterval = settings_node.child("max_particle_emitt_interval").attribute("value").as_uint();
    
    return particleEmitterTemplate;
}