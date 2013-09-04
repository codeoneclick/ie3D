//
//  CShader.cpp
//  gEngine-Core
//
//  Created by Sergey Sergeev on 5/7/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CShader.h"
#include "CTexture.h"

static ui32 s_handle = NULL;

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
    std::string m_bonesMatrix;
    std::string m_cameraPosition;
    std::string m_lightPosition;
    std::string m_clipPlane;
    std::string m_cameraNear;
    std::string m_cameraFar;
    
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
    "MATRIX_Bones",
    "VECTOR_CameraPosition",
    "VECTOR_LightPosition",
    "VECTOR_ClipPlane",
    "FLOAT_CameraNear",
    "FLOAT_CameraFar"
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
    m_values.resize(E_SHADER_UNIFORM_MAX + E_SHADER_SAMPLER_MAX);
    for(ui32 i = 0; i < E_SHADER_UNIFORM_MAX + E_SHADER_SAMPLER_MAX; ++i)
    {
        m_values[i] = nullptr;
    }
}

CShader::~CShader(void)
{
    m_values.clear();
    m_values.resize(0);
}

void CShader::_Set_Handle(ui32 _handle)
{
    m_handle = _handle;
    
    m_uniforms[E_SHADER_UNIFORM_MATRIX_WORLD] = glGetUniformLocation(m_handle, SUniforms.m_worldMatrix.c_str());
    m_uniforms[E_SHADER_UNIFORM_MATRIX_VIEW] = glGetUniformLocation(m_handle, SUniforms.m_viewMatrix.c_str());
    m_uniforms[E_SHADER_UNIFORM_MATRIX_PROJECTION] = glGetUniformLocation(m_handle, SUniforms.m_projectionMatrix.c_str());
    m_uniforms[E_SHADER_UNIFORM_MATRIX_BONES] = glGetUniformLocation(m_handle, SUniforms.m_bonesMatrix.c_str());
    m_uniforms[E_SHADER_UNIFORM_VECTOR_CAMERA_POSITION] = glGetUniformLocation(m_handle, SUniforms.m_cameraPosition.c_str());
    m_uniforms[E_SHADER_UNIFORM_VECTOR_LIGHT_POSITION] = glGetUniformLocation(m_handle, SUniforms.m_lightPosition.c_str());
    m_uniforms[E_SHADER_UNIFORM_VECTOR_CLIP_PLANE] = glGetUniformLocation(m_handle, SUniforms.m_clipPlane.c_str());
    m_uniforms[E_SHADER_UNIFORM_FLOAT_CAMERA_NEAR] = glGetUniformLocation(m_handle, SUniforms.m_cameraNear.c_str());
    m_uniforms[E_SHADER_UNIFORM_FLOAT_CAMERA_FAR] = glGetUniformLocation(m_handle, SUniforms.m_cameraFar.c_str());
    
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

void CShader::Set_Texture(std::shared_ptr<CTexture> _texture, E_SHADER_SAMPLER _sampler)
{
    if(IResource::IsLoaded() && IResource::IsCommited())
    {
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

