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

class CTexture;

class CShaderUniform
{
private:
    
protected:
    
    E_UNIFORM_CLASS m_class;
    
    glm::mat4x4 m_mat4x4_value;
    glm::mat3x3 m_mat3x3_value;
    glm::vec4 m_vec4_value;
    glm::vec3 m_vec3_value;
    glm::vec2 m_vec2_value;
    f32 m_f32_value;
    i32 m_i32_value;
    
    E_SHADER_SAMPLER m_sampler_value;
    std::shared_ptr<CTexture> m_texture_value;
    
public:
    
    CShaderUniform(E_UNIFORM_CLASS _class);
    ~CShaderUniform(void);
    
    inline E_UNIFORM_CLASS Get_Class(void) const
    {
        return m_class;
    };
    
    inline void Set_Matrix3x3(const glm::mat3x3& _matrix)
    {
        assert(m_class == E_UNIFORM_CLASS_MAT3X3);
        m_mat3x3_value = _matrix;
    };
    inline void Set_Matrix4x4(const glm::mat4x4& _matrix)
    {
        assert(m_class == E_UNIFORM_CLASS_MAT4X4);
        m_mat4x4_value = _matrix;
    };

    inline void Set_Vector2(const glm::vec2& _vector)
    {
        assert(m_class == E_UNIFORM_CLASS_VECTOR2);
        m_vec2_value = _vector;
    };
    inline void Set_Vector3(const glm::vec3& _vector)
    {
        assert(m_class == E_UNIFORM_CLASS_VECTOR3);
        m_vec3_value = _vector;
    };
    inline void Set_Vector4(const glm::vec4& _vector)
    {
        assert(m_class == E_UNIFORM_CLASS_VECTOR4);
        m_vec4_value = _vector;
    };
    
    inline void Set_Float(f32 _value)
    {
        assert(m_class == E_UNIFORM_CLASS_FLOAT);
        m_f32_value = _value;
    };
    
    inline void Set_Int(i32 _value)
    {
        assert(m_class = E_UNIFORM_CLASS_INT);
        m_i32_value = _value;
    };
    
    inline void Set_Sampler(std::shared_ptr<CTexture> _texture, E_SHADER_SAMPLER _sampler)
    {
        assert(m_class == E_UNIFORM_CLASS_SAMPLER);
        m_sampler_value = _sampler;
        m_texture_value = _texture;
    };
    
    inline const glm::mat3x3& Get_Matrix3x3(void)
    {
        assert(m_class == E_UNIFORM_CLASS_MAT3X3);
        return m_mat3x3_value;
    };
    inline const glm::mat4x4& Get_Matrix4x4(void)
    {
        assert(m_class == E_UNIFORM_CLASS_MAT4X4);
        return m_mat4x4_value;
    };
    
    inline const glm::vec2& Get_Vector2(void)
    {
        assert(m_class == E_UNIFORM_CLASS_VECTOR2);
        return m_vec2_value;
    };
    inline const glm::vec3& Get_Vector3(void)
    {
        assert(m_class == E_UNIFORM_CLASS_VECTOR3);
        return m_vec3_value;
    };
    inline const glm::vec4& Get_Vector4(void)
    {
        assert(m_class == E_UNIFORM_CLASS_VECTOR4);
        return m_vec4_value;
    };
    
    inline f32 Get_Float(void)
    {
        assert(m_class == E_UNIFORM_CLASS_FLOAT);
        return m_f32_value;
    };
    
    inline i32 Get_Int(void)
    {
        assert(m_class == E_UNIFORM_CLASS_INT);
        return m_i32_value;
    };
    
    inline E_SHADER_SAMPLER Get_Sampler(void)
    {
        assert(m_class == E_UNIFORM_CLASS_SAMPLER);
        return m_sampler_value;
    };
    
    inline std::shared_ptr<CTexture> Get_Texture(void)
    {
        assert(m_class == E_UNIFORM_CLASS_SAMPLER);
        return m_texture_value;
    };
};

class I_RO_Shader
{
private:
    
protected:
    
public:
    
    I_RO_Shader(void) = default;
    ~I_RO_Shader(void) = default;
    
    virtual std::string Get_VertexShaderFilename(void) const = 0;
    virtual std::string Get_FragmentShaderFilename(void) const = 0;
    virtual std::string Get_VertexShaderSourceCode(void) const = 0;
    virtual std::string Get_FragmentShaderSourceCode(void) const = 0;
};

class I_WO_Shader
{
private:
    
protected:
    
public:
    
    I_WO_Shader(void) = default;
    ~I_WO_Shader(void) = default;
    
    virtual void Set_SourceCode(const std::string& _vsFilename,
                                const std::string& _vsSourceCode,
                                const std::string& _fsFilename,
                                const std::string& _fsSourceCode) = 0;
    virtual void Set_Handle(ui32 _handle) = 0;
    
};

class CShader :
public IResource,
I_RO_Shader,
I_WO_Shader
{
private:

protected:
    
    i32 m_uniforms[E_SHADER_UNIFORM_MAX];
    i32 m_samplers[E_SHADER_SAMPLER_MAX];
    i32 m_attributes[E_SHADER_ATTRIBUTE_MAX];
    std::array<std::shared_ptr<CShaderUniform>, E_SHADER_UNIFORM_MAX + E_SHADER_SAMPLER_MAX> m_values;
    
    std::string m_vsFilename;
    std::string m_fsFilename;
    std::string m_vsSourceCode;
    std::string m_fsSourceCode;
    ui32 m_handle;
    
public:
    
    CShader(const std::string& _guid);
    ~CShader(void);
    
    std::string Get_VertexShaderFilename(void) const;
    std::string Get_FragmentShaderFilename(void) const;
    std::string Get_VertexShaderSourceCode(void) const;
    std::string Get_FragmentShaderSourceCode(void) const;
    

    void Set_SourceCode(const std::string& _vsFilename,
                        const std::string& _vsSourceCode,
                        const std::string& _fsFilename,
                        const std::string& _fsSourceCode);
    void Set_Handle(ui32 _handle);
    
    const i32* Get_Attributes(void) const;
    
    void Set_Matrix3x3(const glm::mat3x3& _matrix, E_SHADER_UNIFORM _uniform);
    void Set_Matrix3x3Custom(const glm::mat3x3& _matrix, const std::string& _uniform);
    void Set_Matrix4x4(const glm::mat4x4& _matrix, E_SHADER_UNIFORM _uniform);
    void Set_Matrix4x4Custom(const glm::mat4x4& _matrix, const std::string& _uniform);
    void Set_MatrixArray4x4(const glm::mat4x4* _matrix, ui32 _size, E_SHADER_UNIFORM _uniform);
    void Set_MatrixArray4x4Custom(const glm::mat4x4* _matrix, ui32 _size, const std::string& _uniform);
    void Set_Vector2(const glm::vec2& _vector, E_SHADER_UNIFORM _uniform);
    void Set_Vector2Custom(const glm::vec2& _vector, const std::string& _uniform);
    void Set_Vector3(const glm::vec3& _vector, E_SHADER_UNIFORM _uniform);
    void Set_Vector3Custom(const glm::vec3& _vector, const std::string& _uniform);
    void Set_Vector4(const glm::vec4& _vector, E_SHADER_UNIFORM _uniform);
    void Set_Vector4Custom(const glm::vec4& _vector, const std::string& _uniform);
    void Set_Float(f32 _value, E_SHADER_UNIFORM _uniform);
    void Set_FloatCustom(f32 _value, const std::string& _uniform);
    void Set_Int(i32 _value, E_SHADER_UNIFORM _uniform);
    void Set_IntCustom(i32 _value, const std::string& _uniform);
    void Set_Texture(const std::shared_ptr<CTexture> _texture, E_SHADER_SAMPLER _sampler);
    
    void Bind(void) const;
    void Unbind(void) const;
};

#endif 
