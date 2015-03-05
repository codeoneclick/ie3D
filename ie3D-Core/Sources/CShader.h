//
//  CShader.h
//  gEngine-Core
//
//  Created by Sergey Sergeev on 5/7/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CShader_h
#define CShader_h

#include "IResource.h"
#include "HDeclaration.h"

class CShaderUniform
{
private:
    
protected:
    
    E_UNIFORM_CLASS m_uniformClass;
    
    glm::mat4x4 m_mat4x4_value;
    glm::mat3x3 m_mat3x3_value;
    glm::vec4 m_vec4_value;
    glm::vec3 m_vec3_value;
    glm::vec2 m_vec2_value;
    f32 m_f32_value;
    i32 m_i32_value;
    
    E_SHADER_SAMPLER m_sampler_value;
    CSharedTexture m_texture_value;
    
public:
    
    CShaderUniform(E_UNIFORM_CLASS uniformClass);
    
    ~CShaderUniform(void);
    
    E_UNIFORM_CLASS getClass(void) const;
    
    void setMatrix3x3(const glm::mat3x3& matrix);
    void setMatrix4x4(const glm::mat4x4& matrix);
    void setVector2(const glm::vec2& vector);
    void setVector3(const glm::vec3& vector);
    void setVector4(const glm::vec4& vector);
    void setFloat(f32 value);
    void setInt(i32 value);
    void setSampler(CSharedTextureRef texture, E_SHADER_SAMPLER sampler);
    
    const glm::mat3x3& getMatrix3x3(void) const;
    const glm::mat4x4& getMatrix4x4(void) const;
    const glm::vec2& getVector2(void) const;
    const glm::vec3& getVector3(void) const;
    const glm::vec4& getVector4(void) const;
    f32 getFloat(void) const;
    i32 getInt(void) const;
    E_SHADER_SAMPLER getSampler(void) const;
    CSharedTexture getTexture(void) const;
};

#define k_MAX_CACHED_UNIFORM_VALUES 512

class CShaderData : public IResourceData
{
private:
    
protected:
    
    std::string m_vsFilename;
    std::string m_fsFilename;
    std::string m_vsSourceCode;
    std::string m_fsSourceCode;
    ui32 m_shaderId;
    
public:
    
    CShaderData(const std::string& vsFilename,
                const std::string& fsFilename,
                const std::string& vsSourceCode,
                const std::string& fsSourceCode);
    
    CShaderData(ui32 shaderId);
    
    ~CShaderData(void);
    
    std::string getVSFilename(void) const;
    std::string getFSFilename(void) const;
    std::string getVSSourceCode(void) const;
    std::string getFSSourceCode(void) const;
    
    ui32 getShaderId(void) const;
};

class CShader : public IResource
{
public:
    
    enum E_ATTRIBUTE
    {
        E_ATTRIBUTE_POSITION_3F = 0,
        E_ATTRIBUTE_POSITION_2I16,
        E_ATTRIBUTE_HEIGHT_F32,
        E_ATTRIBUTE_TEXCOORD_UI32,
        E_ATTRIBUTE_NORMAL_UI32,
        E_ATTRIBUTE_TANGENT_UI32,
        E_ATTRIBUTE_COLOR_4UI8,
        E_ATTRIBUTE_EXTRA_4UI8,
        E_ATTRIBUTE_MAX
    };

private:

protected:
    
    i32 m_uniforms[E_SHADER_UNIFORM_MAX];
    i32 m_samplers[E_SHADER_SAMPLER_MAX];
    std::array<i32, E_SHADER_ATTRIBUTE_MAX> m_attributes;
    std::unordered_map<std::string, i32> m_customUniforms;
    
    CSharedShaderData m_shaderData;
    ui32 m_shaderId;
    
    std::array<CSharedShaderUniform, k_MAX_CACHED_UNIFORM_VALUES> m_cachedUniformValues;
    
    void onResourceDataSerializationFinished(ISharedResourceDataRef resourceData);
    
    void onResourceDataCommitFinished(ISharedResourceDataRef resourceData);
    
    void setupUniforms(void);
    i32 getCustomUniform(const std::string& uniform);
    
public:
    
    CShader(const std::string& guid);
    
    static CSharedShader constructCustomShader(const std::string& guid,
                                               const std::string& vsSourceCode,
                                               const std::string& fsSourceCode);
    
    ~CShader(void);
    
    std::string getVSFilename(void) const;
    std::string getFSFilename(void) const;
    std::string getVSSourceCode(void) const;
    std::string getFSSourceCode(void) const;

    const std::array<i32, E_SHADER_ATTRIBUTE_MAX>& getAttributes(void) const;
    
    void setMatrix3x3(const glm::mat3x3& matrix, E_SHADER_UNIFORM uniform);
    void setMatrix3x3Custom(const glm::mat3x3& matrix, const std::string& uniform);
    void setMatrix4x4(const glm::mat4x4& matrix, E_SHADER_UNIFORM uniform);
    void setMatrix4x4Custom(const glm::mat4x4& matrix, const std::string& uniform);
    void setMatrixArray4x4(const glm::mat4x4* matrix, ui32 size, E_SHADER_UNIFORM uniform);
    void setMatrixArray4x4Custom(const glm::mat4x4* matrices, ui32 size, const std::string& uniform);
    void setVector2(const glm::vec2& vector, E_SHADER_UNIFORM uniform);
    void setVector2Custom(const glm::vec2& vector, const std::string& uniform);
    void setVector2ArrayCustom(const glm::vec2* vectors, ui32 size, const std::string& uniform);
    void setVector3(const glm::vec3& vector, E_SHADER_UNIFORM uniform);
    void setVector3Custom(const glm::vec3& vector, const std::string& uniform);
    void setVector3ArrayCustom(const glm::vec3* vectors, ui32 size, const std::string& uniform);
    void setVector4(const glm::vec4& vector, E_SHADER_UNIFORM uniform);
    void setVector4Custom(const glm::vec4& vector, const std::string& uniform);
    void setFloat(f32 value, E_SHADER_UNIFORM uniform);
    void setFloatCustom(f32 value, const std::string& uniform);
    void setInt(i32 value, E_SHADER_UNIFORM uniform);
    void setIntCustom(i32 value, const std::string& uniform);
    void setTexture(CSharedTextureRef texture, E_SHADER_SAMPLER sampler);
    
    void bind(void) const;
    void unbind(void) const;
};

#endif 
