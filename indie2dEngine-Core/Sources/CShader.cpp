//
//  CShader.cpp
//  gEngine-Core
//
//  Created by Sergey Sergeev on 5/7/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CShader.h"
#include "CTexture.h"

static ui32 s_handle = 0;

extern const struct SAttributes
{
    std::string m_position;
    std::string m_texcoord;
    std::string m_normal;
    std::string m_tangent;
    std::string m_color;
    std::string m_extra;
    
} SAttributes;

extern const struct SUniforms
{
    std::string m_worldMatrix;
    std::string m_viewMatrix;
    std::string m_projectionMatrix;
    std::string m_normalMatrix;
    std::string m_bonesMatrix;
    std::string m_cameraPosition;
    std::string m_lightPosition_01;
    std::string m_lightPosition_02;
    std::string m_lightPosition_03;
    std::string m_lightPosition_04;
    std::string m_lightColor_01;
    std::string m_lightColor_02;
    std::string m_lightColor_03;
    std::string m_lightColor_04;
    std::string m_clipPlane;
    std::string m_cameraNear;
    std::string m_cameraFar;
    std::string m_timer;
    std::string m_lightsCount;
    std::string m_flag01;
    std::string m_flag02;
    std::string m_flag03;
    std::string m_flag04;
    
} SUniforms;

extern const struct SSamplers
{
    std::string m_sampler_01;
    std::string m_sampler_02;
    std::string m_sampler_03;
    std::string m_sampler_04;
    std::string m_sampler_05;
    std::string m_sampler_06;
    std::string m_sampler_07;
    std::string m_sampler_08;
    
} SSamplers;

const struct SAttributes SAttributes =
{
    "IN_Position",
    "IN_TexCoord",
    "IN_Normal",
    "IN_Tangent",
    "IN_Color",
    "IN_Extra"
};

const struct SUniforms SUniforms =
{
    "MATRIX_World",
    "MATRIX_View",
    "MATRIX_Projection",
    "MATRIX_Normal",
    "MATRIX_Bones",
    "VECTOR_CameraPosition",
    "VECTOR_LightPosition_01",
    "VECTOR_LightPosition_02",
    "VECTOR_LightPosition_03",
    "VECTOR_LightPosition_04",
    "VECTOR_LightColor_01",
    "VECTOR_LightColor_02",
    "VECTOR_LightColor_03",
    "VECTOR_LightColor_04",
    "VECTOR_ClipPlane",
    "FLOAT_CameraNear",
    "FLOAT_CameraFar",
    "FLOAT_Timer",
    "INT_LightsCount",
    "INT_FLAG_01",
    "INT_FLAG_02",
    "INT_FLAG_03",
    "INT_FLAG_04"
};

const struct SSamplers SSamplers =
{
    "SAMPLER_01",
    "SAMPLER_02",
    "SAMPLER_03",
    "SAMPLER_04",
    "SAMPLER_05",
    "SAMPLER_06",
    "SAMPLER_07",
    "SAMPLER_08"
};

CShaderUniform::CShaderUniform(E_UNIFORM_CLASS _class) :
m_class(_class),
m_mat3x3_value(0.0f),
m_mat4x4_value(0.0f),
m_vec2_value(0.0f),
m_vec3_value(0.0f),
m_vec4_value(0.0f),
m_f32_value(0.0f),
m_sampler_value(E_SHADER_SAMPLER_01),
m_texture_value(nullptr)
{
    
}

CShaderUniform::~CShaderUniform(void)
{
    
}

CShader::CShader(const std::string& _guid) :
IResource(E_RESOURCE_CLASS_SHADER, _guid),
m_handle(0)
{
    m_attributes[E_SHADER_ATTRIBUTE_POSITION] = -1;
    m_attributes[E_SHADER_ATTRIBUTE_TEXCOORD] = -1;
    m_attributes[E_SHADER_ATTRIBUTE_NORMAL] = -1;
    m_attributes[E_SHADER_ATTRIBUTE_TANGENT] = -1;
    m_attributes[E_SHADER_ATTRIBUTE_COLOR] = -1;
    m_attributes[E_SHADER_ATTRIBUTE_EXTRA] = -1;
}

CShader::~CShader(void)
{
    
}

std::string CShader::Get_VertexShaderFilename(void) const
{
    assert(m_vsFilename.length() != 0);
    return m_vsFilename;
}

std::string CShader::Get_FragmentShaderFilename(void) const
{
    assert(m_fsFilename.length() != 0);
    return m_fsFilename;
}

std::string CShader::Get_VertexShaderSourceCode(void) const
{
    return m_vsSourceCode;
}

std::string CShader::Get_FragmentShaderSourceCode(void) const
{
    return m_fsSourceCode;
}

void CShader::Set_SourceCode(const std::string &_vsFilename,
                             const std::string &_vsSourceCode,
                             const std::string &_fsFilename,
                             const std::string &_fsSourceCode)
{
    m_vsFilename = _vsFilename;
    m_fsFilename = _fsFilename;
    m_vsSourceCode = _vsSourceCode;
    m_fsSourceCode = _fsSourceCode;
    m_status |= E_RESOURCE_STATUS_LOADED;
}

void CShader::Set_Handle(ui32 _handle)
{
    m_handle = _handle;
    
    for(ui32 i = 0; i < E_SHADER_UNIFORM_MAX + E_SHADER_SAMPLER_MAX; ++i)
    {
        m_values[i] = nullptr;
    }
    
    m_uniforms[E_SHADER_UNIFORM_MATRIX_WORLD] = glGetUniformLocation(m_handle, SUniforms.m_worldMatrix.c_str());
    m_uniforms[E_SHADER_UNIFORM_MATRIX_VIEW] = glGetUniformLocation(m_handle, SUniforms.m_viewMatrix.c_str());
    m_uniforms[E_SHADER_UNIFORM_MATRIX_PROJECTION] = glGetUniformLocation(m_handle, SUniforms.m_projectionMatrix.c_str());
    m_uniforms[E_SHADER_UNIFORM_MATRIX_NORMAL] = glGetUniformLocation(m_handle, SUniforms.m_normalMatrix.c_str());
    m_uniforms[E_SHADER_UNIFORM_MATRIX_BONES] = glGetUniformLocation(m_handle, SUniforms.m_bonesMatrix.c_str());
    m_uniforms[E_SHADER_UNIFORM_VECTOR_CAMERA_POSITION] = glGetUniformLocation(m_handle, SUniforms.m_cameraPosition.c_str());
    m_uniforms[E_SHADER_UNIFORM_VECTOR_LIGHT_01_POSITION] = glGetUniformLocation(m_handle, SUniforms.m_lightPosition_01.c_str());
    m_uniforms[E_SHADER_UNIFORM_VECTOR_LIGHT_02_POSITION] = glGetUniformLocation(m_handle, SUniforms.m_lightPosition_02.c_str());
    m_uniforms[E_SHADER_UNIFORM_VECTOR_LIGHT_03_POSITION] = glGetUniformLocation(m_handle, SUniforms.m_lightPosition_03.c_str());
    m_uniforms[E_SHADER_UNIFORM_VECTOR_LIGHT_04_POSITION] = glGetUniformLocation(m_handle, SUniforms.m_lightPosition_04.c_str());
    m_uniforms[E_SHADER_UNIFORM_VECTOR_LIGHT_01_COLOR] = glGetUniformLocation(m_handle, SUniforms.m_lightColor_01.c_str());
    m_uniforms[E_SHADER_UNIFORM_VECTOR_LIGHT_02_COLOR] = glGetUniformLocation(m_handle, SUniforms.m_lightColor_02.c_str());
    m_uniforms[E_SHADER_UNIFORM_VECTOR_LIGHT_03_COLOR] = glGetUniformLocation(m_handle, SUniforms.m_lightColor_03.c_str());
    m_uniforms[E_SHADER_UNIFORM_VECTOR_LIGHT_04_COLOR] = glGetUniformLocation(m_handle, SUniforms.m_lightColor_04.c_str());
    m_uniforms[E_SHADER_UNIFORM_VECTOR_CLIP_PLANE] = glGetUniformLocation(m_handle, SUniforms.m_clipPlane.c_str());
    m_uniforms[E_SHADER_UNIFORM_FLOAT_CAMERA_NEAR] = glGetUniformLocation(m_handle, SUniforms.m_cameraNear.c_str());
    m_uniforms[E_SHADER_UNIFORM_FLOAT_CAMERA_FAR] = glGetUniformLocation(m_handle, SUniforms.m_cameraFar.c_str());
    m_uniforms[E_SHADER_UNIFORM_FLOAT_TIMER] = glGetUniformLocation(m_handle, SUniforms.m_timer.c_str());
    m_uniforms[E_SHADER_UNIFORM_INT_LIGHTS_COUNT] = glGetUniformLocation(m_handle, SUniforms.m_lightsCount.c_str());
    
    m_uniforms[E_SHADER_UNIFORM_INT_FLAG_01] = glGetUniformLocation(m_handle, SUniforms.m_flag01.c_str());
    m_uniforms[E_SHADER_UNIFORM_INT_FLAG_02] = glGetUniformLocation(m_handle, SUniforms.m_flag02.c_str());
    m_uniforms[E_SHADER_UNIFORM_INT_FLAG_03] = glGetUniformLocation(m_handle, SUniforms.m_flag03.c_str());
    m_uniforms[E_SHADER_UNIFORM_INT_FLAG_04] = glGetUniformLocation(m_handle, SUniforms.m_flag04.c_str());
    
    m_samplers[E_SHADER_SAMPLER_01] = glGetUniformLocation(m_handle, SSamplers.m_sampler_01.c_str());
    m_samplers[E_SHADER_SAMPLER_02] = glGetUniformLocation(m_handle, SSamplers.m_sampler_02.c_str());
    m_samplers[E_SHADER_SAMPLER_03] = glGetUniformLocation(m_handle, SSamplers.m_sampler_03.c_str());
    m_samplers[E_SHADER_SAMPLER_04] = glGetUniformLocation(m_handle, SSamplers.m_sampler_04.c_str());
    m_samplers[E_SHADER_SAMPLER_05] = glGetUniformLocation(m_handle, SSamplers.m_sampler_05.c_str());
    m_samplers[E_SHADER_SAMPLER_06] = glGetUniformLocation(m_handle, SSamplers.m_sampler_06.c_str());
    m_samplers[E_SHADER_SAMPLER_07] = glGetUniformLocation(m_handle, SSamplers.m_sampler_07.c_str());
    m_samplers[E_SHADER_SAMPLER_08] = glGetUniformLocation(m_handle, SSamplers.m_sampler_08.c_str());
    
    m_attributes[E_SHADER_ATTRIBUTE_POSITION] = glGetAttribLocation(m_handle, SAttributes.m_position.c_str());
    m_attributes[E_SHADER_ATTRIBUTE_TEXCOORD] = glGetAttribLocation(m_handle, SAttributes.m_texcoord.c_str());
    m_attributes[E_SHADER_ATTRIBUTE_NORMAL] = glGetAttribLocation(m_handle, SAttributes.m_normal.c_str());
    m_attributes[E_SHADER_ATTRIBUTE_TANGENT] = glGetAttribLocation(m_handle, SAttributes.m_tangent.c_str());
    m_attributes[E_SHADER_ATTRIBUTE_COLOR] = glGetAttribLocation(m_handle, SAttributes.m_color.c_str());
    m_attributes[E_SHADER_ATTRIBUTE_EXTRA] = glGetAttribLocation(m_handle, SAttributes.m_extra.c_str());
    
    m_status |= E_RESOURCE_STATUS_COMMITED;
}

const i32* CShader::Get_Attributes(void) const
{
    return m_attributes;
}

void CShader::Set_Matrix3x3(const glm::mat3x3 &_matrix, E_SHADER_UNIFORM _uniform)
{
    if(IResource::IsLoaded() && IResource::IsCommited())
    {
        if(m_values[_uniform] != nullptr && m_values[_uniform]->Get_Matrix3x3() == _matrix)
        {
            return;
        }
        else if(m_values[_uniform] == nullptr)
        {
            m_values[_uniform] = std::make_shared<CShaderUniform>(E_UNIFORM_CLASS_MAT3X3);
        }
        
        i32 handle = m_uniforms[_uniform];
        glUniformMatrix3fv(handle, 1, 0, &_matrix[0][0]);
        m_values[_uniform]->Set_Matrix3x3(_matrix);
    }
}

void CShader::Set_Matrix3x3Custom(const glm::mat3x3 &_matrix, const std::string &_uniform)
{
    if(IResource::IsLoaded() && IResource::IsCommited())
    {
        i32 handle = glGetUniformLocation(m_handle, _uniform.c_str());
        glUniformMatrix3fv(handle, 1, 0, &_matrix[0][0]);
    }
}

void CShader::Set_Matrix4x4(const glm::mat4x4 &_matrix, E_SHADER_UNIFORM _uniform)
{
    if(IResource::IsLoaded() && IResource::IsCommited())
    {
        if(m_values[_uniform] != nullptr && m_values[_uniform]->Get_Matrix4x4() == _matrix)
        {
            return;
        }
        else if(m_values[_uniform] == nullptr)
        {
            m_values[_uniform] = std::make_shared<CShaderUniform>(E_UNIFORM_CLASS_MAT4X4);
        }

        i32 handle = m_uniforms[_uniform];
        glUniformMatrix4fv(handle, 1, 0, &_matrix[0][0]);
        m_values[_uniform]->Set_Matrix4x4(_matrix);
    }
}

void CShader::Set_Matrix4x4Custom(const glm::mat4x4 &_matrix, const std::string &_uniform)
{
    if(IResource::IsLoaded() && IResource::IsCommited())
    {
        i32 handle = glGetUniformLocation(m_handle, _uniform.c_str());
        glUniformMatrix4fv(handle, 1, 0, &_matrix[0][0]);
    }
}

void CShader::Set_MatrixArray4x4(const glm::mat4x4* _matrix, ui32 _size, E_SHADER_UNIFORM _uniform)
{
    if(IResource::IsLoaded() && IResource::IsCommited())
    {
        i32 handle = m_uniforms[_uniform];
        glUniformMatrix4fv(handle, _size, 0, &_matrix[0][0][0]);
    }
}

void CShader::Set_MatrixArray4x4Custom(const glm::mat4x4* _matrix, ui32 _size, const std::string& _uniform)
{
    if(IResource::IsLoaded() && IResource::IsCommited())
    {
        i32 handle = glGetUniformLocation(m_handle, _uniform.c_str());
        glUniformMatrix4fv(handle, _size, 0, &_matrix[0][0][0]);
    }
}

void CShader::Set_Vector2(const glm::vec2 &_vector, E_SHADER_UNIFORM _uniform)
{
    if(IResource::IsLoaded() && IResource::IsCommited())
    {
        if(m_values[_uniform] != nullptr && m_values[_uniform]->Get_Vector2() == _vector)
        {
            return;
        }
        else if(m_values[_uniform] == nullptr)
        {
            m_values[_uniform] = std::make_shared<CShaderUniform>(E_UNIFORM_CLASS_VECTOR2);
        }
        
        i32 handle = m_uniforms[_uniform];
        glUniform2fv(handle, 1, &_vector[0]);
        m_values[_uniform]->Set_Vector2(_vector);
    }
}

void CShader::Set_Vector2Custom(const glm::vec2 &_vector, const std::string &_uniform)
{
    if(IResource::IsLoaded() && IResource::IsCommited())
    {
        i32 handle = glGetUniformLocation(m_handle, _uniform.c_str());
        glUniform2fv(handle, 1, &_vector[0]);
    }
}

void CShader::Set_Vector3(const glm::vec3 &_vector, E_SHADER_UNIFORM _uniform)
{
    if(IResource::IsLoaded() && IResource::IsCommited())
    {
        if(m_values[_uniform] != nullptr && m_values[_uniform]->Get_Vector3() == _vector)
        {
            return;
        }
        else if(m_values[_uniform] == nullptr)
        {
            m_values[_uniform] = std::make_shared<CShaderUniform>(E_UNIFORM_CLASS_VECTOR3);
        }
        
        i32 handle = m_uniforms[_uniform];
        glUniform3fv(handle, 1, &_vector[0]);
        m_values[_uniform]->Set_Vector3(_vector);
    }
}

void CShader::Set_Vector3Custom(const glm::vec3 &_vector, const std::string &_uniform)
{
    if(IResource::IsLoaded() && IResource::IsCommited())
    {
        i32 handle = glGetUniformLocation(m_handle, _uniform.c_str());
        glUniform3fv(handle, 1, &_vector[0]);
    }
}

void CShader::Set_Vector4(const glm::vec4 &_vector, E_SHADER_UNIFORM _uniform)
{
    if(IResource::IsLoaded() && IResource::IsCommited())
    {
        if(m_values[_uniform] != nullptr && m_values[_uniform]->Get_Vector4() == _vector)
        {
            return;
        }
        else if(m_values[_uniform] == nullptr)
        {
            m_values[_uniform] = std::make_shared<CShaderUniform>(E_UNIFORM_CLASS_VECTOR4);
        }

        i32 handle = m_uniforms[_uniform];
        glUniform4fv(handle, 1, &_vector[0]);
        m_values[_uniform]->Set_Vector4(_vector);
    }
}

void CShader::Set_Vector4Custom(const glm::vec4 &_vector, const std::string &_uniform)
{
    if(IResource::IsLoaded() && IResource::IsCommited())
    {
        i32 handle = glGetUniformLocation(m_handle, _uniform.c_str());
        glUniform4fv(handle, 1, &_vector[0]);
    }
}

void CShader::Set_Float(f32 _value, E_SHADER_UNIFORM _uniform)
{
    if(IResource::IsLoaded() && IResource::IsCommited())
    {
        if(m_values[_uniform] != nullptr && m_values[_uniform]->Get_Float() == _value)
        {
            return;
        }
        else if(m_values[_uniform] == nullptr)
        {
            m_values[_uniform] = std::make_shared<CShaderUniform>(E_UNIFORM_CLASS_FLOAT);
        }

        i32 handle = m_uniforms[_uniform];
        glUniform1f(handle, _value);
        m_values[_uniform]->Set_Float(_value);
    }
}

void CShader::Set_FloatCustom(f32 _value, const std::string &_uniform)
{
    if(IResource::IsLoaded() && IResource::IsCommited())
    {
        i32 handle = glGetUniformLocation(m_handle, _uniform.c_str());
        glUniform1f(handle, _value);
    }
}

void CShader::Set_Int(i32 _value, E_SHADER_UNIFORM _uniform)
{
    if(IResource::IsLoaded() && IResource::IsCommited())
    {
        if(m_values[_uniform] != nullptr && m_values[_uniform]->Get_Int() == _value)
        {
            return;
        }
        else if(m_values[_uniform] == nullptr)
        {
            m_values[_uniform] = std::make_shared<CShaderUniform>(E_UNIFORM_CLASS_INT);
        }
        
        i32 handle = m_uniforms[_uniform];
        glUniform1i(handle, _value);
        m_values[_uniform]->Set_Int(_value);
    }
}

void CShader::Set_IntCustom(i32 _value, const std::string &_uniform)
{
    if(IResource::IsLoaded() && IResource::IsCommited())
    {
        i32 handle = glGetUniformLocation(m_handle, _uniform.c_str());
        glUniform1i(handle, _value);
    }
}

void CShader::Set_Texture(std::shared_ptr<CTexture> _texture, E_SHADER_SAMPLER _sampler)
{
    if(IResource::IsLoaded() && IResource::IsCommited())
    {
        assert(_sampler < E_SHADER_SAMPLER_MAX);
        glActiveTexture(GL_TEXTURE0 + _sampler);
        _texture->Bind();
        glUniform1i(m_samplers[_sampler], _sampler);
    }
}

void CShader::Bind(void) const
{
    if(IResource::IsLoaded() && IResource::IsCommited() && s_handle != m_handle)
    {
        s_handle = m_handle;
        glUseProgram(m_handle);
    }
}

void CShader::Unbind(void) const
{
    /*if(IResource::IsLoaded() && IResource::IsCommited())
    {
        glUseProgram(NULL);
    }*/
}

