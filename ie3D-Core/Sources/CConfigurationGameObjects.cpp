//
//  CConfigurationGameObjects.cpp
//  indieEngineCore
//
//  Created by Sergey Sergeev on 12/21/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CConfigurationGameObjects.h"

CConfigurationTexture::CConfigurationTexture(void) :
IConfiguration(E_CONFIGURATION_CLASS_TEXTURE),
kTextureMainNode("texture"),
kTextureFilenameAttribute("filename"),
kTextureRenderOperationNameAttribute("render_operation_name"),
kTextureSamplerIndexAttribute("sampler_id"),
kTextureWrapModeAttribute("wrap_mode")
{

}

CConfigurationTexture::~CConfigurationTexture(void)
{
    
}

std::string CConfigurationTexture::getFilename(void) const
{
    const auto& iterator = m_attributes.find(kTextureMainNode + ":" +
                                             kTextureFilenameAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->getString();
}

std::string CConfigurationTexture::getRenderOperationName(void) const
{
    const auto& iterator = m_attributes.find(kTextureMainNode + ":" +
                                             kTextureRenderOperationNameAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->getString();
}

ui32 CConfigurationTexture::getSamplerIndex(void) const
{
    const auto& iterator = m_attributes.find(kTextureMainNode + ":" +
                                             kTextureSamplerIndexAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->getScalar<ui32>();
}

ui32 CConfigurationTexture::getWrapMode(void) const
{
    const auto& iterator = m_attributes.find(kTextureMainNode + ":" +
                                             kTextureWrapModeAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->getScalar<ui32>();
}

CConfigurationShader::CConfigurationShader(void) :
IConfiguration(E_CONFIGURATION_CLASS_SHADER),
kShaderMainNode("shader"),
kShaderVSFilenameAttribute("vs_filename"),
kShaderFSFilenameAttribute("fs_filename")
{
    
}

CConfigurationShader::~CConfigurationShader(void)
{
    
}

std::string CConfigurationShader::getVSFilename(void) const
{
    const auto& iterator = m_attributes.find(kShaderMainNode + ":" +
                                             kShaderVSFilenameAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->getString();
}

std::string CConfigurationShader::getFSFilename(void) const
{
    const auto& iterator = m_attributes.find(kShaderMainNode + ":" +
                                             kShaderFSFilenameAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->getString();
}

CConfigurationMaterial::CConfigurationMaterial(void) :
IConfiguration(E_CONFIGURATION_CLASS_MATERIAL),
kMaterialMainNode("material"),
kMaterialRenderOperationNameAttribute("render_operation_name"),
kMaterialIsCullFaceAttribute("is_cull_face"),
kMaterialCullFaceModeAttribute("cull_face_mode"),
kMaterialIsBlendingAttribute("is_blending"),
kMaterialBlendingFunctionSourceAttribute("blending_function_source"),
kMaterialBlendingFunctionDestinationAttribute("blending_function_destination"),
kMaterialIsDepthTestAttribute("is_depth_test"),
kMaterialIsDepthMaskAttribute("is_depth_mask"),
kMaterialIsClippingAttribute("is_clipping"),
kMaterialClippingXAttribute("clipping_x"),
kMaterialClippingYAttribute("clipping_y"),
kMaterialClippingZAttribute("clipping_z"),
kMaterialClippingWAttribute("clipping_w"),
kMaterialIsReflectingAttribute("is_reflecting"),
kMaterialIsShadowingAttribute("is_shadowing"),
kMaterialIsDebuggingAttribute("is_debugging"),
kMaterialTexturesConfigurationsNode("textures"),
kMaterialTextureConfigurationNode("texture"),
kMaterialShaderConfigurationNode("shader")
{
    
}

CConfigurationMaterial::~CConfigurationMaterial(void)
{
    
}

std::string CConfigurationMaterial::getRenderOperationName(void) const
{
    const auto& iterator = m_attributes.find(kMaterialMainNode + ":" +
                                             kMaterialRenderOperationNameAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->getString();
}

bool CConfigurationMaterial::isCulling(void) const
{
    const auto& iterator = m_attributes.find(kMaterialMainNode + ":" +
                                             kMaterialIsCullFaceAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->getScalar<bool>();
}

ui32 CConfigurationMaterial::getCullingMode(void) const
{
    const auto& iterator = m_attributes.find(kMaterialMainNode + ":" +
                                             kMaterialCullFaceModeAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->getScalar<ui32>();
}

bool CConfigurationMaterial::isBlending(void) const
{
    const auto& iterator = m_attributes.find(kMaterialMainNode + ":" +
                                             kMaterialIsBlendingAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->getScalar<bool>();
}

ui32 CConfigurationMaterial::getBlendingFunctionSource(void) const
{
    const auto& iterator = m_attributes.find(kMaterialMainNode + ":" +
                                             kMaterialBlendingFunctionSourceAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->getScalar<ui32>();
}

ui32 CConfigurationMaterial::getBlendingFunctionDestination(void) const
{
    const auto& iterator = m_attributes.find(kMaterialMainNode + ":" +
                                             kMaterialBlendingFunctionDestinationAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->getScalar<ui32>();
}

bool CConfigurationMaterial::isDepthTest(void) const
{
    const auto& iterator = m_attributes.find(kMaterialMainNode + ":" +
                                             kMaterialIsDepthTestAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->getScalar<bool>();
}

bool CConfigurationMaterial::isDepthMask(void) const
{
    const auto& iterator = m_attributes.find(kMaterialMainNode + ":" +
                                             kMaterialIsDepthMaskAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->getScalar<bool>();
}

bool CConfigurationMaterial::isClipping(void) const
{
    const auto& iterator = m_attributes.find(kMaterialMainNode + ":" +
                                             kMaterialIsClippingAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->getScalar<bool>();
}

glm::vec4 CConfigurationMaterial::getClippingPlane(void) const
{
    const auto& iteratorX = m_attributes.find(kMaterialMainNode + ":" +
                                              kMaterialClippingXAttribute);
    assert(iteratorX != m_attributes.end());
    assert(iteratorX->second.size() != 0);
    f32 x = iteratorX->second[0]->getScalar<f32>();
    
    const auto& iteratorY = m_attributes.find(kMaterialMainNode + ":" +
                                              kMaterialClippingYAttribute);
    assert(iteratorY != m_attributes.end());
    assert(iteratorY->second.size() != 0);
    f32 y = iteratorY->second[0]->getScalar<f32>();
    
    const auto& iteratorZ = m_attributes.find(kMaterialMainNode + ":" +
                                              kMaterialClippingZAttribute);
    assert(iteratorZ != m_attributes.end());
    assert(iteratorZ->second.size() != 0);
    f32 z = iteratorZ->second[0]->getScalar<f32>();
    
    const auto& iteratorW = m_attributes.find(kMaterialMainNode+ ":" +
                                              kMaterialClippingWAttribute);
    assert(iteratorW != m_attributes.end());
    assert(iteratorW->second.size() != 0);
    f32 w = iteratorW->second[0]->getScalar<f32>();
    return glm::vec4(x, y, z, w);
}

bool CConfigurationMaterial::isReflecting(void) const
{
    const auto& iterator = m_attributes.find(kMaterialMainNode + ":" +
                                             kMaterialIsReflectingAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->getScalar<bool>();
}

bool CConfigurationMaterial::isShadowing(void) const
{
    const auto& iterator = m_attributes.find(kMaterialMainNode + ":" +
                                             kMaterialIsShadowingAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->getScalar<bool>();
}

bool CConfigurationMaterial::isDebugging(void) const
{
    const auto& iterator = m_attributes.find(kMaterialMainNode + ":" +
                                             kMaterialIsDebuggingAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->getScalar<bool>();
}

std::vector<ISharedConfiguration> CConfigurationMaterial::getTexturesConfigurations(void) const
{
    const auto& iterator = m_configurations.find(kMaterialMainNode + ":" +
                                                 kMaterialTexturesConfigurationsNode);
    if(iterator != m_configurations.end())
    {
        return iterator->second;
    }
    static std::vector<ISharedConfiguration> configurations;
    return configurations;
}

ISharedConfiguration CConfigurationMaterial::getShaderConfiguration(void) const
{
    const auto& iterator = m_configurations.find(kMaterialMainNode + ":" +
                                                 kMaterialShaderConfigurationNode);
    assert(iterator != m_configurations.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0];
}

CConfigurationGameObject::CConfigurationGameObject(E_CONFIGURATION_CLASS configurationClass) :
IConfiguration(configurationClass),
kGameObjectMaterialsConfigurationsNode("materials"),
kGameObjectMaterialConfigurationNode("material"),
kGameObjectMaterialFilenameAttribute("filename")
{
    
}

CConfigurationGameObject::~CConfigurationGameObject(void)
{
    
}

std::vector<std::string> CConfigurationGameObject::getMaterialsConfigurationsFilenames(void) const
{
    const auto& iterator = m_attributes.find(kGameObjectMaterialsConfigurationsNode + ":" +
                                             kGameObjectMaterialConfigurationNode + ":" +
                                             kGameObjectMaterialFilenameAttribute);
    assert(iterator != m_attributes.end());
    std::vector<std::string> filenames;
    for(const auto& filename : iterator->second)
    {
        filenames.push_back(filename->getString());
    }
    return filenames;
}

std::vector<ISharedConfiguration> CConfigurationGameObject::getMaterialsConfigurations(void) const
{
    const auto& iterator = m_configurations.find(kGameObjectMaterialsConfigurationsNode + ":" +
                                                 kGameObjectMaterialConfigurationNode + ":" +
                                                 kGameObjectMaterialFilenameAttribute);
    assert(iterator != m_configurations.end());
    return iterator->second;
}

CConfigurationModel::CConfigurationModel(void) :
CConfigurationGameObject(E_CONFIGURATION_CLASS_MODEL),
kModelMainNode("model"),
kModelMeshFilenameAttribute("filename"),
kModelMeshIsBatchingAttribute("is_batching")
{
    
}

CConfigurationModel::~CConfigurationModel(void)
{
    
}

std::string CConfigurationModel::getFilename(void) const
{
    const auto& iterator = m_attributes.find(kModelMainNode + ":" +
                                             kModelMeshFilenameAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->getString();
}

bool CConfigurationModel::isBatching(void) const
{
    const auto& iterator = m_attributes.find(kModelMainNode + ":" +
                                             kModelMeshIsBatchingAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->getScalar<bool>();
}

CConfigurationSkyBox::CConfigurationSkyBox(void) :
CConfigurationGameObject(E_CONFIGURATION_CLASS_SKYBOX),
kSkyBoxMainNode("skybox"),
kSkyBoxXPositiveTextureFilenameAttribute("x_positive"),
kSkyBoxXNegativeTextureFilenameAttribute("x_negative"),
kSkyBoxYPositiveTextureFilenameAttribute("y_positive"),
kSkyBoxYNegativeTextureFilenameAttribute("y_negative"),
kSkyBoxZPositiveTextureFilenameAttribute("z_positive"),
kSkyBoxZNegativeTextureFilenameAttribute("z_negative")
{
    
}

CConfigurationSkyBox::~CConfigurationSkyBox(void)
{
    
}

std::string CConfigurationSkyBox::getXPositiveTextureFilename(void) const
{
    const auto& iterator = m_attributes.find(kSkyBoxMainNode + ":" +
                                             kSkyBoxXPositiveTextureFilenameAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->getString();
}

std::string CConfigurationSkyBox::getXNegativeTextureFilename(void) const
{
    const auto& iterator = m_attributes.find(kSkyBoxMainNode + ":" +
                                             kSkyBoxXNegativeTextureFilenameAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->getString();
}

std::string CConfigurationSkyBox::getYPositiveTextureFilename(void) const
{
    const auto& iterator = m_attributes.find(kSkyBoxMainNode + ":" +
                                             kSkyBoxYPositiveTextureFilenameAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->getString();
}

std::string CConfigurationSkyBox::getYNegativeTextureFilename(void) const
{
    const auto& iterator = m_attributes.find(kSkyBoxMainNode + ":" +
                                             kSkyBoxYNegativeTextureFilenameAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->getString();
}

std::string CConfigurationSkyBox::getZPositiveTextureFilename(void) const
{
    const auto& iterator = m_attributes.find(kSkyBoxMainNode + ":" +
                                             kSkyBoxZPositiveTextureFilenameAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->getString();
}

std::string CConfigurationSkyBox::getZNegativeTextureFilename(void) const
{
    const auto& iterator = m_attributes.find(kSkyBoxMainNode + ":" +
                                             kSkyBoxZNegativeTextureFilenameAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->getString();
}

CConfigurationAtmosphericScattering::CConfigurationAtmosphericScattering(void) :
CConfigurationGameObject(E_CONFIGURATION_CLASS_ATMOSPHERIC_SCATTERING),
kAtmosphericScatteringMainNode("atmospheric_scattering"),
kAtmosphericScatteringNumColsAttribute("num_cols"),
kAtmosphericScatteringNumRowsAttribute("num_rows"),
kAtmosphericScatteringSizeAttribute("size"),
kAtmosphericScatteringNumSamplesAttribute("num_samples"),
kAtmosphericScatteringInnerRadiusAttribute("inner_radius"),
kAtmosphericScatteringOuterRadiusAttribute("outer_radius"),
kAtmosphericScatteringKrCoefficientAttribute("kr_coefficient"),
kAtmosphericScatteringKmCoefficientAttribute("km_coefficient"),
kAtmosphericScatteringESunCoefficientAttribute("esun_coefficient"),
kAtmosphericScatteringGCoefficientAttribute("g_coefficient"),
kAtmosphericScatteringRayleighScaleDepthCoefficientAttribute("rayleigh_scale_depth_coefficient"),
kAtmosphericScatteringWaveLengthXAttribute("wave_length_x"),
kAtmosphericScatteringWaveLengthYAttribute("wave_length_y"),
kAtmosphericScatteringWaveLengthZAttribute("wave_length_z")
{
    
}

CConfigurationAtmosphericScattering::~CConfigurationAtmosphericScattering(void)
{
    
}

i32 CConfigurationAtmosphericScattering::getNumCols(void) const
{
    const auto& iterator = m_attributes.find(kAtmosphericScatteringMainNode + ":" +
                                             kAtmosphericScatteringNumColsAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->getScalar<i32>();
}

i32 CConfigurationAtmosphericScattering::getNumRows(void) const
{
    const auto& iterator = m_attributes.find(kAtmosphericScatteringMainNode + ":" +
                                             kAtmosphericScatteringNumRowsAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->getScalar<i32>();
}

i32 CConfigurationAtmosphericScattering::getSize(void) const
{
    const auto& iterator = m_attributes.find(kAtmosphericScatteringMainNode + ":" +
                                             kAtmosphericScatteringSizeAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->getScalar<i32>();
}

i32 CConfigurationAtmosphericScattering::getNumSamples(void) const
{
    const auto& iterator = m_attributes.find(kAtmosphericScatteringMainNode + ":" +
                                             kAtmosphericScatteringNumSamplesAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->getScalar<i32>();
}

f32 CConfigurationAtmosphericScattering::getInnerRadius(void) const
{
    const auto& iterator = m_attributes.find(kAtmosphericScatteringMainNode + ":" +
                                             kAtmosphericScatteringInnerRadiusAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->getScalar<f32>();
}

f32 CConfigurationAtmosphericScattering::getOuterRadius(void) const
{
    const auto& iterator = m_attributes.find(kAtmosphericScatteringMainNode + ":" +
                                             kAtmosphericScatteringOuterRadiusAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->getScalar<f32>();
}

f32 CConfigurationAtmosphericScattering::getKrCoefficient(void) const
{
    const auto& iterator = m_attributes.find(kAtmosphericScatteringMainNode + ":" +
                                             kAtmosphericScatteringKrCoefficientAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->getScalar<f32>();
}

f32 CConfigurationAtmosphericScattering::getKmCoefficient(void) const
{
    const auto& iterator = m_attributes.find(kAtmosphericScatteringMainNode + ":" +
                                             kAtmosphericScatteringKmCoefficientAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->getScalar<f32>();
}

f32 CConfigurationAtmosphericScattering::getESunCoefficient(void) const
{
    const auto& iterator = m_attributes.find(kAtmosphericScatteringMainNode + ":" +
                                             kAtmosphericScatteringESunCoefficientAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->getScalar<f32>();
}

f32 CConfigurationAtmosphericScattering::getGCoefficient(void) const
{
    const auto& iterator = m_attributes.find(kAtmosphericScatteringMainNode + ":" +
                                             kAtmosphericScatteringGCoefficientAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->getScalar<f32>();
}

f32 CConfigurationAtmosphericScattering::getRayleighScaleDepthCoefficient(void) const
{
    const auto& iterator = m_attributes.find(kAtmosphericScatteringMainNode + ":" +
                                             kAtmosphericScatteringRayleighScaleDepthCoefficientAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->getScalar<f32>();
}

glm::vec3 CConfigurationAtmosphericScattering::getWaveLength(void) const
{
    const auto& iteratorX = m_attributes.find(kAtmosphericScatteringMainNode + ":" +
                                              kAtmosphericScatteringWaveLengthXAttribute);
    assert(iteratorX != m_attributes.end());
    assert(iteratorX->second.size() != 0);
    f32 x = iteratorX->second[0]->getScalar<f32>();
    
    const auto& iteratorY = m_attributes.find(kAtmosphericScatteringMainNode + ":" +
                                              kAtmosphericScatteringWaveLengthYAttribute);
    assert(iteratorY != m_attributes.end());
    assert(iteratorY->second.size() != 0);
    ui8 y = iteratorY->second[0]->getScalar<f32>();
    
    const auto& iteratorZ = m_attributes.find(kAtmosphericScatteringMainNode + ":" +
                                              kAtmosphericScatteringWaveLengthZAttribute);
    assert(iteratorZ != m_attributes.end());
    assert(iteratorZ->second.size() != 0);
    ui8 z = iteratorZ->second[0]->getScalar<f32>();
    
    return glm::vec3(x, y, z);
}

CConfigurationOcean::CConfigurationOcean(void) :
CConfigurationGameObject(E_CONFIGURATION_CLASS_OCEAN),
kOceanMainNode("ocean"),
kOceanSizeXAttribute("size_x"),
kOceanSizeYAttribute("size_y"),
kOceanAltitudeAttribute("altitude"),
kOceanWaveGenerationIntervalAttribute("wave_generation_interval")
{
    
}

CConfigurationOcean::~CConfigurationOcean(void)
{
    
}

glm::vec2 CConfigurationOcean::getSize(void) const
{
    const auto& iteratorX = m_attributes.find(kOceanMainNode + ":" +
                                              kOceanSizeXAttribute);
    assert(iteratorX != m_attributes.end());
    assert(iteratorX->second.size() != 0);
    f32 x = iteratorX->second[0]->getScalar<f32>();
    
    const auto& iteratorY = m_attributes.find(kOceanMainNode + ":" +
                                              kOceanSizeYAttribute);
    assert(iteratorY != m_attributes.end());
    assert(iteratorY->second.size() != 0);
    f32 y = iteratorY->second[0]->getScalar<f32>();
    
    return glm::vec2(x, y);
}

f32 CConfigurationOcean::getAltitude(void) const
{
    const auto& iterator = m_attributes.find(kOceanMainNode + ":" +
                                             kOceanAltitudeAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->getScalar<f32>();
}

f32 CConfigurationOcean::getWaveGenerationInterval(void) const
{
    const auto& iterator = m_attributes.find(kOceanMainNode + ":" +
                                             kOceanWaveGenerationIntervalAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->getScalar<f32>();
}

CConfigurationParticleEmitter::CConfigurationParticleEmitter(void) :
CConfigurationGameObject(E_CONFIGURATION_CLASS_PARTICLE_EMITTER),
kParticleEmitterMainNode("particle_emitter"),
kParticleEmitterNumParticlesAttribute("num_particles"),
kParticleEmitterDurationAttribute("duration"),
kParticleEmitterDurationRandomnessAttribute("duration_randomness"),
kParticleEmitterVelocitySensitivityAttribute("velocity_sensitivity"),
kParticleEmitterMinHorizontalVelocityAttribute("min_horizontal_velocity"),
kParticleEmitterMaxHorizontalVelocityAttribute("max_horizontal_velocity"),
kParticleEmitterMinVerticalVelocityAttribute("min_vertical_velocity"),
kParticleEmitterMaxVerticalVelocityAttribute("max_vertical_velocity"),
kParticleEmitterEndVelocityAttribute("end_velocity"),
kParticleEmitterGravityXAttribute("gravity_x"),
kParticleEmitterGravityYAttribute("gravity_y"),
kParticleEmitterGravityZAttribute("gravity_z"),
kParticleEmitterSourceColorRAttribute("source_color_r"),
kParticleEmitterSourceColorGAttribute("source_color_g"),
kParticleEmitterSourceColorBAttribute("source_color_b"),
kParticleEmitterSourceColorAAttribute("source_color_a"),
kParticleEmitterDestinationColorRAttribute("destination_color_r"),
kParticleEmitterDestinationColorGAttribute("destination_color_g"),
kParticleEmitterDestinationColorBAttribute("destination_color_b"),
kParticleEmitterDestinationColorAAttribute("destination_color_a"),
kParticleEmitterSourceSizeXAttribute("source_size_x"),
kParticleEmitterSourceSizeYAttribute("source_size_y"),
kParticleEmitterDestinationSizeXAttribute("destination_size_x"),
kParticleEmitterDestinationSizeYAttribute("destination_size_y"),
kParticleEmitterMaxEmittIntervalAttribute("max_emitt_interval"),
kParticleEmitterMinEmittIntervalAttribute("min_emitt_interval")

{
    
}

CConfigurationParticleEmitter::~CConfigurationParticleEmitter(void)
{
    
}

ui32 CConfigurationParticleEmitter::getNumParticles(void) const
{
    const auto& iterator = m_attributes.find(kParticleEmitterMainNode + ":" +
                                             kParticleEmitterNumParticlesAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->getScalar<ui32>();
}

ui64 CConfigurationParticleEmitter::getDuration(void) const
{
    const auto& iterator = m_attributes.find(kParticleEmitterMainNode + ":" +
                                             kParticleEmitterDurationAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->getScalar<ui32>();
}

f32 CConfigurationParticleEmitter::getDurationRandomness(void) const
{
    const auto& iterator = m_attributes.find(kParticleEmitterMainNode + ":" +
                                             kParticleEmitterDurationAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->getScalar<f32>();
}

f32 CConfigurationParticleEmitter::getVelocitySensitivity(void) const
{
    const auto& iterator = m_attributes.find(kParticleEmitterMainNode + ":" +
                                             kParticleEmitterVelocitySensitivityAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->getScalar<f32>();

}

f32 CConfigurationParticleEmitter::getMinHorizontalVelocity(void) const
{
    const auto& iterator = m_attributes.find(kParticleEmitterMainNode + ":" +
                                             kParticleEmitterMinHorizontalVelocityAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->getScalar<f32>();
}

f32 CConfigurationParticleEmitter::getMaxHorizontalVelocity(void) const
{
    const auto& iterator = m_attributes.find(kParticleEmitterMainNode + ":" +
                                             kParticleEmitterMaxHorizontalVelocityAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->getScalar<f32>();
}

f32 CConfigurationParticleEmitter::getMinVerticalVelocity(void) const
{
    const auto& iterator = m_attributes.find(kParticleEmitterMainNode + ":" +
                                             kParticleEmitterMinVerticalVelocityAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->getScalar<f32>();
}

f32 CConfigurationParticleEmitter::getMaxVerticalVelocity(void) const
{
    const auto& iterator = m_attributes.find(kParticleEmitterMainNode + ":" +
                                           kParticleEmitterMaxVerticalVelocityAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->getScalar<f32>();
    
}

f32 CConfigurationParticleEmitter::getEndVelocity(void) const
{
    const auto& iterator = m_attributes.find(kParticleEmitterMainNode + ":" +
                                             kParticleEmitterEndVelocityAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->getScalar<f32>();
}

glm::vec3 CConfigurationParticleEmitter::getGravity(void) const
{
    const auto& iteratorX = m_attributes.find(kParticleEmitterMainNode + ":" +
                                             kParticleEmitterGravityXAttribute);
    assert(iteratorX != m_attributes.end());
    assert(iteratorX->second.size() != 0);
    f32 x = iteratorX->second[0]->getScalar<f32>();
    
    const auto& iteratorY = m_attributes.find(kParticleEmitterMainNode + ":" +
                                              kParticleEmitterGravityYAttribute);
    assert(iteratorY != m_attributes.end());
    assert(iteratorY->second.size() != 0);
    ui8 y = iteratorY->second[0]->getScalar<f32>();
    
    const auto& iteratorZ = m_attributes.find(kParticleEmitterMainNode + ":" +
                                              kParticleEmitterGravityZAttribute);
    assert(iteratorZ != m_attributes.end());
    assert(iteratorZ->second.size() != 0);
    ui8 z = iteratorZ->second[0]->getScalar<f32>();
    
    return glm::vec3(x, y, z);
}

glm::u8vec4 CConfigurationParticleEmitter::getSourceColor(void) const
{
    const auto& iteratorR = m_attributes.find(kParticleEmitterMainNode + ":" +
                                              kParticleEmitterSourceColorRAttribute);
    assert(iteratorR != m_attributes.end());
    assert(iteratorR->second.size() != 0);
    ui8 r = iteratorR->second[0]->getScalar<ui8>();
    
    const auto& iteratorG = m_attributes.find(kParticleEmitterMainNode + ":" +
                                              kParticleEmitterSourceColorGAttribute);
    assert(iteratorG != m_attributes.end());
    assert(iteratorG->second.size() != 0);
    ui8 g = iteratorG->second[0]->getScalar<ui8>();
    
    const auto& iteratorB = m_attributes.find(kParticleEmitterMainNode + ":" +
                                              kParticleEmitterSourceColorBAttribute);
    assert(iteratorB != m_attributes.end());
    assert(iteratorB->second.size() != 0);
    ui8 b = iteratorB->second[0]->getScalar<ui8>();
    
    const auto& iteratorA = m_attributes.find(kParticleEmitterMainNode + ":" +
                                              kParticleEmitterSourceColorAAttribute);
    assert(iteratorA != m_attributes.end());
    assert(iteratorA->second.size() != 0);
    ui8 a = iteratorA->second[0]->getScalar<ui8>();
    return glm::u8vec4(r, g, b, a);
}

glm::u8vec4 CConfigurationParticleEmitter::getDestinationColor(void) const
{
    const auto& iteratorR = m_attributes.find(kParticleEmitterMainNode + ":" +
                                              kParticleEmitterDestinationColorRAttribute);
    assert(iteratorR != m_attributes.end());
    assert(iteratorR->second.size() != 0);
    ui8 r = iteratorR->second[0]->getScalar<ui8>();
    
    const auto& iteratorG = m_attributes.find(kParticleEmitterMainNode + ":" +
                                              kParticleEmitterDestinationColorGAttribute);
    assert(iteratorG != m_attributes.end());
    assert(iteratorG->second.size() != 0);
    ui8 g = iteratorG->second[0]->getScalar<ui8>();
    
    const auto& iteratorB = m_attributes.find(kParticleEmitterMainNode + ":" +
                                              kParticleEmitterDestinationColorBAttribute);
    assert(iteratorB != m_attributes.end());
    assert(iteratorB->second.size() != 0);
    ui8 b = iteratorB->second[0]->getScalar<ui8>();
    
    const auto& iteratorA = m_attributes.find(kParticleEmitterMainNode + ":" +
                                              kParticleEmitterDestinationColorAAttribute);
    assert(iteratorA != m_attributes.end());
    assert(iteratorA->second.size() != 0);
    ui8 a = iteratorA->second[0]->getScalar<ui8>();
    
    return glm::u8vec4(r, g, b, a);
}

glm::vec2 CConfigurationParticleEmitter::getSourceSize(void) const
{
    const auto& iteratorX = m_attributes.find(kParticleEmitterMainNode + ":" +
                                              kParticleEmitterSourceSizeXAttribute);
    assert(iteratorX != m_attributes.end());
    assert(iteratorX->second.size() != 0);
    f32 x = iteratorX->second[0]->getScalar<f32>();
    
    const auto& iteratorY = m_attributes.find(kParticleEmitterMainNode + ":" +
                                              kParticleEmitterSourceSizeYAttribute);
    assert(iteratorY != m_attributes.end());
    assert(iteratorY->second.size() != 0);
    ui8 y = iteratorY->second[0]->getScalar<f32>();
    
    return glm::vec2(x, y);
}

glm::vec2 CConfigurationParticleEmitter::getDestinationSize(void) const
{
    const auto& iteratorX = m_attributes.find(kParticleEmitterMainNode + ":" +
                                              kParticleEmitterDestinationSizeXAttribute);
    assert(iteratorX != m_attributes.end());
    assert(iteratorX->second.size() != 0);
    f32 x = iteratorX->second[0]->getScalar<f32>();
    
    const auto& iteratorY = m_attributes.find(kParticleEmitterMainNode + ":" +
                                              kParticleEmitterDestinationSizeXAttribute);
    assert(iteratorY != m_attributes.end());
    assert(iteratorY->second.size() != 0);
    ui8 y = iteratorY->second[0]->getScalar<f32>();
    
    return glm::vec2(x, y);
}

ui64 CConfigurationParticleEmitter::getMinEmittInterval(void) const
{
    const auto& iterator = m_attributes.find(kParticleEmitterMainNode + ":" +
                                              kParticleEmitterMaxEmittIntervalAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->getScalar<ui32>();
}

ui64 CConfigurationParticleEmitter::getMaxEmittInterval(void) const
{
    const auto& iterator = m_attributes.find(kParticleEmitterMainNode + ":" +
                                             kParticleEmitterMinEmittIntervalAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->getScalar<ui32>();
}

CConfigurationLandscape::CConfigurationLandscape(void) :
CConfigurationGameObject(E_CONFIGURATION_CLASS_LANDSCAPE),
kLandscapeMainNode("landscape"),
kLandscapeHeightmapDataFilenameAttribute("heightmap_data_filename"),
kLandscapeSplattingDataFilenameAttribute("splatting_data_filename"),
kLandscapeSizeXAttribute("size_x"),
kLandscapeSizeYAttribute("size_y"),
kLandscapeSplattingDiffuseMaterialFilenameAttribute("splatting_diffuse_material_filename"),
kLandscapeSplattingNormalMaterialFilenameAttribute("splatting_normal_material_filename"),
kLandscapeIsEdgesEnabledAttribute("is_edges_enabled"),
kLandscapeEdgesNode("edges"),
kLandscapeEdgesSizeXAttribute("size_x"),
kLandscapeEdgesSizeYAttribute("size_y"),
kLandscapeEdgesMaterialsConfigurationsNode("edges_materials"),
kLandscapeEdgeMaterialConfigurationNode("material"),
kLandscapeEdgeMaterialConfigurationFilenameAttribute("filename")
{
    
}

CConfigurationLandscape::~CConfigurationLandscape(void)
{
    
}

std::string CConfigurationLandscape::getHeightmapDataFilename(void) const
{
    const auto& iterator = m_attributes.find(kLandscapeMainNode + ":" +
                                             kLandscapeHeightmapDataFilenameAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->getString();
}

std::string CConfigurationLandscape::getSplattingDataFilename(void) const
{
    const auto& iterator = m_attributes.find(kLandscapeMainNode + ":" +
                                             kLandscapeSplattingDataFilenameAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->getString();
}

glm::vec2 CConfigurationLandscape::getSize(void) const
{
    const auto& iteratorX = m_attributes.find(kLandscapeMainNode + ":" +
                                              kLandscapeSizeXAttribute);
    assert(iteratorX != m_attributes.end());
    assert(iteratorX->second.size() != 0);
    f32 x = iteratorX->second[0]->getScalar<f32>();
    
    const auto& iteratorY = m_attributes.find(kLandscapeMainNode + ":" +
                                              kLandscapeSizeYAttribute);
    assert(iteratorY != m_attributes.end());
    assert(iteratorY->second.size() != 0);
    f32 y = iteratorY->second[0]->getScalar<f32>();
    
    return glm::vec2(x, y);
}

std::string CConfigurationLandscape::getSplattingDiffuseMaterialFilename(void) const
{
    const auto& iterator = m_attributes.find(kLandscapeMainNode + ":" +
                                             kLandscapeSplattingDiffuseMaterialFilenameAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->getString();
}

std::string CConfigurationLandscape::getSplattingNormalMaterialFilename(void) const
{
    const auto& iterator = m_attributes.find(kLandscapeMainNode + ":" +
                                             kLandscapeSplattingNormalMaterialFilenameAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->getString();
}

ISharedConfiguration CConfigurationLandscape::getSplattingDiffuseMaterialConfiguration(void) const
{
    const auto& iterator = m_configurations.find(kLandscapeMainNode + ":" +
                                                 kLandscapeSplattingDiffuseMaterialFilenameAttribute);
    assert(iterator != m_configurations.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0];
}

ISharedConfiguration CConfigurationLandscape::getSplattingNormalMaterialConfiguration(void) const
{
    const auto& iterator = m_configurations.find(kLandscapeMainNode + ":" +
                                                 kLandscapeSplattingNormalMaterialFilenameAttribute);
    assert(iterator != m_configurations.end());
    assert(iterator->second.size() != 0);
    return std::static_pointer_cast<CConfigurationMaterial>(iterator->second[0]);
}

bool CConfigurationLandscape::isEdgesEnabled(void) const
{
    const auto& iterator = m_attributes.find(kLandscapeMainNode + ":" +
                                             kLandscapeIsEdgesEnabledAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->getScalar<bool>();
}

glm::vec2 CConfigurationLandscape::getEdgesSize(void) const
{
    const auto& iteratorX = m_attributes.find(kLandscapeMainNode + ":" +
                                              kLandscapeEdgesNode + ":" +
                                              kLandscapeEdgesSizeXAttribute);
    assert(iteratorX != m_attributes.end());
    assert(iteratorX->second.size() != 0);
    f32 x = iteratorX->second[0]->getScalar<f32>();
    
    const auto& iteratorY = m_attributes.find(kLandscapeMainNode + ":" +
                                              kLandscapeEdgesNode + ":" +
                                              kLandscapeEdgesSizeYAttribute);
    assert(iteratorY != m_attributes.end());
    assert(iteratorY->second.size() != 0);
    ui8 y = iteratorY->second[0]->getScalar<f32>();
    
    return glm::vec2(x, y);
}

std::vector<std::string> CConfigurationLandscape::getEdgesMaterialsFilenames(void) const
{
    const auto& iterator = m_attributes.find(kLandscapeMainNode + ":" +
                                             kLandscapeEdgesNode + ":" +
                                             kLandscapeEdgesMaterialsConfigurationsNode + ":" +
                                             kLandscapeEdgeMaterialConfigurationNode + ":" +
                                             kLandscapeEdgeMaterialConfigurationFilenameAttribute);
    assert(iterator != m_attributes.end());
    std::vector<std::string> filenames;
    for(const auto& filename : iterator->second)
    {
        filenames.push_back(filename->getString());
    }
    return filenames;
}

std::vector<ISharedConfiguration> CConfigurationLandscape::getEdgesMaterialsConfigurations(void) const
{
    const auto& iterator = m_configurations.find(kLandscapeMainNode + ":" +
                                                 kLandscapeEdgesNode + ":" +
                                                 kLandscapeEdgesMaterialsConfigurationsNode + ":" +
                                                 kLandscapeEdgeMaterialConfigurationNode + ":" +
                                                 kLandscapeEdgeMaterialConfigurationFilenameAttribute);
    assert(iterator != m_configurations.end());
    return iterator->second;
}

CConfigurationWSRenderOperation::CConfigurationWSRenderOperation(void) :
IConfiguration(E_CONFIGURATION_CLASS_WS_RENDEROPERATION),
kWSRenderOperationMainNode("operation"),
kWSRenderOperationGuidAttribute("guid"),
kWSRenderOperationIndexAttribute("index"),
kWSRenderOperationScreenWidthAttribute("width"),
kWSRenderOperationScreenHeightAttribute("height"),
kWSRenderOperationClearColorRAttribute("clear_color_r"),
kWSRenderOperationClearColorGAttribute("clear_color_g"),
kWSRenderOperationClearColorBAttribute("clear_color_b"),
kWSRenderOperationClearColorAAttribute("clear_color_a")
{
    
}

CConfigurationWSRenderOperation::~CConfigurationWSRenderOperation(void)
{
    
}

std::string CConfigurationWSRenderOperation::getGuid(void) const
{
    const auto& iterator = m_attributes.find(kWSRenderOperationMainNode + ":" +
                                             kWSRenderOperationGuidAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->getString();
}

ui32 CConfigurationWSRenderOperation::getIndex(void) const
{
    const auto& iterator = m_attributes.find(kWSRenderOperationMainNode + ":" +
                                             kWSRenderOperationIndexAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->getScalar<ui32>();
}

ui32 CConfigurationWSRenderOperation::getScreenWidth(void) const
{
    const auto& iterator = m_attributes.find(kWSRenderOperationMainNode + ":" +
                                             kWSRenderOperationScreenWidthAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->getScalar<ui32>();
}

ui32 CConfigurationWSRenderOperation::getScreenHeight(void) const
{
    const auto& iterator = m_attributes.find(kWSRenderOperationMainNode + ":" +
                                             kWSRenderOperationScreenHeightAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->getScalar<ui32>();
}

glm::vec4 CConfigurationWSRenderOperation::getClearColor(void) const
{
    const auto& iteratorR = m_attributes.find(kWSRenderOperationMainNode + ":" +
                                              kWSRenderOperationClearColorRAttribute);
    assert(iteratorR != m_attributes.end());
    assert(iteratorR->second.size() != 0);
    f32 r = iteratorR->second[0]->getScalar<f32>();
    
    const auto& iteratorG = m_attributes.find(kWSRenderOperationMainNode + ":" +
                                              kWSRenderOperationClearColorGAttribute);
    assert(iteratorG != m_attributes.end());
    assert(iteratorG->second.size() != 0);
    f32 g = iteratorG->second[0]->getScalar<f32>();
    
    const auto& iteratorB = m_attributes.find(kWSRenderOperationMainNode + ":" +
                                              kWSRenderOperationClearColorBAttribute);
    assert(iteratorB != m_attributes.end());
    assert(iteratorB->second.size() != 0);
    f32 b = iteratorB->second[0]->getScalar<f32>();
    
    const auto& iteratorA = m_attributes.find(kWSRenderOperationMainNode + ":" +
                                              kWSRenderOperationClearColorAAttribute);
    assert(iteratorA != m_attributes.end());
    assert(iteratorA->second.size() != 0);
    f32 a = iteratorA->second[0]->getScalar<f32>();
    
    return glm::vec4(r, g, b, a);
}

CConfigurationSSRenderOperation::CConfigurationSSRenderOperation(void) :
IConfiguration(E_CONFIGURATION_CLASS_SS_RENDEROPERATION),
kSSRenderOperationMainNode("operation"),
kSSRenderOperationGuidAttribute("guid"),
kSSRenderOperationScreenWidthAttribute("width"),
kSSRenderOperationScreenHeightAttribute("height"),
kSSRenderOperationMaterialConfigurationFilenameAttribute("material_filename")
{
    
}

CConfigurationSSRenderOperation::~CConfigurationSSRenderOperation(void)
{
    
}

std::string CConfigurationSSRenderOperation::getGuid(void) const
{
    const auto& iterator = m_attributes.find(kSSRenderOperationMainNode + ":" +
                                             kSSRenderOperationGuidAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->getString();
}

ui32 CConfigurationSSRenderOperation::getScreenWidth(void) const
{
    const auto& iterator = m_attributes.find(kSSRenderOperationMainNode + ":" +
                                             kSSRenderOperationScreenWidthAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->getScalar<ui32>();
}

ui32 CConfigurationSSRenderOperation::getScreenHeight(void) const
{
    const auto& iterator = m_attributes.find(kSSRenderOperationMainNode + ":" +
                                             kSSRenderOperationScreenHeightAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->getScalar<ui32>();
}

std::string CConfigurationSSRenderOperation::getMaterialConfigurationFilename(void) const
{
    const auto& iterator = m_attributes.find(kSSRenderOperationMainNode + ":" +
                                             kSSRenderOperationMaterialConfigurationFilenameAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->getString();
}

ISharedConfiguration CConfigurationSSRenderOperation::getMaterialConfiguration(void) const
{
    const auto& iterator = m_configurations.find(kSSRenderOperationMainNode + ":" +
                                                 kSSRenderOperationMaterialConfigurationFilenameAttribute);
    assert(iterator != m_configurations.end());
    return iterator->second[0];
}

CConfigurationORenderOperation::CConfigurationORenderOperation(void) :
IConfiguration(E_CONFIGURATION_CLASS_O_RENDEROPERATION),
kORenderOperationMainNode("operation"),
kORenderOperationGuidAttribute("guid"),
kORenderOperationMaterialConfigurationFilenameAttribute("filename")
{
    
}

CConfigurationORenderOperation::~CConfigurationORenderOperation(void)
{
    
}

std::string CConfigurationORenderOperation::getGuid(void) const
{
    const auto& iterator = m_attributes.find(kORenderOperationMainNode + ":" +
                                             kORenderOperationGuidAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->getString();
}

std::string CConfigurationORenderOperation::getMaterialConfigurationFilename(void) const
{
    const auto& iterator = m_attributes.find(kORenderOperationMainNode + ":" +
                                             kORenderOperationMaterialConfigurationFilenameAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->getString();
}

ISharedConfiguration CConfigurationORenderOperation::getMaterialConfiguration(void) const
{
    const auto& iterator = m_configurations.find(kORenderOperationMainNode + ":" +
                                            kORenderOperationMaterialConfigurationFilenameAttribute);
    assert(iterator != m_configurations.end());
    return iterator->second[0];
}

CConfigurationGameTransition::CConfigurationGameTransition(void) :
IConfiguration(E_CONFIGURATION_CLASS_GAMETRANSITION),
kGameTransitionMainNode("transition"),
kGameTransitionGuidAttribute("guid"),
kGameTransitionORenderOperationConfigurationNode("output_render_operation"),
kGameTransitionORenderOperationConfigurationFilenameAttribute("filename"),
kGameTransitionWSRenderOperationsConfigurationsNode("world_space_operations"),
kGameTransitionWSRenderOperationConfigurationNode("operation"),
kGameTransitionWSRenderOperationConfigurationFilenameAttribute("filename"),
kGameTransitionSSRenderOperationsConfigurationsNode("screen_space_operations"),
kGameTransitionSSRenderOperationConfigurationNode("operation"),
kGameTransitionSSRenderOperationConfigurationFilenameAttribute("filename")
{
    
}

CConfigurationGameTransition::~CConfigurationGameTransition(void)
{
    
}

std::string CConfigurationGameTransition::getGuid(void) const
{
    const auto& iterator = m_attributes.find(kGameTransitionMainNode + ":" +
                                             kGameTransitionORenderOperationConfigurationNode);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->getString();
}

std::string CConfigurationGameTransition::getORenderOperationConfigurationFilename(void) const
{
    const auto& iterator = m_attributes.find(kGameTransitionMainNode + ":" +
                                             kGameTransitionORenderOperationConfigurationNode + ":" +
                                             kGameTransitionORenderOperationConfigurationFilenameAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->getString();
}

ISharedConfiguration CConfigurationGameTransition::getORenderOperationConfiguration(void) const
{
    const auto& iterator = m_configurations.find(kGameTransitionMainNode + ":" +
                                            kGameTransitionORenderOperationConfigurationNode + ":" +
                                            kGameTransitionORenderOperationConfigurationFilenameAttribute);
    assert(iterator != m_configurations.end());
    return std::static_pointer_cast<CConfigurationORenderOperation>(iterator->second[0]);
}

std::vector<std::string> CConfigurationGameTransition::getWSRenderOperationsConfigurationsFilenames(void) const
{
    const auto& iterator = m_attributes.find(kGameTransitionMainNode + ":" +
                                             kGameTransitionWSRenderOperationsConfigurationsNode + ":" +
                                             kGameTransitionWSRenderOperationConfigurationNode + ":" +
                                             kGameTransitionWSRenderOperationConfigurationFilenameAttribute);
    assert(iterator != m_attributes.end());
    std::vector<std::string> filenames;
    for(const auto& filename : iterator->second)
    {
        filenames.push_back(filename->getString());
    }
    return filenames;
}

std::vector<ISharedConfiguration> CConfigurationGameTransition::getWSRenderOperationsConfigurations(void) const
{
    const auto& iterator = m_configurations.find(kGameTransitionMainNode + ":" +
                                            kGameTransitionWSRenderOperationsConfigurationsNode + ":" +
                                            kGameTransitionWSRenderOperationConfigurationNode + ":" +
                                            kGameTransitionWSRenderOperationConfigurationFilenameAttribute);
    assert(iterator != m_configurations.end());
    return iterator->second;
}

std::vector<std::string> CConfigurationGameTransition::getSSRenderOperationsConfigurationsFilenames(void) const
{
    const auto& iterator = m_attributes.find(kGameTransitionMainNode + ":" +
                                             kGameTransitionSSRenderOperationsConfigurationsNode + ":" +
                                             kGameTransitionSSRenderOperationConfigurationNode + ":" +
                                             kGameTransitionSSRenderOperationConfigurationFilenameAttribute);
    
    std::vector<std::string> filenames;
    if(iterator != m_attributes.end())
    {
        for(const auto& filename : iterator->second)
        {
            filenames.push_back(filename->getString());
        }
    }
    return filenames;
}

std::vector<ISharedConfiguration> CConfigurationGameTransition::getSSRenderOperationsConfigurations(void) const
{
    const auto& iterator = m_configurations.find(kGameTransitionMainNode + ":" +
                                            kGameTransitionSSRenderOperationsConfigurationsNode + ":" +
                                            kGameTransitionSSRenderOperationConfigurationNode + ":" +
                                            kGameTransitionSSRenderOperationConfigurationFilenameAttribute);
    if(iterator == m_configurations.end())
    {
        return std::vector<ISharedConfiguration>();
    }
    return iterator->second;
}

