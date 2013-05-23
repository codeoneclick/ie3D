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

class CShader final : public IResource 
{
private:

protected:
    
    friend class CShaderSerializer_GLSL;
    friend class CShaderCommiter_GLSL;
    friend class CShaderLoadingOperation;
    
    i32 m_uniforms[E_SHADER_UNIFORM_MAX];
    i32 m_samplers[E_SHADER_SAMPLER_MAX];
    i32 m_attributes[E_SHADER_ATTRIBUTE_MAX];
    
    std::string m_vsSourceCode;
    std::string m_fsSourceCode;
    ui32 m_handle;
    
    inline void _Set_SourceCode(const std::string& _vsSourceCode, const std::string& _fsSourceCode)
    {
        m_vsSourceCode = _vsSourceCode;
        m_fsSourceCode = _fsSourceCode;
        m_isLoaded = true;
    };

#ifdef TESTING
public:
#endif
    
    inline std::string _Get_VertexShaderSourceCode(void)
    {
        return m_vsSourceCode;
    };
    
    inline std::string _Get_FragmentShaderSourceCode(void)
    {
        return m_fsSourceCode;
    };
    
#ifdef TESTING
protected:
#endif
    
    void _Set_Handle(ui32 _handle);
    
public:
    
    CShader(const std::string& _guid);
    ~CShader(void);
    
    inline const i32* Get_Attributes(void)
    {
        return m_attributes;
    };
    
    void Set_Matrix3x3(const glm::mat3x3& _matrix, E_SHADER_UNIFORM _uniform);
    void Set_Matrix3x3Custom(const glm::mat3x3& _matrix, const std::string& _uniform);
    void Set_Matrix4x4(const glm::mat4x4& _matrix, E_SHADER_UNIFORM _uniform);
    void Set_Matrix4x4Custom(const glm::mat4x4& _matrix, const std::string& _uniform);
    void Set_Vector2(const glm::vec2& _vector, E_SHADER_UNIFORM _uniform);
    void Set_Vector2Custom(const glm::vec2& _vector, const std::string& _uniform);
    void Set_Vector3(const glm::vec3& _vector, E_SHADER_UNIFORM _uniform);
    void Set_Vector3Custom(const glm::vec3& _vector, const std::string& _uniform);
    void Set_Vector4(const glm::vec4& _vector, E_SHADER_UNIFORM _uniform);
    void Set_Vector4Custom(const glm::vec4& _vector, const std::string& _uniform);
    void Set_Float(f32 _value, E_SHADER_UNIFORM _uniform);
    void Set_FloatCustom(f32 _value, const std::string& _uniform);
    void Set_Texture(const std::shared_ptr<CTexture> _texture, E_SHADER_SAMPLER _sampler);
    
    void Bind(void);
    void Unbind(void);
};

#endif 
