//
//  CTemplateGameObjects.h
//  indieEngineCore
//
//  Created by Sergey Sergeev on 12/21/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CTemplateGameObjects_h
#define CTemplateGameObjects_h

#include "CTemplateCommon.h"

class CTemplateTexture : public CTemplateCommon
{
public:
    
    const std::string kTextureMainNode;
    const std::string kTextureFilenameAttribute;
    const std::string kTextureRenderOperationNameAttribute;
    const std::string kTextureSamplerIdAttribute;
    const std::string kTextureWrapModeAttribute;
    
private:
    
protected:
    
public:
    
    CTemplateTexture(void);
    ~CTemplateTexture(void);
    
    std::string Get_RenderOperationName(void) const;
    ui32 Get_SamplerId(void) const;
    ui32 Get_WrapMode(void) const;
};

class CTemplateShader : public CTemplateCommon
{
public:
    
    const std::string kShaderMainNode;
    const std::string kShaderVSFilenameAttribute;
    const std::string kShaderFSFilenameAttribute;

private:
    
protected:
    
public:
    
    CTemplateShader(void);
    ~CTemplateShader(void);
    
    std::string Get_VSFilename(void) const;
    std::string Get_FSFilename(void) const;
};

class CTemplateMaterial : public CTemplateCommon
{
public:
    
    const std::string kMaterialMainNode;
    const std::string kMaterialRenderOperationNameAttribute;
    const std::string kMaterialIsCullFaceAttribute;
    const std::string kMaterialCullFaceModeAttribute;
    const std::string kMaterialIsBlendingAttribute;
    const std::string kMaterialBlendingFunctionSourceAttribute;
    const std::string kMaterialBlendingFunctionDestinationAttribute;
    const std::string kMaterialIsDepthTestAttribute;
    const std::string kMaterialIsDepthMaskAttribute;
    const std::string kMaterialIsClippingAttribute;
    const std::string kMaterialClippingXAttribute;
    const std::string kMaterialClippingYAttribute;
    const std::string kMaterialClippingZAttribute;
    const std::string kMaterialClippingWAttribute;
    const std::string kMaterialIsReflectingAttribute;
    const std::string kMaterialIsShadowingAttribute;
    const std::string kMaterialIsDebuggingAttribute;
    
    const std::string kMaterialTexturesTemplatesNode;
    const std::string kMaterialTextureTemplateNode;
    const std::string kMaterialShaderTemplateNode;
    
private:
    
protected:
    
public:
    
    CTemplateMaterial(void);
    ~CTemplateMaterial(void);
    
    std::string Get_RenderOperationName(void) const;
    bool Get_IsCullFace(void) const;
    ui32 Get_CullFaceMode(void) const ;
    bool Get_IsBlending(void) const;
    ui32 Get_BlendingFunctionSource(void) const;
    ui32 Get_BlendingFunctionDestination(void) const;
    bool Get_IsDepthTest(void) const;
    bool Get_IsDepthMask(void) const;
    bool Get_IsClipping(void) const;
    glm::vec4 Get_ClippingPlane(void) const;
    bool Get_IsReflecting(void) const;
    bool Get_IsShadowing(void) const;
    bool Get_IsDebugging(void) const;
    
    std::vector<std::shared_ptr<I_RO_TemplateCommon>> Get_TexturesTemplates(void) const;
    std::shared_ptr<I_RO_TemplateCommon> Get_ShaderTemplates(void) const;
};

class CGameObjectTemplate : public CTemplateCommon
{
public:
    
    const std::string kGameObjectMaterialsTemplatesNode;
    const std::string kGameObjectMaterialTemplateNode;
    const std::string kGameObjectMaterialFilenameAttribute;
    
private:
    
protected:
    
public:
    
    CGameObjectTemplate(void);
    ~CGameObjectTemplate(void);
    
    std::vector<std::string> Get_MaterialsTemplatesFilenames(void) const;
    std::vector<std::shared_ptr<I_RO_TemplateCommon>> Get_MaterialsTemplates(void) const;
};


class CModelTemplate : public CGameObjectTemplate
{
public:
    
    const std::string kModelMainNode;
    const std::string kModelMeshNode;
    const std::string kModelMeshFilenameAttribute;
    const std::string kModelMeshIsBatchingAttribute;
    const std::string kModelSkeletonNode;
    const std::string kModelSkeletonFilenameAttribute;
    const std::string kModelSequencesNode;
    const std::string kModelSequenceNode;
    const std::string kModelSequenceFilenameAttribute;
    const std::string kModelSequenceAnimationNameAttribute;
    
private:
    
protected:
    
public:
    
    CModelTemplate(void);
    ~CModelTemplate(void);
    
    std::string Get_MeshFilename(void) const;
    std::string Get_SkeletonFilename(void) const;
    std::vector<std::string> Get_SequencesFilenames(void) const;
    std::vector<std::string> Get_SequenceAnimationsNames(void) const;
    bool Get_IsBatching(void) const;
};

class  COceanTemplate : public CGameObjectTemplate
{
public:
   
    const std::string kOceanMainNode;
    const std::string kOceanSizeXAttribute;
    const std::string kOceanSizeYAttribute;
    const std::string kOceanAltitudeAttribute;
    const std::string kOceanWaveGenerationIntervalAttribute;
    
private:
    
protected:
    
public:
    
    COceanTemplate(void);
    ~COceanTemplate(void);
    
    glm::vec2 Get_Size(void) const;
    f32 Get_Altitude(void) const;
    f32 Get_WaveGenerationInterval(void) const;
};

class CParticleEmitterTemplate : public CGameObjectTemplate
{
public:

private:
    
protected:
    
    const std::string kParticleEmitterMainNode;
    const std::string kParticleEmitterNumParticlesAttribute;
    const std::string kParticleEmitterDurationAttribute;
    const std::string kParticleEmitterDurationRandomnessAttribute;
    const std::string kParticleEmitterVelocitySensitivityAttribute;
    const std::string kParticleEmitterMinHorizontalVelocityAttribute;
    const std::string kParticleEmitterMaxHorizontalVelocityAttribute;
    const std::string kParticleEmitterMinVerticalVelocityAttribute;
    const std::string kParticleEmitterMaxVerticalVelocityAttribute;
    const std::string kParticleEmitterEndVelocityAttribute;
    const std::string kParticleEmitterGravityXAttribute;
    const std::string kParticleEmitterGravityYAttribute;
    const std::string kParticleEmitterGravityZAttribute;
    const std::string kParticleEmitterSourceColorRAttribute;
    const std::string kParticleEmitterSourceColorGAttribute;
    const std::string kParticleEmitterSourceColorBAttribute;
    const std::string kParticleEmitterSourceColorAAttribute;
    const std::string kParticleEmitterDestinationColorRAttribute;
    const std::string kParticleEmitterDestinationColorGAttribute;
    const std::string kParticleEmitterDestinationColorBAttribute;
    const std::string kParticleEmitterDestinationColorAAttribute;
    const std::string kParticleEmitterSourceSizeXAttribute;
    const std::string kParticleEmitterSourceSizeYAttribute;
    const std::string kParticleEmitterDestinationSizeXAttribute;
    const std::string kParticleEmitterDestinationSizeYAttribute;
    const std::string kParticleEmitterMaxEmittIntervalAttribute;
    const std::string kParticleEmitterMinEmittIntervalAttribute;
    
    
public:
    
    CParticleEmitterTemplate(void);
    ~CParticleEmitterTemplate(void);
    
    ui32 Get_NumParticles(void) const;
    ui64 Get_Duration(void) const;
    f32 Get_DurationRandomness(void) const;
    f32 Get_VelocitySensitivity(void) const;
    f32 Get_MinHorizontalVelocity(void) const;
    f32 Get_MaxHorizontalVelocity(void) const;
    f32 Get_MinVerticalVelocity(void) const;
    f32 Get_MaxVerticalVelocity(void) const;
    f32 Get_EndVelocity(void) const;
    glm::vec3 Get_Gravity(void) const;
    glm::u8vec4 Get_SourceColor(void) const;
    glm::u8vec4 Get_DestinationColor(void) const;
    glm::vec2 Get_SourceSize(void) const;
    glm::vec2 Get_DestinationSize(void) const;
	ui64 Get_MinEmittInterval(void) const;
    ui64 Get_MaxEmittInterval(void) const;
};

class CLandscapeTemplate : public CGameObjectTemplate
{
    
public:
    
    const std::string kLandscapeMainNode;
    const std::string kLandscapeHeightmapDataFilenameAttribute;
    const std::string kLandscapeSplattingDataFilenameAttribute;
    const std::string kLandscapeSizeXAttribute;
    const std::string kLandscapeSizeYAttribute;
    const std::string kLandscapeSplattingDiffuseMaterialFilenameAttribute;
    const std::string kLandscapeSplattingNormalMaterialFilenameAttribute;
    const std::string kLandscapeIsEdgesEnabledAttribute;
    const std::string kLandscapeEdgesNode;
    const std::string kLandscapeEdgesSizeXAttribute;
    const std::string kLandscapeEdgesSizeYAttribute;
    const std::string kLandscapeEdgesMaterialsTemplatesNode;
    const std::string kLandscapeEdgeMaterialTemplateNode;
    const std::string kLandscapeEdgeMaterialTemplateFilenameAttribute;
    
private:
    
protected:
    
public:
    
    CLandscapeTemplate(void);
    ~CLandscapeTemplate(void);
    
    std::string Get_HeightmapDataFilename(void) const;
    std::string Get_SplattingDataFilename(void) const;
    glm::vec2 Get_Size(void) const;
    std::string Get_SplattingDiffuseMaterialFilename(void) const;
    std::string Get_SplattingNormalMaterialFilename(void) const;
    std::shared_ptr<I_RO_TemplateCommon> Get_SplattingDiffuseMaterialTemplate(void) const;
    std::shared_ptr<I_RO_TemplateCommon> Get_SplattingNormalMaterialTemplate(void) const;
    bool Get_IsEdgesEnabled(void) const;
    glm::vec2 Get_EdgesSize(void) const;
    std::vector<std::string> Get_EdgesMaterialsFilenames(void) const;
    std::vector<std::shared_ptr<I_RO_TemplateCommon>> Get_EdgesMaterialsTemplates(void) const;
};





#endif
