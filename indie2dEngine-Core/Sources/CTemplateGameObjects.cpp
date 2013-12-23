//
//  CTemplateGameObjects.cpp
//  indieEngineCore
//
//  Created by Sergey Sergeev on 12/21/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CTemplateGameObjects.h"

CTemplateTexture::CTemplateTexture(void) :
CTemplateCommon(),
kTextureMainNode("texture"),
kTextureFilenameAttribute("texture_filename"),
kTextureRenderOperationNameAttribute("render_operation_name"),
kTextureShaderSamplerIdAttribute("shader_sampler_id"),
kTextureWrapModeAttribute("texture_wrap_mode")
{

}

CTemplateTexture::~CTemplateTexture(void)
{
    
    
}

std::string CTemplateTexture::Get_RenderOperationName(void)
{
    const auto& iterator = m_attributes.find(kTextureRenderOperationNameAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return std::get<E_TEMPLATE_META_TYPE_STRING>(iterator->second[0].second);
}

ui32 CTemplateTexture::Get_ShaderSampleId(void)
{
    const auto& iterator = m_attributes.find(kTextureShaderSamplerIdAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return std::get<E_TEMPLATE_META_TYPE_UI32>(iterator->second[0].second);
}

ui32 CTemplateTexture::Get_TextureWrapMode(void)
{
    const auto& iterator = m_attributes.find(kTextureWrapModeAttribute);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return std::get<E_TEMPLATE_META_TYPE_UI32>(iterator->second[0].second);
}

CTemplateShader::CTemplateShader(void) :
CTemplateCommon(),
kShaderMainNode("shader"),
kShaderVSFilename("shader_vs_filename"),
kShaderFSFilename("shader_fs_filename")
{
    
}

CTemplateShader::~CTemplateShader(void)
{
    
}

std::string CTemplateShader::Get_VSFilename(void)
{
    const auto& iterator = m_attributes.find(kShaderVSFilename);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return std::get<E_TEMPLATE_META_TYPE_STRING>(iterator->second[0].second);
}

std::string CTemplateShader::Get_FSFilename(void)
{
    const auto& iterator = m_attributes.find(kShaderFSFilename);
    assert(iterator != m_attributes.end());
    assert(iterator->second.size() != 0);
    return std::get<E_TEMPLATE_META_TYPE_STRING>(iterator->second[0].second);
}

CTemplateMaterial::CTemplateMaterial(void) :
CTemplateCommon(),
kMaterialMainNode("material"),
kMaterialRenderOperationName("material_render_operation_name"),
kMaterialIsCullFace("material_is_cull_face"),
kMaterialCullFaceMode("material_cull_face_mode"),
kMaterialIsBlending;
kMaterialBlendingFunctionSource;
kMaterialBlendingFunctionDestination;
kMaterialIsDepthTest;
kMaterialIsDepthMask;
kMaterialIsClipping;
kMaterialClippingX;
kMaterialClippingY;
kMaterialClippingZ;
kMaterialClippingW;
kMaterialIsReflecting;
kMaterialIsShadowing;
kMaterialIsDebugging;

{
    
}