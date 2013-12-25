//
//  CMaterialTemplateSerializer.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/29/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CMaterialTemplateSerializer.h"
#include "CTemplateGameObjects.h"


CMaterialTemplateSerializer::CMaterialTemplateSerializer(void)
{
    
}

CMaterialTemplateSerializer::~CMaterialTemplateSerializer(void)
{
    
}

std::shared_ptr<I_RO_TemplateCommon> CMaterialTemplateSerializer::Serialize(const std::string& _filename)
{
    pugi::xml_document document;
    pugi::xml_parse_result result = ITemplateSerializer::_LoadDocument(document, _filename);
    assert(result.status == pugi::status_ok);
    
    std::shared_ptr<CMaterialTemplate> materialTemplate = std::make_shared<CMaterialTemplate>();
    
    pugi::xml_node node = document.child(materialTemplate->kMaterialMainNode.c_str());
    
    std::string renderOperationName = node.attribute(materialTemplate->kMaterialRenderOperationNameAttribute.c_str()).as_string();
    materialTemplate->Set_Attribute(Get_TemplateAttributeKey(materialTemplate->kMaterialMainNode,
                                                             materialTemplate->kMaterialRenderOperationNameAttribute),
                                    E_TEMPLATE_META_TYPE_STRING,
                                    &renderOperationName);
    
    bool isCullFace = node.attribute(materialTemplate->kMaterialIsCullFaceAttribute.c_str()).as_bool();
    materialTemplate->Set_Attribute(Get_TemplateAttributeKey(materialTemplate->kMaterialMainNode,
                                                             materialTemplate->kMaterialIsCullFaceAttribute),
                                    E_TEMPLATE_META_TYPE_BOOL,
                                    &isCullFace);
    
    bool isDepthTest = node.attribute(materialTemplate->kMaterialIsDepthTestAttribute.c_str()).as_bool();
    materialTemplate->Set_Attribute(Get_TemplateAttributeKey(materialTemplate->kMaterialMainNode,
                                                             materialTemplate->kMaterialIsDepthTestAttribute),
                                    E_TEMPLATE_META_TYPE_BOOL,
                                    &isDepthTest);
    
    bool isDepthMask = node.attribute(materialTemplate->kMaterialIsDepthMaskAttribute.c_str()).as_bool();
    materialTemplate->Set_Attribute(Get_TemplateAttributeKey(materialTemplate->kMaterialMainNode,
                                                             materialTemplate->kMaterialIsDepthMaskAttribute),
                                    E_TEMPLATE_META_TYPE_BOOL,
                                    &isDepthMask);
    
    bool isBlending = node.attribute(materialTemplate->kMaterialIsBlendingAttribute.c_str()).as_bool();
    materialTemplate->Set_Attribute(Get_TemplateAttributeKey(materialTemplate->kMaterialMainNode,
                                                             materialTemplate->kMaterialIsBlendingAttribute),
                                    E_TEMPLATE_META_TYPE_BOOL,
                                    &isBlending);
    
    std::string cullFaceModeStr = node.attribute(materialTemplate->kMaterialCullFaceModeAttribute.c_str()).as_string();
    assert(g_stringToGLenum.find(cullFaceModeStr) != g_stringToGLenum.end());
    ui32 cullFaceMode = g_stringToGLenum.find(cullFaceModeStr)->second;
    materialTemplate->Set_Attribute(Get_TemplateAttributeKey(materialTemplate->kMaterialMainNode,
                                                             materialTemplate->kMaterialCullFaceModeAttribute),
                                    E_TEMPLATE_META_TYPE_UI32,
                                    &cullFaceMode);
    
    std::string blendingFunctionSourceStr = node.attribute(materialTemplate->kMaterialBlendingFunctionSourceAttribute.c_str()).as_string();
    assert(g_stringToGLenum.find(blendingFunctionSourceStr) != g_stringToGLenum.end());
    ui32 blendingFunctionSource = g_stringToGLenum.find(blendingFunctionSourceStr)->second;
    materialTemplate->Set_Attribute(Get_TemplateAttributeKey(materialTemplate->kMaterialMainNode,
                                                             materialTemplate->kMaterialBlendingFunctionSourceAttribute),
                                    E_TEMPLATE_META_TYPE_UI32,
                                    &blendingFunctionSource);
    
    std::string blendingFunctionDestinationStr = node.attribute(materialTemplate->kMaterialBlendingFunctionDestinationAttribute.c_str()).as_string();
    assert(g_stringToGLenum.find(blendingFunctionDestinationStr) != g_stringToGLenum.end());
    ui32 blendingFunctionDestination = g_stringToGLenum.find(blendingFunctionDestinationStr)->second;
    materialTemplate->Set_Attribute(Get_TemplateAttributeKey(materialTemplate->kMaterialMainNode,
                                                             materialTemplate->kMaterialBlendingFunctionDestinationAttribute),
                                    E_TEMPLATE_META_TYPE_UI32,
                                    &blendingFunctionDestination);
    
    bool isClipping = node.attribute(materialTemplate->kMaterialIsClippingAttribute.c_str()).as_bool();
    materialTemplate->Set_Attribute(Get_TemplateAttributeKey(materialTemplate->kMaterialMainNode,
                                                             materialTemplate->kMaterialIsClippingAttribute),
                                    E_TEMPLATE_META_TYPE_BOOL,
                                    &isClipping);

    f32 clippingX = node.attribute(materialTemplate->kMaterialClippingXAttribute.c_str()).as_float();
    materialTemplate->Set_Attribute(Get_TemplateAttributeKey(materialTemplate->kMaterialMainNode,
                                                             materialTemplate->kMaterialClippingXAttribute),
                                    E_TEMPLATE_META_TYPE_F32,
                                    &clippingX);
    
    f32 clippingY = node.attribute(materialTemplate->kMaterialClippingYAttribute.c_str()).as_float();
    materialTemplate->Set_Attribute(Get_TemplateAttributeKey(materialTemplate->kMaterialMainNode,
                                                             materialTemplate->kMaterialClippingYAttribute),
                                    E_TEMPLATE_META_TYPE_F32,
                                    &clippingY);
    
    f32 clippingZ = node.attribute(materialTemplate->kMaterialClippingZAttribute.c_str()).as_float();
    materialTemplate->Set_Attribute(Get_TemplateAttributeKey(materialTemplate->kMaterialMainNode,
                                                             materialTemplate->kMaterialClippingZAttribute),
                                    E_TEMPLATE_META_TYPE_F32,
                                    &clippingZ);
    
    f32 clippingW = node.attribute(materialTemplate->kMaterialClippingWAttribute.c_str()).as_float();
    materialTemplate->Set_Attribute(Get_TemplateAttributeKey(materialTemplate->kMaterialMainNode,
                                                             materialTemplate->kMaterialClippingWAttribute),
                                    E_TEMPLATE_META_TYPE_F32,
                                    &clippingW);
    
    bool isReflecting = node.attribute(materialTemplate->kMaterialIsReflectingAttribute.c_str()).as_bool();
    materialTemplate->Set_Attribute(Get_TemplateAttributeKey(materialTemplate->kMaterialMainNode,
                                                             materialTemplate->kMaterialIsReflectingAttribute),
                                    E_TEMPLATE_META_TYPE_BOOL,
                                    &isReflecting);

    bool isShadowing = node.attribute(materialTemplate->kMaterialIsShadowingAttribute.c_str()).as_bool();
    materialTemplate->Set_Attribute(Get_TemplateAttributeKey(materialTemplate->kMaterialMainNode,
                                                             materialTemplate->kMaterialIsShadowingAttribute),
                                    E_TEMPLATE_META_TYPE_BOOL,
                                    &isShadowing);
    
    bool isDebugging = node.attribute(materialTemplate->kMaterialIsDebuggingAttribute.c_str()).as_bool();
    materialTemplate->Set_Attribute(Get_TemplateAttributeKey(materialTemplate->kMaterialMainNode,
                                                             materialTemplate->kMaterialIsDebuggingAttribute),
                                    E_TEMPLATE_META_TYPE_BOOL,
                                    &isDebugging);
    
    
    std::shared_ptr<CShaderTemplate> shaderTemplate = std::make_shared<CShaderTemplate>();
    std::string vsFilename = node.child(materialTemplate->kMaterialShaderTemplateNode.c_str()).attribute(shaderTemplate->kShaderVSFilenameAttribute.c_str()).as_string();
    shaderTemplate->Set_Attribute(Get_TemplateAttributeKey(shaderTemplate->kShaderMainNode,
                                                           shaderTemplate->kShaderVSFilenameAttribute),
                                  E_TEMPLATE_META_TYPE_STRING,
                                  &vsFilename);
    
    std::string fsFilename = node.child(materialTemplate->kMaterialShaderTemplateNode.c_str()).attribute(shaderTemplate->kShaderFSFilenameAttribute.c_str()).as_string();
    shaderTemplate->Set_Attribute(Get_TemplateAttributeKey(shaderTemplate->kShaderMainNode,
                                                           shaderTemplate->kShaderFSFilenameAttribute),
                                  E_TEMPLATE_META_TYPE_STRING,
                                  &fsFilename);
    
    materialTemplate->Set_Template(Get_TemplateAttributeKey(materialTemplate->kMaterialMainNode,
                                                            materialTemplate->kMaterialShaderTemplateNode),
                                   shaderTemplate);
    
    pugi::xml_node texturesNode = node.child(materialTemplate->kMaterialTexturesTemplatesNode.c_str());
    for (pugi::xml_node texture = texturesNode.child(materialTemplate->kMaterialTextureTemplateNode.c_str());
         texture;
         texture = texture.next_sibling(materialTemplate->kMaterialTextureTemplateNode.c_str()))
    {
        std::shared_ptr<CTextureTemplate> textureTemplate = std::make_shared<CTextureTemplate>();
        
        std::string filename = texture.attribute(textureTemplate->kTextureFilenameAttribute.c_str()).as_string();
        textureTemplate->Set_Attribute(Get_TemplateAttributeKey(textureTemplate->kTextureMainNode,
                                                                textureTemplate->kTextureFilenameAttribute),
                                       E_TEMPLATE_META_TYPE_STRING,
                                       &filename);
        
        std::string renderOperationName = texture.attribute(textureTemplate->kTextureRenderOperationNameAttribute.c_str()).as_string();
        textureTemplate->Set_Attribute(Get_TemplateAttributeKey(textureTemplate->kTextureMainNode,
                                                                textureTemplate->kTextureRenderOperationNameAttribute),
                                       E_TEMPLATE_META_TYPE_STRING,
                                       &renderOperationName);
        
        ui32 samplerId = texture.attribute(textureTemplate->kTextureSamplerIdAttribute.c_str()).as_uint();
        textureTemplate->Set_Attribute(Get_TemplateAttributeKey(textureTemplate->kTextureMainNode,
                                                                textureTemplate->kTextureSamplerIdAttribute),
                                       E_TEMPLATE_META_TYPE_UI32,
                                       &samplerId);
        
        std::string textureWrapModeStr = texture.attribute(textureTemplate->kTextureWrapModeAttribute.c_str()).as_string();
        assert(g_stringToGLenum.find(textureWrapModeStr) != g_stringToGLenum.end());
        ui32 textureWrapMode = g_stringToGLenum.find(textureWrapModeStr)->second;
        textureTemplate->Set_Attribute(Get_TemplateAttributeKey(textureTemplate->kTextureMainNode,
                                                                textureTemplate->kTextureWrapModeAttribute),
                                       E_TEMPLATE_META_TYPE_UI32,
                                       &textureWrapMode);
        
        materialTemplate->Set_Template(Get_TemplateAttributeKey(materialTemplate->kMaterialMainNode,
                                                                materialTemplate->kMaterialTexturesTemplatesNode),
                                       textureTemplate);
    }

    return materialTemplate;
}

std::shared_ptr<I_RO_TemplateCommon> CMaterialTemplateSerializer::Serialize(const std::string& _host, ui32 _port, const std::string& _filename)
{
    return nullptr;
}

void CMaterialTemplateSerializer::Deserialize(const std::string& _filename, std::shared_ptr<I_RO_TemplateCommon> _template)
{
    
}
