//
//  IGameObjectTemplateSerializer.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/28/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "IGameObjectTemplateSerializer.h"

std::map<std::string, GLenum> g_glEnumContainer = {
    {"GL_FRONT", GL_FRONT},
    {"GL_BACK", GL_BACK},
    {"GL_SRC_ALPHA", GL_SRC_ALPHA},
    {"GL_ONE_MINUS_SRC_ALPHA", GL_ONE_MINUS_SRC_ALPHA},
    {"GL_REPEAT", GL_REPEAT},
    {"GL_CLAMP_TO_EDGE", GL_CLAMP_TO_EDGE },
    {"GL_MIRRORED_REPEAT", GL_MIRRORED_REPEAT }
};

IGameObjectTemplateSerializer::IGameObjectTemplateSerializer(void)
{
    
}

IGameObjectTemplateSerializer::~IGameObjectTemplateSerializer(void)
{
    
}

std::vector<std::shared_ptr<SMaterialTemplate> > IGameObjectTemplateSerializer::Deserialize(pugi::xml_node const& _templateNode)
{
    std::vector<std::shared_ptr<SMaterialTemplate> > materialsTemplates;
    
    pugi::xml_node materials_node = _templateNode.child("materials");
    
    for (pugi::xml_node material = materials_node.child("material"); material; material = material.next_sibling("material"))
    {
        std::shared_ptr<SMaterialTemplate> materialTemplate = std::make_shared<SMaterialTemplate>();
        materialTemplate->m_renderMode = material.attribute("render_mode").as_uint();
        materialTemplate->m_isCullFace = material.attribute("is_cull_face").as_bool();
        materialTemplate->m_isDepthTest = material.attribute("is_depth_test").as_bool();
        materialTemplate->m_isDepthMask = material.attribute("is_depth_mask").as_bool();
        materialTemplate->m_isBlend = material.attribute("is_blend").as_bool();
        
        std::string cullFaceModeStr = material.attribute("cull_face_mode").as_string();
        assert(g_glEnumContainer.find(cullFaceModeStr) != g_glEnumContainer.end());
        materialTemplate->m_cullFaceMode = g_glEnumContainer.find(cullFaceModeStr)->second;
        
        std::string blendFunctionSourceStr = material.attribute("blend_function_source").as_string();
        assert(g_glEnumContainer.find(blendFunctionSourceStr) != g_glEnumContainer.end());
        materialTemplate->m_blendFunctionSource = g_glEnumContainer.find(blendFunctionSourceStr)->second;
        
        std::string blendFunctionDestinationStr = material.attribute("blend_function_destination").as_string();
        assert(g_glEnumContainer.find(blendFunctionDestinationStr) != g_glEnumContainer.end());
        materialTemplate->m_blendFunctionDestination = g_glEnumContainer.find(blendFunctionDestinationStr)->second;
        
        std::shared_ptr<SShaderTemplate> shaderTemplate = std::make_shared<SShaderTemplate>();
        shaderTemplate->m_vsFilename = material.child("shader").attribute("vs_name").as_string();
        shaderTemplate->m_fsFilename = material.child("shader").attribute("fs_name").as_string();
        materialTemplate->m_shaderTemplate = shaderTemplate;
        
        pugi::xml_node textures_node = material.child("textures");
        for (pugi::xml_node texture = textures_node.child("texture"); texture; texture = texture.next_sibling("texture"))
        {
            std::shared_ptr<STextureTemplate> textureTemplate = std::make_shared<STextureTemplate>();
            textureTemplate->m_filename = texture.attribute("name").as_string();
            textureTemplate->m_slot = texture.attribute("slot").as_uint();
            
            std::string wrapStr = texture.attribute("wrap").as_string();
            assert(g_glEnumContainer.find(wrapStr) != g_glEnumContainer.end());
            textureTemplate->m_wrap = g_glEnumContainer.find(wrapStr)->second;
            materialTemplate->m_texturesTemplates.push_back(textureTemplate);
        }
        materialsTemplates.push_back(materialTemplate);
    }
    return materialsTemplates;
}
