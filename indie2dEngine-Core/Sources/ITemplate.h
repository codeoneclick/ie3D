//
//  ITemplate.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/29/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef ITemplate_h
#define ITemplate_h

#include "HCommon.h"

struct ITemplate
{
    
};

struct STextureTemplate : public ITemplate
{
    std::string m_filename;
    std::string m_operationName;
    ui32 m_sampler;
    ui32 m_wrap;
};

struct SShaderTemplate : public ITemplate
{
    std::string m_vsFilename;
    std::string m_fsFilename;
};

struct SMaterialTemplate : public ITemplate
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

struct SModelTemplate : public ITemplate
{
    std::vector<std::string> m_materialsFilenames;
    std::vector<std::shared_ptr<SMaterialTemplate> > m_materialsTemplates;
    std::string m_meshFilename;
};

struct SParticleEmitterTemplate : public ITemplate
{
    std::vector<std::string> m_materialsFilenames;
    std::vector<std::shared_ptr<SMaterialTemplate> > m_materialsTemplates;
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

struct SWorldSpaceRenderOperationTemplate : public ITemplate
{
    std::string m_guid;
};

struct SScreenSpaceRenderOperationTemplate : public ITemplate
{
    std::string m_guid;
    std::string m_materialTemplateFilename;
    std::shared_ptr<SMaterialTemplate> m_materialTemplate;
};

struct SOutputRenderOperationTemplate : public ITemplate
{
    std::string m_guid;
    std::string m_materialTemplateFilename;
    std::shared_ptr<SMaterialTemplate> m_materialTemplate;
};

struct SGameTransitionTemplate : public ITemplate
{
    std::string m_guid;
    std::string m_outputRenderOperationTemplateFilename;
    std::shared_ptr<SOutputRenderOperationTemplate> m_outputRenderOperationTemplate;
    std::vector<std::string> m_worldSpaceRenderOperationsTemplatesFilenames;
    std::vector<std::shared_ptr<SWorldSpaceRenderOperationTemplate> > m_worldSpaceRenderOperationsTemplates;
    std::vector<std::string> m_screenSpaceRenderOperationsTemplatesFilenames;
    std::vector<std::shared_ptr<SScreenSpaceRenderOperationTemplate> > m_screenSpaceRenderOperationsTemplates;
};

#endif
