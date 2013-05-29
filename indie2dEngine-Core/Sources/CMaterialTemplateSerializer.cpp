//
//  CMaterialTemplateSerializer.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/29/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CMaterialTemplateSerializer.h"
#include "CCommonOS.h"
#include "ITemplate.h"

std::map<std::string, GLenum> g_glEnumContainer = {
    {"GL_FRONT", GL_FRONT},
    {"GL_BACK", GL_BACK},
    {"GL_SRC_ALPHA", GL_SRC_ALPHA},
    {"GL_ONE_MINUS_SRC_ALPHA", GL_ONE_MINUS_SRC_ALPHA},
    {"GL_REPEAT", GL_REPEAT},
    {"GL_CLAMP_TO_EDGE", GL_CLAMP_TO_EDGE },
    {"GL_MIRRORED_REPEAT", GL_MIRRORED_REPEAT }
};

CMaterialTemplateSerializer::CMaterialTemplateSerializer(void)
{
    
}

CMaterialTemplateSerializer::~CMaterialTemplateSerializer(void)
{
    
}

std::shared_ptr<ITemplate> CMaterialTemplateSerializer::Serialize(const std::string& _filename)
{
    std::string path(Get_BundlePath());
    path.append(_filename);
    pugi::xml_document document;
    pugi::xml_parse_result result = document.load_file(path.c_str());
    assert(result.status == pugi::status_ok);
    pugi::xml_node node = document.child("material");
    
    std::shared_ptr<SMaterialTemplate> materialTemplate = std::make_shared<SMaterialTemplate>();
    materialTemplate->m_renderMode = node.attribute("render_mode").as_string();
    materialTemplate->m_isCullFace = node.attribute("is_cull_face").as_bool();
    materialTemplate->m_isDepthTest = node.attribute("is_depth_test").as_bool();
    materialTemplate->m_isDepthMask = node.attribute("is_depth_mask").as_bool();
    materialTemplate->m_isBlend = node.attribute("is_blend").as_bool();
    
    std::string cullFaceModeStr = node.attribute("cull_face_mode").as_string();
    assert(g_glEnumContainer.find(cullFaceModeStr) != g_glEnumContainer.end());
    materialTemplate->m_cullFaceMode = g_glEnumContainer.find(cullFaceModeStr)->second;
    
    std::string blendFunctionSourceStr = node.attribute("blend_function_source").as_string();
    assert(g_glEnumContainer.find(blendFunctionSourceStr) != g_glEnumContainer.end());
    materialTemplate->m_blendFunctionSource = g_glEnumContainer.find(blendFunctionSourceStr)->second;
    
    std::string blendFunctionDestinationStr = node.attribute("blend_function_destination").as_string();
    assert(g_glEnumContainer.find(blendFunctionDestinationStr) != g_glEnumContainer.end());
    materialTemplate->m_blendFunctionDestination = g_glEnumContainer.find(blendFunctionDestinationStr)->second;
    
    std::shared_ptr<SShaderTemplate> shaderTemplate = std::make_shared<SShaderTemplate>();
    shaderTemplate->m_vsFilename = node.child("shader").attribute("vs_name").as_string();
    shaderTemplate->m_fsFilename = node.child("shader").attribute("fs_name").as_string();
    materialTemplate->m_shaderTemplate = shaderTemplate;
    
    pugi::xml_node textures_node = node.child("textures");
    for (pugi::xml_node texture = textures_node.child("texture"); texture; texture = texture.next_sibling("texture"))
    {
        std::shared_ptr<STextureTemplate> textureTemplate = std::make_shared<STextureTemplate>();
        textureTemplate->m_filename = texture.attribute("name").as_string();
        textureTemplate->m_sampler = texture.attribute("slot").as_uint();
        
        std::string wrapStr = texture.attribute("wrap").as_string();
        assert(g_glEnumContainer.find(wrapStr) != g_glEnumContainer.end());
        textureTemplate->m_wrap = g_glEnumContainer.find(wrapStr)->second;
        materialTemplate->m_texturesTemplates.push_back(textureTemplate);
    }
    return materialTemplate;
}
