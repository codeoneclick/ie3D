//
//  CParticleEmitterTemplateSerializer.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/28/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CParticleEmitterTemplateSerializer.h"
#include "CCommonOS.h"
#include "ITemplate.h"

CParticleEmitterTemplateSerializer::CParticleEmitterTemplateSerializer(void)
{
    
}

CParticleEmitterTemplateSerializer::~CParticleEmitterTemplateSerializer(void)
{
    
}

std::shared_ptr<ITemplate> CParticleEmitterTemplateSerializer::Serialize(const std::string& _filename)
{
    std::string path(Get_BundlePath());
    path.append(_filename);
    pugi::xml_document document;
    pugi::xml_parse_result result = document.load_file(path.c_str());
    assert(result.status == pugi::status_ok);
    pugi::xml_node node = document.child("particle_emitter");
    std::shared_ptr<SParticleEmitterTemplate> particleEmitterTemplate = std::make_shared<SParticleEmitterTemplate>();
    
    particleEmitterTemplate->m_numParticles = node.child("num_particles").attribute("value").as_int();
    particleEmitterTemplate->m_duration = node.child("duration").attribute("value").as_uint();
    particleEmitterTemplate->m_durationRandomness = node.child("duration_randomness").attribute("value").as_uint();
    particleEmitterTemplate->m_velocitySensitivity = node.child("velocity_sensitivity").attribute("value").as_float();
    particleEmitterTemplate->m_minHorizontalVelocity = node.child("min_horizontal_velocity").attribute("value").as_float();
    particleEmitterTemplate->m_maxHorizontalVelocity = node.child("max_horizontal_velocity").attribute("value").as_float();
    particleEmitterTemplate->m_minVerticalVelocity = node.child("min_vertical_velocity").attribute("value").as_float();
    particleEmitterTemplate->m_maxVerticalVelocity = node.child("max_vertical_velocity").attribute("value").as_float();
    particleEmitterTemplate->m_endVelocity = node.child("end_velocity").attribute("value").as_float();
    particleEmitterTemplate->m_gravity.x = node.child("gravity").attribute("x").as_float();
    particleEmitterTemplate->m_gravity.y = node.child("gravity").attribute("y").as_float();
    particleEmitterTemplate->m_gravity.z = node.child("gravity").attribute("z").as_float();
    particleEmitterTemplate->m_startColor.x = node.child("start_color").attribute("r").as_float();
    particleEmitterTemplate->m_startColor.y = node.child("start_color").attribute("g").as_float();
    particleEmitterTemplate->m_startColor.z = node.child("start_color").attribute("b").as_float();
    particleEmitterTemplate->m_startColor.w = node.child("start_color").attribute("a").as_float();
    particleEmitterTemplate->m_endColor.x = node.child("end_color").attribute("r").as_float();
    particleEmitterTemplate->m_endColor.y = node.child("end_color").attribute("g").as_float();
    particleEmitterTemplate->m_endColor.z = node.child("end_color").attribute("b").as_float();
    particleEmitterTemplate->m_endColor.w = node.child("end_color").attribute("a").as_float();
    particleEmitterTemplate->m_startSize.x = node.child("start_size").attribute("width").as_float();
    particleEmitterTemplate->m_startSize.y = node.child("start_size").attribute("height").as_float();
    particleEmitterTemplate->m_endSize.x = node.child("end_size").attribute("width").as_float();
    particleEmitterTemplate->m_endSize.y = node.child("end_size").attribute("height").as_float();
    particleEmitterTemplate->m_minParticleEmittInterval = node.child("min_particle_emitt_interval").attribute("value").as_uint();
    particleEmitterTemplate->m_maxParticleEmittInterval = node.child("max_particle_emitt_interval").attribute("value").as_uint();
    
    pugi::xml_node materials_node = node.child("materials");
    for (pugi::xml_node material = materials_node.child("material"); material; material = material.next_sibling("material"))
    {
        particleEmitterTemplate->m_materialsFilenames.push_back(material.attribute("filename").as_string());
    }
    
    return particleEmitterTemplate;
}