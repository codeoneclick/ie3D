//
//  CConfigurationGameObjects.h
//  indieEngineCore
//
//  Created by Sergey Sergeev on 12/21/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CConfigurationGameObjects_h
#define CConfigurationGameObjects_h

#include "IConfiguration.h"
#include "HDeclaration.h"

class CConfigurationTexture : public IConfiguration
{
public:
    
    const std::string kTextureMainNode;
    const std::string kTextureFilenameAttribute;
    const std::string kTextureRenderOperationNameAttribute;
    const std::string kTextureSamplerIndexAttribute;
    const std::string kTextureWrapModeAttribute;
    
private:
    
protected:
    
public:
    
    CConfigurationTexture(void);
    ~CConfigurationTexture(void);
    
    std::string getFilename(void) const;
    std::string getRenderOperationName(void) const;
    
    ui32 getSamplerIndex(void) const;
    
    ui32 getWrapMode(void) const;
};

class CConfigurationShader : public IConfiguration
{
public:
    
    const std::string kShaderMainNode;
    const std::string kShaderVSFilenameAttribute;
    const std::string kShaderFSFilenameAttribute;

private:
    
protected:
    
public:
    
    CConfigurationShader(void);
    ~CConfigurationShader(void);
    
    std::string getVSFilename(void) const;
    std::string getFSFilename(void) const;
};

class CConfigurationMaterial : public IConfiguration
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
    const std::string kMaterialTexturesConfigurationsNode;
    const std::string kMaterialTextureConfigurationNode;
    const std::string kMaterialShaderConfigurationNode;
    
private:
    
protected:
    
public:
    
    CConfigurationMaterial(void);
    ~CConfigurationMaterial(void);
    
    std::string getRenderOperationName(void) const;
    
    bool isCulling(void) const;
    ui32 getCullingMode(void) const;
    
    bool isBlending(void) const;
    ui32 getBlendingFunctionSource(void) const;
    ui32 getBlendingFunctionDestination(void) const;
    
    bool isDepthTest(void) const;
    bool isDepthMask(void) const;
    
    bool isClipping(void) const;
    glm::vec4 getClippingPlane(void) const;
    
    bool isReflecting(void) const;
    bool isShadowing(void) const;
    bool isDebugging(void) const;
    
    std::vector<ISharedConfiguration> getTexturesConfigurations(void) const;
    ISharedConfiguration getShaderConfiguration(void) const;
};

class CConfigurationGameObject : public IConfiguration
{
public:
    
    const std::string kGameObjectMaterialsConfigurationsNode;
    const std::string kGameObjectMaterialConfigurationNode;
    const std::string kGameObjectMaterialFilenameAttribute;
    
private:
    
protected:
    
public:
    
    CConfigurationGameObject(void);
    ~CConfigurationGameObject(void);
    
    std::vector<std::string> getMaterialsConfigurationsFilenames(void) const;
    std::vector<ISharedConfiguration> getMaterialsConfigurations(void) const;
};

class CConfigurationModel : public CConfigurationGameObject
{
public:
    
    const std::string kModelMainNode;
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
    
    CConfigurationModel(void);
    ~CConfigurationModel(void);
    
    std::string getMeshFilename(void) const;
    std::string getSkeletonFilename(void) const;
    
    std::vector<std::string> getSequencesFilenames(void) const;
    std::vector<std::string> getSequenceAnimationsNames(void) const;
    
    bool isBatching(void) const;
};

class  CConfigurationOcean : public CConfigurationGameObject
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
    
    CConfigurationOcean(void);
    ~CConfigurationOcean(void);
    
    glm::vec2 getSize(void) const;
    f32 getAltitude(void) const;
    f32 getWaveGenerationInterval(void) const;
};

class CConfigurationParticleEmitter : public CConfigurationGameObject
{
public:

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

private:
    
protected:
    
public:
    
    CConfigurationParticleEmitter(void);
    ~CConfigurationParticleEmitter(void);
    
    ui32 getNumParticles(void) const;
    
    ui64 getDuration(void) const;
    
    f32 getDurationRandomness(void) const;
    f32 getVelocitySensitivity(void) const;
    
    f32 getMinHorizontalVelocity(void) const;
    f32 getMaxHorizontalVelocity(void) const;
    
    f32 getMinVerticalVelocity(void) const;
    f32 getMaxVerticalVelocity(void) const;
    
    f32 getEndVelocity(void) const;
    
    glm::vec3 getGravity(void) const;
    
    glm::u8vec4 getSourceColor(void) const;
    glm::u8vec4 getDestinationColor(void) const;
    
    glm::vec2 getSourceSize(void) const;
    glm::vec2 getDestinationSize(void) const;
    
	ui64 getMinEmittInterval(void) const;
    ui64 getMaxEmittInterval(void) const;
};

class CConfigurationLandscape : public CConfigurationGameObject
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
    const std::string kLandscapeEdgesMaterialsConfigurationsNode;
    const std::string kLandscapeEdgeMaterialConfigurationNode;
    const std::string kLandscapeEdgeMaterialConfigurationFilenameAttribute;
    
private:
    
protected:
    
public:
    
    CConfigurationLandscape(void);
    ~CConfigurationLandscape(void);
    
    std::string getHeightmapDataFilename(void) const;
    std::string getSplattingDataFilename(void) const;
    
    glm::vec2 getSize(void) const;
    
    std::string getSplattingDiffuseMaterialFilename(void) const;
    std::string getSplattingNormalMaterialFilename(void) const;
    
    ISharedConfiguration getSplattingDiffuseMaterialConfiguration(void) const;
    ISharedConfiguration getSplattingNormalMaterialConfiguration(void) const;
    
    bool isEdgesEnabled(void) const;
    
    glm::vec2 getEdgesSize(void) const;
    
    std::vector<std::string> getEdgesMaterialsFilenames(void) const;
    
    std::vector<ISharedConfiguration> getEdgesMaterialsConfigurations(void) const;
};


struct CConfigurationWSRenderOperation : public IConfiguration
{
public:
    
    const std::string kWSRenderOperationMainNode;
    const std::string kWSRenderOperationGuidAttribute;
    const std::string kWSRenderOperationIndexAttribute;
    const std::string kWSRenderOperationScreenWidthAttribute;
    const std::string kWSRenderOperationScreenHeightAttribute;
    const std::string kWSRenderOperationClearColorRAttribute;
    const std::string kWSRenderOperationClearColorGAttribute;
    const std::string kWSRenderOperationClearColorBAttribute;
    const std::string kWSRenderOperationClearColorAAttribute;
    
private:
    
protected:
    
public:
    
    CConfigurationWSRenderOperation(void);
    ~CConfigurationWSRenderOperation(void);
    
    std::string getGuid(void) const;
    
    ui32 getIndex(void) const;
    
    ui32 getScreenWidth(void) const;
    ui32 getScreenHeight(void) const;
    
    glm::vec4 getClearColor(void) const;
};

class CConfigurationSSRenderOperation : public IConfiguration
{
public:
    
    const std::string kSSRenderOperationMainNode;
    const std::string kSSRenderOperationGuidAttribute;
    const std::string kSSRenderOperationScreenWidthAttribute;
    const std::string kSSRenderOperationScreenHeightAttribute;
    const std::string kSSRenderOperationMaterialConfigurationFilenameAttribute;
    
private:
    
protected:
    
public:
    
    CConfigurationSSRenderOperation(void);
    ~CConfigurationSSRenderOperation(void);
    
    std::string getGuid(void) const;
    
    ui32 getScreenWidth(void) const;
    ui32 getScreenHeight(void) const;
    
    std::string getMaterialConfigurationFilename(void) const;
    ISharedConfiguration getMaterialConfiguration(void) const;
};

class CConfigurationORenderOperation : public IConfiguration
{
public:
    
    const std::string kORenderOperationMainNode;
    const std::string kORenderOperationGuidAttribute;
    const std::string kORenderOperationMaterialConfigurationFilenameAttribute;
    
private:
    
protected:
    
public:
    
    CConfigurationORenderOperation(void);
    ~CConfigurationORenderOperation(void);
    
    std::string getGuid(void) const;
    std::string getMaterialConfigurationFilename(void) const;
    ISharedConfiguration getMaterialConfiguration(void) const;
};

class CConfigurationGameTransition : public IConfiguration
{
public:
    
    const std::string kGameTransitionMainNode;
    const std::string kGameTransitionGuidAttribute;
    const std::string kGameTransitionORenderOperationConfigurationNode;
    const std::string kGameTransitionORenderOperationConfigurationFilenameAttribute;
    const std::string kGameTransitionWSRenderOperationsConfigurationsNode;
    const std::string kGameTransitionWSRenderOperationConfigurationNode;
    const std::string kGameTransitionWSRenderOperationConfigurationFilenameAttribute;
    const std::string kGameTransitionSSRenderOperationsConfigurationsNode;
    const std::string kGameTransitionSSRenderOperationConfigurationNode;
    const std::string kGameTransitionSSRenderOperationConfigurationFilenameAttribute;
    
private:
    
protected:
    
public:
    
    CConfigurationGameTransition(void);
    ~CConfigurationGameTransition(void);
    
    std::string getGuid(void) const;
    
    std::string getORenderOperationConfigurationFilename(void) const;
    ISharedConfiguration getORenderOperationConfiguration(void) const;
    
    std::vector<std::string> getWSRenderOperationsConfigurationsFilenames(void) const;
    std::vector<ISharedConfiguration> getWSRenderOperationsConfigurations(void) const;
    std::vector<std::string> getSSRenderOperationsConfigurationsFilenames(void) const;
    std::vector<ISharedConfiguration> getSSRenderOperationsConfigurations(void) const;
};

#endif
