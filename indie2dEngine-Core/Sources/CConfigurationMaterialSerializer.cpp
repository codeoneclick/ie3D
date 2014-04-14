//
//  CConfigurationMaterialSerializer.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/29/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CConfigurationMaterialSerializer.h"
#include "CConfigurationGameObjects.h"

CConfigurationMaterialSerializer::CConfigurationMaterialSerializer(void)
{
    
}

CConfigurationMaterialSerializer::~CConfigurationMaterialSerializer(void)
{
    
}

ISharedConfiguration CConfigurationMaterialSerializer::serialize(const std::string& filename)
{
    pugi::xml_document document;
    pugi::xml_parse_result result = IConfigurationSerializer::openXMLDocument(document, filename);
    assert(result.status == pugi::status_ok);
    
    std::shared_ptr<CConfigurationMaterial> materialConfiguration = std::make_shared<CConfigurationMaterial>();
    
    pugi::xml_node node = document.child(materialConfiguration->kMaterialMainNode.c_str());
    
    std::string renderOperationName = node.attribute(materialConfiguration->kMaterialRenderOperationNameAttribute.c_str()).as_string();
    materialConfiguration->setAttribute(getConfigurationAttributeKey(materialConfiguration->kMaterialMainNode,
                                                                materialConfiguration->kMaterialRenderOperationNameAttribute),
                                   std::make_shared<CConfigurationAttribute>(renderOperationName));
    
    bool isCullFace = node.attribute(materialConfiguration->kMaterialIsCullFaceAttribute.c_str()).as_bool();
    materialConfiguration->setAttribute(getConfigurationAttributeKey(materialConfiguration->kMaterialMainNode,
                                                             materialConfiguration->kMaterialIsCullFaceAttribute),
                                    std::make_shared<CConfigurationAttribute>(isCullFace));
    
    bool isDepthTest = node.attribute(materialConfiguration->kMaterialIsDepthTestAttribute.c_str()).as_bool();
    materialConfiguration->setAttribute(getConfigurationAttributeKey(materialConfiguration->kMaterialMainNode,
                                                             materialConfiguration->kMaterialIsDepthTestAttribute),
                                    std::make_shared<CConfigurationAttribute>(isDepthTest));
    
    bool isDepthMask = node.attribute(materialConfiguration->kMaterialIsDepthMaskAttribute.c_str()).as_bool();
    materialConfiguration->setAttribute(getConfigurationAttributeKey(materialConfiguration->kMaterialMainNode,
                                                             materialConfiguration->kMaterialIsDepthMaskAttribute),
                                    std::make_shared<CConfigurationAttribute>(isDepthMask));
    
    bool isBlending = node.attribute(materialConfiguration->kMaterialIsBlendingAttribute.c_str()).as_bool();
    materialConfiguration->setAttribute(getConfigurationAttributeKey(materialConfiguration->kMaterialMainNode,
                                                             materialConfiguration->kMaterialIsBlendingAttribute),
                                    std::make_shared<CConfigurationAttribute>(isBlending));
    
    std::string cullFaceModeStr = node.attribute(materialConfiguration->kMaterialCullFaceModeAttribute.c_str()).as_string();
    assert(g_stringToGLenum.find(cullFaceModeStr) != g_stringToGLenum.end());
    ui32 cullFaceMode = g_stringToGLenum.find(cullFaceModeStr)->second;
    materialConfiguration->setAttribute(getConfigurationAttributeKey(materialConfiguration->kMaterialMainNode,
                                                             materialConfiguration->kMaterialCullFaceModeAttribute),
                                    std::make_shared<CConfigurationAttribute>(cullFaceMode));
    
    std::string blendingFunctionSourceStr = node.attribute(materialConfiguration->kMaterialBlendingFunctionSourceAttribute.c_str()).as_string();
    assert(g_stringToGLenum.find(blendingFunctionSourceStr) != g_stringToGLenum.end());
    ui32 blendingFunctionSource = g_stringToGLenum.find(blendingFunctionSourceStr)->second;
    materialConfiguration->setAttribute(getConfigurationAttributeKey(materialConfiguration->kMaterialMainNode,
                                                             materialConfiguration->kMaterialBlendingFunctionSourceAttribute),
                                    std::make_shared<CConfigurationAttribute>(blendingFunctionSource));
    
    std::string blendingFunctionDestinationStr = node.attribute(materialConfiguration->kMaterialBlendingFunctionDestinationAttribute.c_str()).as_string();
    assert(g_stringToGLenum.find(blendingFunctionDestinationStr) != g_stringToGLenum.end());
    ui32 blendingFunctionDestination = g_stringToGLenum.find(blendingFunctionDestinationStr)->second;
    materialConfiguration->setAttribute(getConfigurationAttributeKey(materialConfiguration->kMaterialMainNode,
                                                             materialConfiguration->kMaterialBlendingFunctionDestinationAttribute),
                                    std::make_shared<CConfigurationAttribute>(blendingFunctionDestination));
    
    bool isClipping = node.attribute(materialConfiguration->kMaterialIsClippingAttribute.c_str()).as_bool();
    materialConfiguration->setAttribute(getConfigurationAttributeKey(materialConfiguration->kMaterialMainNode,
                                                             materialConfiguration->kMaterialIsClippingAttribute),
                                    std::make_shared<CConfigurationAttribute>(isClipping));

    f32 clippingX = node.attribute(materialConfiguration->kMaterialClippingXAttribute.c_str()).as_float();
    materialConfiguration->setAttribute(getConfigurationAttributeKey(materialConfiguration->kMaterialMainNode,
                                                             materialConfiguration->kMaterialClippingXAttribute),
                                    std::make_shared<CConfigurationAttribute>(clippingX));
    
    f32 clippingY = node.attribute(materialConfiguration->kMaterialClippingYAttribute.c_str()).as_float();
    materialConfiguration->setAttribute(getConfigurationAttributeKey(materialConfiguration->kMaterialMainNode,
                                                             materialConfiguration->kMaterialClippingYAttribute),
                                    std::make_shared<CConfigurationAttribute>(clippingY));
    
    f32 clippingZ = node.attribute(materialConfiguration->kMaterialClippingZAttribute.c_str()).as_float();
    materialConfiguration->setAttribute(getConfigurationAttributeKey(materialConfiguration->kMaterialMainNode,
                                                             materialConfiguration->kMaterialClippingZAttribute),
                                    std::make_shared<CConfigurationAttribute>(clippingZ));
    
    f32 clippingW = node.attribute(materialConfiguration->kMaterialClippingWAttribute.c_str()).as_float();
    materialConfiguration->setAttribute(getConfigurationAttributeKey(materialConfiguration->kMaterialMainNode,
                                                             materialConfiguration->kMaterialClippingWAttribute),
                                    std::make_shared<CConfigurationAttribute>(clippingW));
    
    bool isReflecting = node.attribute(materialConfiguration->kMaterialIsReflectingAttribute.c_str()).as_bool();
    materialConfiguration->setAttribute(getConfigurationAttributeKey(materialConfiguration->kMaterialMainNode,
                                                             materialConfiguration->kMaterialIsReflectingAttribute),
                                    std::make_shared<CConfigurationAttribute>(isReflecting));

    bool isShadowing = node.attribute(materialConfiguration->kMaterialIsShadowingAttribute.c_str()).as_bool();
    materialConfiguration->setAttribute(getConfigurationAttributeKey(materialConfiguration->kMaterialMainNode,
                                                             materialConfiguration->kMaterialIsShadowingAttribute),
                                    std::make_shared<CConfigurationAttribute>(isShadowing));
    
    bool isDebugging = node.attribute(materialConfiguration->kMaterialIsDebuggingAttribute.c_str()).as_bool();
    materialConfiguration->setAttribute(getConfigurationAttributeKey(materialConfiguration->kMaterialMainNode,
                                                             materialConfiguration->kMaterialIsDebuggingAttribute),
                                    std::make_shared<CConfigurationAttribute>(isDebugging));
    
    
    std::shared_ptr<CConfigurationShader> shaderConfiguration = std::make_shared<CConfigurationShader>();
    std::string vsFilename = node.child(materialConfiguration->kMaterialShaderConfigurationNode.c_str()).attribute(shaderConfiguration->kShaderVSFilenameAttribute.c_str()).as_string();
    shaderConfiguration->setAttribute(getConfigurationAttributeKey(shaderConfiguration->kShaderMainNode,
                                                           shaderConfiguration->kShaderVSFilenameAttribute),
                                  std::make_shared<CConfigurationAttribute>(vsFilename));
    
    std::string fsFilename = node.child(materialConfiguration->kMaterialShaderConfigurationNode.c_str()).attribute(shaderConfiguration->kShaderFSFilenameAttribute.c_str()).as_string();
    shaderConfiguration->setAttribute(getConfigurationAttributeKey(shaderConfiguration->kShaderMainNode,
                                                           shaderConfiguration->kShaderFSFilenameAttribute),
                                  std::make_shared<CConfigurationAttribute>(fsFilename));
    
    materialConfiguration->setConfiguration(getConfigurationAttributeKey(materialConfiguration->kMaterialMainNode,
                                                            materialConfiguration->kMaterialShaderConfigurationNode),
                                   shaderConfiguration);
    
    pugi::xml_node texturesNode = node.child(materialConfiguration->kMaterialTexturesConfigurationsNode.c_str());
    for (pugi::xml_node texture = texturesNode.child(materialConfiguration->kMaterialTextureConfigurationNode.c_str());
         texture;
         texture = texture.next_sibling(materialConfiguration->kMaterialTextureConfigurationNode.c_str()))
    {
        std::shared_ptr<CConfigurationTexture> textureConfiguration = std::make_shared<CConfigurationTexture>();
        
        std::string filename = texture.attribute(textureConfiguration->kTextureFilenameAttribute.c_str()).as_string();
        textureConfiguration->setAttribute(getConfigurationAttributeKey(textureConfiguration->kTextureMainNode,
                                                                textureConfiguration->kTextureFilenameAttribute),
                                       std::make_shared<CConfigurationAttribute>(filename));
        
        std::string renderOperationName = texture.attribute(textureConfiguration->kTextureRenderOperationNameAttribute.c_str()).as_string();
        textureConfiguration->setAttribute(getConfigurationAttributeKey(textureConfiguration->kTextureMainNode,
                                                                textureConfiguration->kTextureRenderOperationNameAttribute),
                                       std::make_shared<CConfigurationAttribute>(renderOperationName));
        
        ui32 samplerIndex = texture.attribute(textureConfiguration->kTextureSamplerIndexAttribute.c_str()).as_uint();
        textureConfiguration->setAttribute(getConfigurationAttributeKey(textureConfiguration->kTextureMainNode,
                                                                textureConfiguration->kTextureSamplerIndexAttribute),
                                       std::make_shared<CConfigurationAttribute>(samplerIndex));
        
        std::string textureWrapModeStr = texture.attribute(textureConfiguration->kTextureWrapModeAttribute.c_str()).as_string();
        assert(g_stringToGLenum.find(textureWrapModeStr) != g_stringToGLenum.end());
        ui32 textureWrapMode = g_stringToGLenum.find(textureWrapModeStr)->second;
        textureConfiguration->setAttribute(getConfigurationAttributeKey(textureConfiguration->kTextureMainNode,
                                                                textureConfiguration->kTextureWrapModeAttribute),
                                       std::make_shared<CConfigurationAttribute>(textureWrapMode));
        
        materialConfiguration->setConfiguration(getConfigurationAttributeKey(materialConfiguration->kMaterialMainNode,
                                                                materialConfiguration->kMaterialTexturesConfigurationsNode),
                                       textureConfiguration);
    }

    return materialConfiguration;
}

void CConfigurationMaterialSerializer::deserialize(const std::string& filename, ISharedConfigurationRef configuration)
{
    
}
