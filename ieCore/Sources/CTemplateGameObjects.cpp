//
//  CTemplateGameObjects.cpp
//  indieEngineCore
//
//  Created by Sergey Sergeev on 12/21/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CTemplateGameObjects.h"

CTextureTemplate::CTextureTemplate(void) :
CTemplateCommon(),
kTextureMainNode("texture"),
kTextureFilenameAttribute("filename"),
kTextureRenderOperationNameAttribute("render_operation_name"),
kTextureSamplerIdAttribute("sampler_id"),
kTextureWrapModeAttribute("wrap_mode")
{

}

CTextureTemplate::~CTextureTemplate(void)
{
    
    
}

std::string CTextureTemplate::Get_Filename(void) const
{
    const auto& iterator = m_attributes.find(kTextureMainNode + ":" +
                                             kTextureFilenameAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->Get_String();
}

std::string CTextureTemplate::Get_RenderOperationName(void) const
{
    const auto& iterator = m_attributes.find(kTextureMainNode + ":" +
                                             kTextureRenderOperationNameAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->Get_String();
}

ui32 CTextureTemplate::Get_SamplerId(void) const
{
    const auto& iterator = m_attributes.find(kTextureMainNode + ":" +
                                             kTextureSamplerIdAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->Get_UI32();
}

ui32 CTextureTemplate::Get_WrapMode(void) const
{
    const auto& iterator = m_attributes.find(kTextureMainNode + ":" +
                                             kTextureWrapModeAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->Get_UI32();
}

CShaderTemplate::CShaderTemplate(void) :
CTemplateCommon(),
kShaderMainNode("shader"),
kShaderVSFilenameAttribute("vs_filename"),
kShaderFSFilenameAttribute("fs_filename")
{
    
}

CShaderTemplate::~CShaderTemplate(void)
{
    
}

std::string CShaderTemplate::Get_VSFilename(void) const
{
    const auto& iterator = m_attributes.find(kShaderMainNode + ":" +
                                             kShaderVSFilenameAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->Get_String();
}

std::string CShaderTemplate::Get_FSFilename(void) const
{
    const auto& iterator = m_attributes.find(kShaderMainNode + ":" +
                                             kShaderFSFilenameAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->Get_String();
}

CMaterialTemplate::CMaterialTemplate(void) :
CTemplateCommon(),
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
kMaterialTexturesTemplatesNode("textures"),
kMaterialTextureTemplateNode("texture"),
kMaterialShaderTemplateNode("shader")
{
    
}

CMaterialTemplate::~CMaterialTemplate(void)
{
    
}

std::string CMaterialTemplate::Get_RenderOperationName(void) const
{
    const auto& iterator = m_attributes.find(kMaterialMainNode + ":" +
                                             kMaterialRenderOperationNameAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->Get_String();
}

bool CMaterialTemplate::Get_IsCulling(void) const
{
    const auto& iterator = m_attributes.find(kMaterialMainNode + ":" +
                                             kMaterialIsCullFaceAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->Get_Bool();
}

ui32 CMaterialTemplate::Get_CullingMode(void) const
{
    const auto& iterator = m_attributes.find(kMaterialMainNode + ":" +
                                             kMaterialCullFaceModeAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->Get_UI32();
}

bool CMaterialTemplate::Get_IsBlending(void) const
{
    const auto& iterator = m_attributes.find(kMaterialMainNode + ":" +
                                             kMaterialIsBlendingAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->Get_Bool();
}

ui32 CMaterialTemplate::Get_BlendingFunctionSource(void) const
{
    const auto& iterator = m_attributes.find(kMaterialMainNode + ":" +
                                             kMaterialBlendingFunctionSourceAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->Get_UI32();
}

ui32 CMaterialTemplate::Get_BlendingFunctionDestination(void) const
{
    const auto& iterator = m_attributes.find(kMaterialMainNode + ":" +
                                             kMaterialBlendingFunctionDestinationAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->Get_UI32();
}

bool CMaterialTemplate::Get_IsDepthTest(void) const
{
    const auto& iterator = m_attributes.find(kMaterialMainNode + ":" +
                                             kMaterialIsDepthTestAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->Get_Bool();
}

bool CMaterialTemplate::Get_IsDepthMask(void) const
{
    const auto& iterator = m_attributes.find(kMaterialMainNode + ":" +
                                             kMaterialIsDepthMaskAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->Get_Bool();
}

bool CMaterialTemplate::Get_IsClipping(void) const
{
    const auto& iterator = m_attributes.find(kMaterialMainNode + ":" +
                                             kMaterialIsClippingAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->Get_Bool();
}

glm::vec4 CMaterialTemplate::Get_ClippingPlane(void) const
{
    const auto& iteratorX = m_attributes.find(kMaterialMainNode + ":" +
                                              kMaterialClippingXAttribute);
    assert(iteratorX != m_attributes.end());
    assert(iteratorX->second.size() != 0);
    f32 x = iteratorX->second[0]->Get_F32();
    
    const auto& iteratorY = m_attributes.find(kMaterialMainNode + ":" +
                                              kMaterialClippingYAttribute);
    assert(iteratorY != m_attributes.end());
    assert(iteratorY->second.size() != 0);
    f32 y = iteratorY->second[0]->Get_F32();
    
    const auto& iteratorZ = m_attributes.find(kMaterialMainNode + ":" +
                                              kMaterialClippingZAttribute);
    assert(iteratorZ != m_attributes.end());
    assert(iteratorZ->second.size() != 0);
    f32 z = iteratorZ->second[0]->Get_F32();
    
    const auto& iteratorW = m_attributes.find(kMaterialMainNode+ ":" +
                                              kMaterialClippingWAttribute);
    assert(iteratorW != m_attributes.end());
    assert(iteratorW->second.size() != 0);
    f32 w = iteratorW->second[0]->Get_F32();
    return glm::vec4(x, y, z, w);
}

bool CMaterialTemplate::Get_IsReflecting(void) const
{
    const auto& iterator = m_attributes.find(kMaterialMainNode + ":" +
                                             kMaterialIsReflectingAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->Get_Bool();
}

bool CMaterialTemplate::Get_IsShadowing(void) const
{
    const auto& iterator = m_attributes.find(kMaterialMainNode + ":" +
                                             kMaterialIsShadowingAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->Get_Bool();
}

bool CMaterialTemplate::Get_IsDebugging(void) const
{
    const auto& iterator = m_attributes.find(kMaterialMainNode + ":" +
                                             kMaterialIsDebuggingAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->Get_Bool();
}

std::vector<std::shared_ptr<I_RO_TemplateCommon>> CMaterialTemplate::Get_TexturesTemplates(void) const
{
    const auto& iterator = m_templates.find(kMaterialMainNode + ":" +
                                            kMaterialTexturesTemplatesNode);
    assert(iterator != m_templates.end());
    return iterator->second;
}

std::shared_ptr<I_RO_TemplateCommon> CMaterialTemplate::Get_ShaderTemplate(void) const
{
    const auto& iterator = m_templates.find(kMaterialMainNode + ":" +
                                            kMaterialShaderTemplateNode);
    assert(iterator != m_templates.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0];
}

CGameObjectTemplate::CGameObjectTemplate(void) :
CTemplateCommon(),
kGameObjectMaterialsTemplatesNode("materials"),
kGameObjectMaterialTemplateNode("material"),
kGameObjectMaterialFilenameAttribute("filename")
{
    
}

CGameObjectTemplate::~CGameObjectTemplate(void)
{
    
}

std::vector<std::string> CGameObjectTemplate::Get_MaterialsTemplatesFilenames(void) const
{
    const auto& iterator = m_attributes.find(kGameObjectMaterialsTemplatesNode + ":" +
                                             kGameObjectMaterialTemplateNode + ":" +
                                             kGameObjectMaterialFilenameAttribute);
    assert(iterator != m_attributes.end());
    std::vector<std::string> filenames;
    for(const auto& filename : iterator->second)
    {
        filenames.push_back(filename->Get_String());
    }
    return filenames;
}

std::vector<std::shared_ptr<I_RO_TemplateCommon>> CGameObjectTemplate::Get_MaterialsTemplates(void) const
{
    const auto& iterator = m_templates.find(kGameObjectMaterialsTemplatesNode + ":" +
                                            kGameObjectMaterialTemplateNode + ":" +
                                            kGameObjectMaterialFilenameAttribute);
    assert(iterator != m_templates.end());
    return iterator->second;
}

CModelTemplate::CModelTemplate(void) :
CGameObjectTemplate(),
kModelMainNode("model"),
kModelMeshFilenameAttribute("filename"),
kModelMeshIsBatchingAttribute("is_batching"),
kModelSkeletonNode("skeleton"),
kModelSkeletonFilenameAttribute("filename"),
kModelSequencesNode("sequences"),
kModelSequenceNode("sequence"),
kModelSequenceFilenameAttribute("filename"),
kModelSequenceAnimationNameAttribute("animation_name")
{
    
}

CModelTemplate::~CModelTemplate(void)
{
    
}

std::string CModelTemplate::Get_MeshFilename(void) const
{
    const auto& iterator = m_attributes.find(kModelMainNode + ":" +
                                             kModelMeshFilenameAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->Get_String();
}

std::string CModelTemplate::Get_SkeletonFilename(void) const
{
    const auto& iterator = m_attributes.find(kModelMainNode + ":" +
                                             kModelSkeletonNode + ":" +
                                             kModelSkeletonFilenameAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->Get_String();
}

std::vector<std::string> CModelTemplate::Get_SequencesFilenames(void) const
{
    const auto& iterator = m_attributes.find(kModelMainNode + ":" +
                                             kModelSequencesNode + ":" +
                                             kModelSequenceNode + ":" +
                                             kModelSequenceFilenameAttribute);
    assert(iterator != m_attributes.end());
    std::vector<std::string> filenames;
    for(const auto& filename : iterator->second)
    {
        filenames.push_back(filename->Get_String());
    }
    return filenames;
}

std::vector<std::string> CModelTemplate::Get_SequenceAnimationsNames(void) const
{
    const auto& iterator = m_attributes.find(kModelMainNode + ":" +
                                             kModelSequencesNode + ":" +
                                             kModelSequenceNode + ":" +
                                             kModelSequenceAnimationNameAttribute);
    assert(iterator != m_attributes.end());
    std::vector<std::string> animationsNames;
    for(const auto& animationName : iterator->second)
    {
        animationsNames.push_back(animationName->Get_String());
    }
    return animationsNames;
}

bool CModelTemplate::Get_IsBatching(void) const
{
    const auto& iterator = m_attributes.find(kModelMainNode + ":" +
                                             kModelMeshIsBatchingAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->Get_Bool();
}

COceanTemplate::COceanTemplate(void) :
CGameObjectTemplate(),
kOceanMainNode("ocean"),
kOceanSizeXAttribute("size_x"),
kOceanSizeYAttribute("size_y"),
kOceanAltitudeAttribute("altitude"),
kOceanWaveGenerationIntervalAttribute("wave_generation_interval")
{
    
}

COceanTemplate::~COceanTemplate(void)
{
    
}

glm::vec2 COceanTemplate::Get_Size(void) const
{
    const auto& iteratorX = m_attributes.find(kOceanMainNode + ":" +
                                              kOceanSizeXAttribute);
    assert(iteratorX != m_attributes.end());
    assert(iteratorX->second.size() != 0);
    f32 x = iteratorX->second[0]->Get_F32();
    
    const auto& iteratorY = m_attributes.find(kOceanMainNode + ":" +
                                              kOceanSizeYAttribute);
    assert(iteratorY != m_attributes.end());
    assert(iteratorY->second.size() != 0);
    f32 y = iteratorY->second[0]->Get_F32();
    
    return glm::vec2(x, y);
}

f32 COceanTemplate::Get_Altitude(void) const
{
    const auto& iterator = m_attributes.find(kOceanMainNode + ":" +
                                             kOceanAltitudeAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->Get_F32();
}

f32 COceanTemplate::Get_WaveGenerationInterval(void) const
{
    const auto& iterator = m_attributes.find(kOceanMainNode + ":" +
                                             kOceanWaveGenerationIntervalAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->Get_F32();
}

CParticleEmitterTemplate::CParticleEmitterTemplate(void) :
CGameObjectTemplate(),
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

CParticleEmitterTemplate::~CParticleEmitterTemplate(void)
{
    
}

ui32 CParticleEmitterTemplate::Get_NumParticles(void) const
{
    const auto& iterator = m_attributes.find(kParticleEmitterMainNode + ":" +
                                             kParticleEmitterNumParticlesAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->Get_UI32();
}

ui64 CParticleEmitterTemplate::Get_Duration(void) const
{
    const auto& iterator = m_attributes.find(kParticleEmitterMainNode + ":" +
                                             kParticleEmitterDurationAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->Get_UI32();
}

f32 CParticleEmitterTemplate::Get_DurationRandomness(void) const
{
    const auto& iterator = m_attributes.find(kParticleEmitterMainNode + ":" +
                                             kParticleEmitterDurationAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->Get_F32();
}

f32 CParticleEmitterTemplate::Get_VelocitySensitivity(void) const
{
    const auto& iterator = m_attributes.find(kParticleEmitterMainNode + ":" +
                                             kParticleEmitterVelocitySensitivityAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->Get_F32();

}

f32 CParticleEmitterTemplate::Get_MinHorizontalVelocity(void) const
{
    const auto& iterator = m_attributes.find(kParticleEmitterMainNode + ":" +
                                             kParticleEmitterMinHorizontalVelocityAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->Get_F32();
}

f32 CParticleEmitterTemplate::Get_MaxHorizontalVelocity(void) const
{
    const auto& iterator = m_attributes.find(kParticleEmitterMainNode + ":" +
                                             kParticleEmitterMaxHorizontalVelocityAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->Get_F32();
}

f32 CParticleEmitterTemplate::Get_MinVerticalVelocity(void) const
{
    const auto& iterator = m_attributes.find(kParticleEmitterMainNode + ":" +
                                             kParticleEmitterMinVerticalVelocityAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->Get_F32();
}

f32 CParticleEmitterTemplate::Get_MaxVerticalVelocity(void) const
{
    const auto& iterator = m_attributes.find(kParticleEmitterMainNode + ":" +
                                           kParticleEmitterMaxVerticalVelocityAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->Get_F32();
    
}

f32 CParticleEmitterTemplate::Get_EndVelocity(void) const
{
    const auto& iterator = m_attributes.find(kParticleEmitterMainNode + ":" +
                                             kParticleEmitterEndVelocityAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->Get_F32();
}

glm::vec3 CParticleEmitterTemplate::Get_Gravity(void) const
{
    const auto& iteratorX = m_attributes.find(kParticleEmitterMainNode + ":" +
                                             kParticleEmitterGravityXAttribute);
    assert(iteratorX != m_attributes.end());
    assert(iteratorX->second.size() != 0);
    f32 x = iteratorX->second[0]->Get_F32();
    
    const auto& iteratorY = m_attributes.find(kParticleEmitterMainNode + ":" +
                                              kParticleEmitterGravityYAttribute);
    assert(iteratorY != m_attributes.end());
    assert(iteratorY->second.size() != 0);
    ui8 y = iteratorY->second[0]->Get_F32();
    
    const auto& iteratorZ = m_attributes.find(kParticleEmitterMainNode + ":" +
                                              kParticleEmitterGravityZAttribute);
    assert(iteratorZ != m_attributes.end());
    assert(iteratorZ->second.size() != 0);
    ui8 z = iteratorZ->second[0]->Get_F32();
    
    return glm::vec3(x, y, z);
}

glm::u8vec4 CParticleEmitterTemplate::Get_SourceColor(void) const
{
    const auto& iteratorR = m_attributes.find(kParticleEmitterMainNode + ":" +
                                              kParticleEmitterSourceColorRAttribute);
    assert(iteratorR != m_attributes.end());
    assert(iteratorR->second.size() != 0);
    ui8 r = iteratorR->second[0]->Get_UI8();
    
    const auto& iteratorG = m_attributes.find(kParticleEmitterMainNode + ":" +
                                              kParticleEmitterSourceColorGAttribute);
    assert(iteratorG != m_attributes.end());
    assert(iteratorG->second.size() != 0);
    ui8 g = iteratorG->second[0]->Get_UI8();
    
    const auto& iteratorB = m_attributes.find(kParticleEmitterMainNode + ":" +
                                              kParticleEmitterSourceColorBAttribute);
    assert(iteratorB != m_attributes.end());
    assert(iteratorB->second.size() != 0);
    ui8 b = iteratorB->second[0]->Get_UI8();
    
    const auto& iteratorA = m_attributes.find(kParticleEmitterMainNode + ":" +
                                              kParticleEmitterSourceColorAAttribute);
    assert(iteratorA != m_attributes.end());
    assert(iteratorA->second.size() != 0);
    ui8 a = iteratorA->second[0]->Get_UI8();
    return glm::u8vec4(r, g, b, a);
}

glm::u8vec4 CParticleEmitterTemplate::Get_DestinationColor(void) const
{
    const auto& iteratorR = m_attributes.find(kParticleEmitterMainNode + ":" +
                                              kParticleEmitterDestinationColorRAttribute);
    assert(iteratorR != m_attributes.end());
    assert(iteratorR->second.size() != 0);
    ui8 r = iteratorR->second[0]->Get_UI8();
    
    const auto& iteratorG = m_attributes.find(kParticleEmitterMainNode + ":" +
                                              kParticleEmitterDestinationColorGAttribute);
    assert(iteratorG != m_attributes.end());
    assert(iteratorG->second.size() != 0);
    ui8 g = iteratorG->second[0]->Get_UI8();
    
    const auto& iteratorB = m_attributes.find(kParticleEmitterMainNode + ":" +
                                              kParticleEmitterDestinationColorBAttribute);
    assert(iteratorB != m_attributes.end());
    assert(iteratorB->second.size() != 0);
    ui8 b = iteratorB->second[0]->Get_UI8();
    
    const auto& iteratorA = m_attributes.find(kParticleEmitterMainNode + ":" +
                                              kParticleEmitterDestinationColorAAttribute);
    assert(iteratorA != m_attributes.end());
    assert(iteratorA->second.size() != 0);
    ui8 a = iteratorA->second[0]->Get_UI8();
    
    return glm::u8vec4(r, g, b, a);
}

glm::vec2 CParticleEmitterTemplate::Get_SourceSize(void) const
{
    const auto& iteratorX = m_attributes.find(kParticleEmitterMainNode + ":" +
                                              kParticleEmitterSourceSizeXAttribute);
    assert(iteratorX != m_attributes.end());
    assert(iteratorX->second.size() != 0);
    f32 x = iteratorX->second[0]->Get_F32();
    
    const auto& iteratorY = m_attributes.find(kParticleEmitterMainNode + ":" +
                                              kParticleEmitterSourceSizeYAttribute);
    assert(iteratorY != m_attributes.end());
    assert(iteratorY->second.size() != 0);
    ui8 y = iteratorY->second[0]->Get_F32();
    
    return glm::vec2(x, y);
}

glm::vec2 CParticleEmitterTemplate::Get_DestinationSize(void) const
{
    const auto& iteratorX = m_attributes.find(kParticleEmitterMainNode + ":" +
                                              kParticleEmitterDestinationSizeXAttribute);
    assert(iteratorX != m_attributes.end());
    assert(iteratorX->second.size() != 0);
    f32 x = iteratorX->second[0]->Get_F32();
    
    const auto& iteratorY = m_attributes.find(kParticleEmitterMainNode + ":" +
                                              kParticleEmitterDestinationSizeXAttribute);
    assert(iteratorY != m_attributes.end());
    assert(iteratorY->second.size() != 0);
    ui8 y = iteratorY->second[0]->Get_F32();
    
    return glm::vec2(x, y);
}

ui64 CParticleEmitterTemplate::Get_MinEmittInterval(void) const
{
    const auto& iterator = m_attributes.find(kParticleEmitterMainNode + ":" +
                                              kParticleEmitterMaxEmittIntervalAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->Get_UI32();
}

ui64 CParticleEmitterTemplate::Get_MaxEmittInterval(void) const
{
    const auto& iterator = m_attributes.find(kParticleEmitterMainNode + ":" +
                                             kParticleEmitterMinEmittIntervalAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->Get_UI32();
}

CLandscapeTemplate::CLandscapeTemplate(void) :
CGameObjectTemplate(),
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
kLandscapeEdgesMaterialsTemplatesNode("edges_materials"),
kLandscapeEdgeMaterialTemplateNode("material"),
kLandscapeEdgeMaterialTemplateFilenameAttribute("filename")
{
    
}

CLandscapeTemplate::~CLandscapeTemplate(void)
{
    
}

std::string CLandscapeTemplate::Get_HeightmapDataFilename(void) const
{
    const auto& iterator = m_attributes.find(kLandscapeMainNode + ":" +
                                             kLandscapeHeightmapDataFilenameAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->Get_String();
}

std::string CLandscapeTemplate::Get_SplattingDataFilename(void) const
{
    const auto& iterator = m_attributes.find(kLandscapeMainNode + ":" +
                                             kLandscapeSplattingDataFilenameAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->Get_String();
}

glm::vec2 CLandscapeTemplate::Get_Size(void) const
{
    const auto& iteratorX = m_attributes.find(kLandscapeMainNode + ":" +
                                              kLandscapeSizeXAttribute);
    assert(iteratorX != m_attributes.end());
    assert(iteratorX->second.size() != 0);
    f32 x = iteratorX->second[0]->Get_F32();
    
    const auto& iteratorY = m_attributes.find(kLandscapeMainNode + ":" +
                                              kLandscapeSizeYAttribute);
    assert(iteratorY != m_attributes.end());
    assert(iteratorY->second.size() != 0);
    ui8 y = iteratorY->second[0]->Get_F32();
    
    return glm::vec2(x, y);
}

std::string CLandscapeTemplate::Get_SplattingDiffuseMaterialFilename(void) const
{
    const auto& iterator = m_attributes.find(kLandscapeMainNode + ":" +
                                             kLandscapeSplattingDiffuseMaterialFilenameAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->Get_String();
}

std::string CLandscapeTemplate::Get_SplattingNormalMaterialFilename(void) const
{
    const auto& iterator = m_attributes.find(kLandscapeMainNode + ":" +
                                             kLandscapeSplattingNormalMaterialFilenameAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->Get_String();
}

std::shared_ptr<I_RO_TemplateCommon> CLandscapeTemplate::Get_SplattingDiffuseMaterialTemplate(void) const
{
    const auto& iterator = m_templates.find(kLandscapeMainNode + ":" +
                                             kLandscapeSplattingDiffuseMaterialFilenameAttribute);
    assert(iterator != m_templates.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0];
}

std::shared_ptr<I_RO_TemplateCommon> CLandscapeTemplate::Get_SplattingNormalMaterialTemplate(void) const
{
    const auto& iterator = m_templates.find(kLandscapeMainNode + ":" +
                                            kLandscapeSplattingNormalMaterialFilenameAttribute);
    assert(iterator != m_templates.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0];
}

bool CLandscapeTemplate::Get_IsEdgesEnabled(void) const
{
    const auto& iterator = m_attributes.find(kLandscapeMainNode + ":" +
                                             kLandscapeIsEdgesEnabledAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->Get_Bool();
}

glm::vec2 CLandscapeTemplate::Get_EdgesSize(void) const
{
    const auto& iteratorX = m_attributes.find(kLandscapeMainNode + ":" +
                                              kLandscapeEdgesNode + ":" +
                                              kLandscapeEdgesSizeXAttribute);
    assert(iteratorX != m_attributes.end());
    assert(iteratorX->second.size() != 0);
    f32 x = iteratorX->second[0]->Get_F32();
    
    const auto& iteratorY = m_attributes.find(kLandscapeMainNode + ":" +
                                              kLandscapeEdgesNode + ":" +
                                              kLandscapeEdgesSizeYAttribute);
    assert(iteratorY != m_attributes.end());
    assert(iteratorY->second.size() != 0);
    ui8 y = iteratorY->second[0]->Get_F32();
    
    return glm::vec2(x, y);
}

std::vector<std::string> CLandscapeTemplate::Get_EdgesMaterialsFilenames(void) const
{
    const auto& iterator = m_attributes.find(kLandscapeMainNode + ":" +
                                             kLandscapeEdgesNode + ":" +
                                             kLandscapeEdgesMaterialsTemplatesNode + ":" +
                                             kLandscapeEdgeMaterialTemplateNode + ":" +
                                             kLandscapeEdgeMaterialTemplateFilenameAttribute);
    assert(iterator != m_attributes.end());
    std::vector<std::string> filenames;
    for(const auto& filename : iterator->second)
    {
        filenames.push_back(filename->Get_String());
    }
    return filenames;
}

std::vector<std::shared_ptr<I_RO_TemplateCommon>> CLandscapeTemplate::Get_EdgesMaterialsTemplates(void) const
{
    const auto& iterator = m_templates.find(kLandscapeMainNode + ":" +
                                            kLandscapeEdgesNode + ":" +
                                            kLandscapeEdgesMaterialsTemplatesNode + ":" +
                                            kLandscapeEdgeMaterialTemplateNode + ":" +
                                            kLandscapeEdgeMaterialTemplateFilenameAttribute);
    assert(iterator != m_templates.end());
    return iterator->second;
}

CWorldSpaceRenderOperationTemplate::CWorldSpaceRenderOperationTemplate(void) :
CTemplateCommon(),
kWorldSpaceRenderOperationMainNode("operation"),
kWorldSpaceRenderOperationGuidAttribute("guid"),
kWorldSpaceRenderOperationIndexAttribute("index"),
kWorldSpaceRenderOperationScreenWidthAttribute("width"),
kWorldSpaceRenderOperationScreenHeightAttribute("height"),
kWorldSpaceRenderOperationClearColorRAttribute("clear_color_r"),
kWorldSpaceRenderOperationClearColorGAttribute("clear_color_g"),
kWorldSpaceRenderOperationClearColorBAttribute("clear_color_b"),
kWorldSpaceRenderOperationClearColorAAttribute("clear_color_a")
{
    
}

CWorldSpaceRenderOperationTemplate::~CWorldSpaceRenderOperationTemplate(void)
{
    
}

std::string CWorldSpaceRenderOperationTemplate::Get_Guid(void) const
{
    const auto& iterator = m_attributes.find(kWorldSpaceRenderOperationMainNode + ":" +
                                             kWorldSpaceRenderOperationGuidAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->Get_String();
}

ui32 CWorldSpaceRenderOperationTemplate::Get_Index(void) const
{
    const auto& iterator = m_attributes.find(kWorldSpaceRenderOperationMainNode + ":" +
                                             kWorldSpaceRenderOperationIndexAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->Get_UI32();
}

ui32 CWorldSpaceRenderOperationTemplate::Get_ScreenWidth(void) const
{
    const auto& iterator = m_attributes.find(kWorldSpaceRenderOperationMainNode + ":" +
                                             kWorldSpaceRenderOperationScreenWidthAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->Get_UI32();
}

ui32 CWorldSpaceRenderOperationTemplate::Get_ScreenHeight(void) const
{
    const auto& iterator = m_attributes.find(kWorldSpaceRenderOperationMainNode + ":" +
                                             kWorldSpaceRenderOperationScreenHeightAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->Get_UI32();
}

glm::vec4 CWorldSpaceRenderOperationTemplate::Get_ClearColor(void) const
{
    const auto& iteratorR = m_attributes.find(kWorldSpaceRenderOperationMainNode + ":" +
                                              kWorldSpaceRenderOperationClearColorRAttribute);
    assert(iteratorR != m_attributes.end());
    assert(iteratorR->second.size() != 0);
    f32 r = iteratorR->second[0]->Get_F32();
    
    const auto& iteratorG = m_attributes.find(kWorldSpaceRenderOperationMainNode + ":" +
                                              kWorldSpaceRenderOperationClearColorGAttribute);
    assert(iteratorG != m_attributes.end());
    assert(iteratorG->second.size() != 0);
    ui8 g = iteratorG->second[0]->Get_F32();
    
    const auto& iteratorB = m_attributes.find(kWorldSpaceRenderOperationMainNode + ":" +
                                              kWorldSpaceRenderOperationClearColorBAttribute);
    assert(iteratorB != m_attributes.end());
    assert(iteratorB->second.size() != 0);
    ui8 b = iteratorB->second[0]->Get_F32();
    
    const auto& iteratorA = m_attributes.find(kWorldSpaceRenderOperationMainNode + ":" +
                                              kWorldSpaceRenderOperationClearColorAAttribute);
    assert(iteratorA != m_attributes.end());
    assert(iteratorA->second.size() != 0);
    ui8 a = iteratorA->second[0]->Get_F32();
    
    return glm::vec4(r, g, b, a);
}

CScreenSpaceRenderOperationTemplate::CScreenSpaceRenderOperationTemplate(void) :
CTemplateCommon(),
kScreenSpaceRenderOperationMainNode("operation"),
kScreenSpaceRenderOperationGuidAttribute("guid"),
kScreenSpaceRenderOperationScreenWidthAttribute("width"),
kScreenSpaceRenderOperationScreenHeightAttribute("height"),
kScreenSpaceRenderOperationMaterialTemplateFilenameAttribute("material_filename")
{
    
}

CScreenSpaceRenderOperationTemplate::~CScreenSpaceRenderOperationTemplate(void)
{
    
}

std::string CScreenSpaceRenderOperationTemplate::Get_Guid(void) const
{
    const auto& iterator = m_attributes.find(kScreenSpaceRenderOperationMainNode + ":" +
                                             kScreenSpaceRenderOperationGuidAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->Get_String();
}

ui32 CScreenSpaceRenderOperationTemplate::Get_ScreenWidth(void) const
{
    const auto& iterator = m_attributes.find(kScreenSpaceRenderOperationMainNode + ":" +
                                             kScreenSpaceRenderOperationScreenWidthAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->Get_UI32();
}

ui32 CScreenSpaceRenderOperationTemplate::Get_ScreenHeight(void) const
{
    const auto& iterator = m_attributes.find(kScreenSpaceRenderOperationMainNode + ":" +
                                             kScreenSpaceRenderOperationScreenHeightAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->Get_UI32();
}

std::string CScreenSpaceRenderOperationTemplate::Get_MaterialTemplateFilename(void) const
{
    const auto& iterator = m_attributes.find(kScreenSpaceRenderOperationMainNode + ":" +
                                             kScreenSpaceRenderOperationMaterialTemplateFilenameAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->Get_String();
}

std::shared_ptr<I_RO_TemplateCommon> CScreenSpaceRenderOperationTemplate::Get_MaterialTemplate(void) const
{
    const auto& iterator = m_templates.find(kScreenSpaceRenderOperationMainNode + ":" +
                                            kScreenSpaceRenderOperationMaterialTemplateFilenameAttribute);
    assert(iterator != m_templates.end());
    return iterator->second[0];
}

COutputRenderOperationTemplate::COutputRenderOperationTemplate(void) :
CTemplateCommon(),
kOutputRenderOperationMainNode("operation"),
kOutputRenderOperationGuidAttribute("guid"),
kOutputRenderOperationMaterialTemplateFilenameAttribute("filename")
{
    
}

COutputRenderOperationTemplate::~COutputRenderOperationTemplate(void)
{
    
}

std::string COutputRenderOperationTemplate::Get_Guid(void) const
{
    const auto& iterator = m_attributes.find(kOutputRenderOperationMainNode + ":" +
                                             kOutputRenderOperationGuidAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->Get_String();
}

std::string COutputRenderOperationTemplate::Get_MaterialTemplateFilename(void) const
{
    const auto& iterator = m_attributes.find(kOutputRenderOperationMainNode + ":" +
                                             kOutputRenderOperationMaterialTemplateFilenameAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->Get_String();
}

std::shared_ptr<I_RO_TemplateCommon> COutputRenderOperationTemplate::Get_MaterialTemplate(void) const
{
    const auto& iterator = m_templates.find(kOutputRenderOperationMainNode + ":" +
                                            kOutputRenderOperationMaterialTemplateFilenameAttribute);
    assert(iterator != m_templates.end());
    return iterator->second[0];
}

CGameTransitionTemplate::CGameTransitionTemplate(void) :
CTemplateCommon(),
kGameTransitionMainNode("transition"),
kGameTransitionGuidAttribute("guid"),
kGameTransitionOutputRenderOperationTemplateNode("output_render_operation"),
kGameTransitionOutputRenderOperationTemplateFilenameAttribute("filename"),
kGameTransitionWorldSpaceRenderOperationsTemplatesNode("world_space_operations"),
kGameTransitionWorldSpaceRenderOperationTemplateNode("operation"),
kGameTransitionWorldSpaceRenderOperationTemplateFilenameAttribute("filename"),
kGameTransitionScreenSpaceRenderOperationsTemplatesNode("screen_space_operations"),
kGameTransitionScreenSpaceRenderOperationTemplateNode("operation"),
kGameTransitionScreenSpaceRenderOperationTemplateFilenameAttribute("filename")
{
    
}

CGameTransitionTemplate::~CGameTransitionTemplate(void)
{
    
}

std::string CGameTransitionTemplate::Get_Guid(void) const
{
    const auto& iterator = m_attributes.find(kGameTransitionMainNode + ":" +
                                             kGameTransitionOutputRenderOperationTemplateNode);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->Get_String();
}

std::string CGameTransitionTemplate::Get_OutputRenderOperationTemplateFilename(void) const
{
    const auto& iterator = m_attributes.find(kGameTransitionMainNode + ":" +
                                             kGameTransitionOutputRenderOperationTemplateNode + ":" +
                                             kGameTransitionOutputRenderOperationTemplateFilenameAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return iterator->second[0]->Get_String();
}

std::shared_ptr<I_RO_TemplateCommon> CGameTransitionTemplate::Get_OutputRenderOperationTemplate(void) const
{
    const auto& iterator = m_templates.find(kGameTransitionMainNode + ":" +
                                            kGameTransitionOutputRenderOperationTemplateNode + ":" +
                                            kGameTransitionOutputRenderOperationTemplateFilenameAttribute);
    assert(iterator != m_templates.end());
    return iterator->second[0];
}

std::vector<std::string> CGameTransitionTemplate::Get_WorldSpaceRenderOperationsTemplatesFilenames(void) const
{
    const auto& iterator = m_attributes.find(kGameTransitionMainNode + ":" +
                                             kGameTransitionWorldSpaceRenderOperationsTemplatesNode + ":" +
                                             kGameTransitionWorldSpaceRenderOperationTemplateNode + ":" +
                                             kGameTransitionWorldSpaceRenderOperationTemplateFilenameAttribute);
    assert(iterator != m_attributes.end());
    std::vector<std::string> filenames;
    for(const auto& filename : iterator->second)
    {
        filenames.push_back(filename->Get_String());
    }
    return filenames;
}

std::vector<std::shared_ptr<I_RO_TemplateCommon>> CGameTransitionTemplate::Get_WorldSpaceRenderOperationsTemplates(void) const
{
    const auto& iterator = m_templates.find(kGameTransitionMainNode + ":" +
                                            kGameTransitionWorldSpaceRenderOperationsTemplatesNode + ":" +
                                            kGameTransitionWorldSpaceRenderOperationTemplateNode + ":" +
                                            kGameTransitionWorldSpaceRenderOperationTemplateFilenameAttribute);
    assert(iterator != m_templates.end());
    return iterator->second;
}

std::vector<std::string> CGameTransitionTemplate::Get_ScreenSpaceRenderOperationsTemplatesFilenames(void) const
{
    const auto& iterator = m_attributes.find(kGameTransitionMainNode + ":" +
                                             kGameTransitionScreenSpaceRenderOperationsTemplatesNode + ":" +
                                             kGameTransitionScreenSpaceRenderOperationTemplateNode + ":" +
                                             kGameTransitionScreenSpaceRenderOperationTemplateFilenameAttribute);
    
    std::vector<std::string> filenames;
    if(iterator != m_attributes.end())
    {
        for(const auto& filename : iterator->second)
        {
            filenames.push_back(filename->Get_String());
        }
    }
    return filenames;
}

std::vector<std::shared_ptr<I_RO_TemplateCommon>> CGameTransitionTemplate::Get_ScreenSpaceRenderOperationsTemplates(void) const
{
    const auto& iterator = m_templates.find(kGameTransitionMainNode + ":" +
                                            kGameTransitionScreenSpaceRenderOperationsTemplatesNode + ":" +
                                            kGameTransitionScreenSpaceRenderOperationTemplateNode + ":" +
                                            kGameTransitionScreenSpaceRenderOperationTemplateFilenameAttribute);
    if(iterator == m_templates.end())
    {
        return std::vector<std::shared_ptr<I_RO_TemplateCommon>>();
    }
    return iterator->second;
}

