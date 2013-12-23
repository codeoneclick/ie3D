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
    const std::string kTextureShaderSamplerIdAttribute;
    const std::string kTextureWrapModeAttribute;
    
private:
    
protected:
    
public:
    
    CTemplateTexture(void);
    ~CTemplateTexture(void);
    
    std::string Get_RenderOperationName(void);
    ui32 Get_ShaderSampleId(void);
    ui32 Get_TextureWrapMode(void);
};

class CTemplateShader : public CTemplateCommon
{
public:
    
    const std::string kShaderMainNode;
    const std::string kShaderVSFilename;
    const std::string kShaderFSFilename;

private:
    
protected:
    
public:
    
    CTemplateShader(void);
    ~CTemplateShader(void);
    
    std::string Get_VSFilename(void);
    std::string Get_FSFilename(void);
};

class CTemplateMaterial : public CTemplateCommon
{
public:
    
    const std::string kMaterialMainNode;
    const std::string kMaterialRenderOperationName;
    const std::string kMaterialIsCullFace;
    const std::string kMaterialCullFaceMode;
    const std::string kMaterialIsBlending;
    const std::string kMaterialBlendingFunctionSource;
    const std::string kMaterialBlendingFunctionDestination;
    const std::string kMaterialIsDepthTest;
    const std::string kMaterialIsDepthMask;
    const std::string kMaterialIsClipping;
    const std::string kMaterialClippingX;
    const std::string kMaterialClippingY;
    const std::string kMaterialClippingZ;
    const std::string kMaterialClippingW;
    const std::string kMaterialIsReflecting;
    const std::string kMaterialIsShadowing;
    const std::string kMaterialIsDebugging;
    
private:
    
protected:
    
public:
    
    CTemplateMaterial(void);
    ~CTemplateMaterial(void);
    
    std::string Get_RenderOperationName(void);
    bool Get_IsCullFace(void);
    ui32 Get_CullFaceMode(void);
    bool Get_IsBlending(void);
    ui32 Get_BlendingFunctionSource(void);
    ui32 Get_BlendingFunctionDestination(void);
    bool Get_IsDepthTest(void);
    bool Get_IsDepthMask(void);
    bool Get_IsClipping(void);
    glm::vec4 Get_ClippingPlane(void);
    bool Get_IsReflecting(void);
    bool Get_IsShadowing(void);
    bool Get_IsDebugging(void);
    
    std::vector<std::shared_ptr<CTemplateTexture>> Get_TexturesTemplates(void);
    std::shared_ptr<CTemplateShader> Get_ShaderTemplates(void);
};


#endif
