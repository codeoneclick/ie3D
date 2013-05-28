//
//  HTemplates.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/28/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef HTemplates_h
#define HTemplates_h

#include "HCommon.h"

struct STextureTemplate
{
    std::string m_filename;
    ui32 m_slot;
    ui32 m_wrap;
};

struct SShaderTemplate
{
    std::string m_vsFilename;
    std::string m_fsFilename;
};

struct SMaterialTemplate
{
    bool m_isCullFace;
    bool m_isBlend;
    bool m_isDepthTest;
    bool m_isDepthMask;
    ui32 m_cullFaceMode;
    ui32 m_blendFunctionSource;
    ui32 m_blendFunctionDestination;
    
    std::vector<std::shared_ptr<STextureTemplate> > m_texturesTemplates;
    std::shared_ptr<SShaderTemplate> m_shaderTemplate;
    
    std::string m_renderMode;
};

struct SGameObjectTemplate
{
    std::vector<std::shared_ptr<SMaterialTemplate> > m_materialsTemplates;
};

struct SModelTemplate : public SGameObjectTemplate
{
    std::string m_meshFilename;
};

struct SParticleEmitterTemplate : public SGameObjectTemplate
{
    ui32 m_numParticles;
    
    f32 m_duration;
    f32 m_durationRandomness;
    
    f32 m_velocitySensitivity;
    
    f32 m_minHorizontalVelocity;
    f32 m_maxHorizontalVelocity;
    
    f32 m_minVerticalVelocity;
    f32 m_maxVerticalVelocity;
    
    f32 m_endVelocity;
    
    glm::vec3 m_gravity;
    
    glm::u8vec4 m_startColor;
    glm::u8vec4 m_endColor;
    
    glm::vec2 m_startSize;
    glm::vec2 m_endSize;
    
    f32 m_minParticleEmittInterval;
    f32 m_maxParticleEmittInterval;
};




#endif
