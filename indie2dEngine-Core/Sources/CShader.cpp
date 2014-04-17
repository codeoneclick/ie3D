//
//  CShader.cpp
//  gEngine-Core
//
//  Created by Sergey Sergeev on 5/7/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CShader.h"
#include "CTexture.h"

static ui32 g_shaderId = 0;

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
m_uniformClass(_class),
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

E_UNIFORM_CLASS CShaderUniform::getClass(void) const
{
    return m_uniformClass;
}

void CShaderUniform::setMatrix3x3(const glm::mat3x3& matrix)
{
    assert(m_uniformClass == E_UNIFORM_CLASS_MAT3X3);
    m_mat3x3_value = matrix;
}

void CShaderUniform::setMatrix4x4(const glm::mat4x4& matrix)
{
    assert(m_uniformClass == E_UNIFORM_CLASS_MAT4X4);
    m_mat4x4_value = matrix;
}

void CShaderUniform::setVector2(const glm::vec2& vector)
{
    assert(m_uniformClass == E_UNIFORM_CLASS_VECTOR2);
    m_vec2_value = vector;
}

void CShaderUniform::setVector3(const glm::vec3& vector)
{
    assert(m_uniformClass == E_UNIFORM_CLASS_VECTOR3);
    m_vec3_value = vector;
}

void CShaderUniform::setVector4(const glm::vec4& vector)
{
    assert(m_uniformClass == E_UNIFORM_CLASS_VECTOR4);
    m_vec4_value = vector;
}

void CShaderUniform::setFloat(f32 value)
{
    assert(m_uniformClass == E_UNIFORM_CLASS_FLOAT);
    m_f32_value = value;
}

void CShaderUniform::setInt(i32 value)
{
    assert(m_uniformClass == E_UNIFORM_CLASS_INT);
    m_i32_value = value;
}

void CShaderUniform::setSampler(CSharedTextureRef texture, E_SHADER_SAMPLER sampler)
{
    assert(m_uniformClass == E_UNIFORM_CLASS_SAMPLER);
    m_texture_value = texture;
    m_sampler_value = sampler;
}

const glm::mat3x3& CShaderUniform::getMatrix3x3(void) const
{
    assert(m_uniformClass == E_UNIFORM_CLASS_MAT3X3);
    return m_mat3x3_value;
}

const glm::mat4x4& CShaderUniform::getMatrix4x4(void) const
{
    assert(m_uniformClass == E_UNIFORM_CLASS_MAT4X4);
    return m_mat4x4_value;
}

const glm::vec2& CShaderUniform::getVector2(void) const
{
    assert(m_uniformClass == E_UNIFORM_CLASS_VECTOR2);
    return m_vec2_value;
}

const glm::vec3& CShaderUniform::getVector3(void) const
{
    assert(m_uniformClass == E_UNIFORM_CLASS_VECTOR3);
    return m_vec3_value;
}

const glm::vec4& CShaderUniform::getVector4(void) const
{
    assert(m_uniformClass == E_UNIFORM_CLASS_VECTOR4);
    return m_vec4_value;
}

f32 CShaderUniform::getFloat(void) const
{
    assert(m_uniformClass == E_UNIFORM_CLASS_FLOAT);
    return m_f32_value;
}

i32 CShaderUniform::getInt(void) const
{
    assert(m_uniformClass == E_UNIFORM_CLASS_INT);
    return m_i32_value;
}

E_SHADER_SAMPLER CShaderUniform::getSampler(void) const
{
    assert(m_uniformClass == E_UNIFORM_CLASS_SAMPLER);
    return m_sampler_value;
}

CSharedTexture CShaderUniform::getTexture(void) const
{
    assert(m_uniformClass == E_UNIFORM_CLASS_SAMPLER);
    return m_texture_value;
}


CShaderData::CShaderData(const std::string& vsFilename,
                         const std::string& fsFilename,
                         const std::string& vsSourceCode,
                         const std::string& fsSourceCode) : IResourceData(E_RESOURCE_DATA_CLASS_SHADER_DATA),
m_shaderId(0),
m_vsFilename(vsFilename),
m_fsFilename(fsFilename),
m_vsSourceCode(vsSourceCode),
m_fsSourceCode(fsSourceCode)
{
    
}

CShaderData::CShaderData(ui32 shaderId) : IResourceData(E_RESOURCE_DATA_CLASS_SHADER_DATA),
m_shaderId(shaderId)
{
    
}

CShaderData::~CShaderData(void)
{
    
}

std::string CShaderData::getVSFilename(void) const
{
    return m_vsFilename;
}

std::string CShaderData::getFSFilename(void) const
{
    return m_fsFilename;
}

std::string CShaderData::getVSSourceCode(void) const
{
    return m_vsSourceCode;
}

std::string CShaderData::getFSSourceCode(void) const
{
    return m_fsSourceCode;
}

ui32 CShaderData::getShaderId(void) const
{
    return m_shaderId;
}

CShader::CShader(const std::string& guid) : IResource(E_RESOURCE_CLASS_SHADER, guid),
m_shaderId(0)
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
    glDeleteProgram(m_shaderId);
}

void CShader::onResourceDataSerializationStatusChanged(ISharedResourceDataRef resourceData,
                                                       E_RESOURCE_DATA_STATUS status)
{
    if(status == E_RESOURCE_DATA_STATUS_STARTED)
    {
        
    } else if(status == E_RESOURCE_DATA_STATUS_PROGRESS) {
        assert(resourceData != nullptr);
        
        switch(resourceData->getResourceDataClass())
        {
            case E_RESOURCE_DATA_CLASS_SHADER_DATA:
            {
                m_shaderData = std::static_pointer_cast<CShaderData>(resourceData);
            }
                break;
            default:
            {
                assert(false);
            }
                break;
        }
        
    } else if(status == E_RESOURCE_DATA_STATUS_FINISHED) {
        m_status |= E_RESOURCE_STATUS_LOADED;
    }
}

void CShader::onResourceDataCommitStatusChanged(ISharedResourceDataRef resourceData,
                                                E_RESOURCE_DATA_STATUS status)
{
    if(status == E_RESOURCE_DATA_STATUS_STARTED)
    {
        
    } else if(status == E_RESOURCE_DATA_STATUS_PROGRESS) {
        assert(resourceData != nullptr);
        
        switch(resourceData->getResourceDataClass())
        {
            case E_RESOURCE_DATA_CLASS_SHADER_DATA:
            {
                CSharedShaderData shaderData = std::static_pointer_cast<CShaderData>(resourceData);
                m_shaderId = shaderData->getShaderId();
                assert(m_shaderId != 0);
                CShader::setupUniforms();
            }
                break;
            default:
            {
                assert(false);
            }
                break;
        }
        
    } else if(status == E_RESOURCE_DATA_STATUS_FINISHED) {
        m_status |= E_RESOURCE_STATUS_COMMITED;
    }
}

void CShader::setupUniforms(void)
{
    m_uniforms[E_SHADER_UNIFORM_MATRIX_WORLD] = glGetUniformLocation(m_shaderId, SUniforms.m_worldMatrix.c_str());
    m_uniforms[E_SHADER_UNIFORM_MATRIX_VIEW] = glGetUniformLocation(m_shaderId, SUniforms.m_viewMatrix.c_str());
    m_uniforms[E_SHADER_UNIFORM_MATRIX_PROJECTION] = glGetUniformLocation(m_shaderId, SUniforms.m_projectionMatrix.c_str());
    m_uniforms[E_SHADER_UNIFORM_MATRIX_NORMAL] = glGetUniformLocation(m_shaderId, SUniforms.m_normalMatrix.c_str());
    m_uniforms[E_SHADER_UNIFORM_MATRIX_BONES] = glGetUniformLocation(m_shaderId, SUniforms.m_bonesMatrix.c_str());
    m_uniforms[E_SHADER_UNIFORM_VECTOR_CAMERA_POSITION] = glGetUniformLocation(m_shaderId, SUniforms.m_cameraPosition.c_str());
    m_uniforms[E_SHADER_UNIFORM_VECTOR_LIGHT_01_POSITION] = glGetUniformLocation(m_shaderId, SUniforms.m_lightPosition_01.c_str());
    m_uniforms[E_SHADER_UNIFORM_VECTOR_LIGHT_02_POSITION] = glGetUniformLocation(m_shaderId, SUniforms.m_lightPosition_02.c_str());
    m_uniforms[E_SHADER_UNIFORM_VECTOR_LIGHT_03_POSITION] = glGetUniformLocation(m_shaderId, SUniforms.m_lightPosition_03.c_str());
    m_uniforms[E_SHADER_UNIFORM_VECTOR_LIGHT_04_POSITION] = glGetUniformLocation(m_shaderId, SUniforms.m_lightPosition_04.c_str());
    m_uniforms[E_SHADER_UNIFORM_VECTOR_LIGHT_01_COLOR] = glGetUniformLocation(m_shaderId, SUniforms.m_lightColor_01.c_str());
    m_uniforms[E_SHADER_UNIFORM_VECTOR_LIGHT_02_COLOR] = glGetUniformLocation(m_shaderId, SUniforms.m_lightColor_02.c_str());
    m_uniforms[E_SHADER_UNIFORM_VECTOR_LIGHT_03_COLOR] = glGetUniformLocation(m_shaderId, SUniforms.m_lightColor_03.c_str());
    m_uniforms[E_SHADER_UNIFORM_VECTOR_LIGHT_04_COLOR] = glGetUniformLocation(m_shaderId, SUniforms.m_lightColor_04.c_str());
    m_uniforms[E_SHADER_UNIFORM_VECTOR_CLIP_PLANE] = glGetUniformLocation(m_shaderId, SUniforms.m_clipPlane.c_str());
    m_uniforms[E_SHADER_UNIFORM_FLOAT_CAMERA_NEAR] = glGetUniformLocation(m_shaderId, SUniforms.m_cameraNear.c_str());
    m_uniforms[E_SHADER_UNIFORM_FLOAT_CAMERA_FAR] = glGetUniformLocation(m_shaderId, SUniforms.m_cameraFar.c_str());
    m_uniforms[E_SHADER_UNIFORM_FLOAT_TIMER] = glGetUniformLocation(m_shaderId, SUniforms.m_timer.c_str());
    m_uniforms[E_SHADER_UNIFORM_INT_LIGHTS_COUNT] = glGetUniformLocation(m_shaderId, SUniforms.m_lightsCount.c_str());
    
    m_uniforms[E_SHADER_UNIFORM_INT_FLAG_01] = glGetUniformLocation(m_shaderId, SUniforms.m_flag01.c_str());
    m_uniforms[E_SHADER_UNIFORM_INT_FLAG_02] = glGetUniformLocation(m_shaderId, SUniforms.m_flag02.c_str());
    m_uniforms[E_SHADER_UNIFORM_INT_FLAG_03] = glGetUniformLocation(m_shaderId, SUniforms.m_flag03.c_str());
    m_uniforms[E_SHADER_UNIFORM_INT_FLAG_04] = glGetUniformLocation(m_shaderId, SUniforms.m_flag04.c_str());
    
    m_samplers[E_SHADER_SAMPLER_01] = glGetUniformLocation(m_shaderId, SSamplers.m_sampler_01.c_str());
    m_samplers[E_SHADER_SAMPLER_02] = glGetUniformLocation(m_shaderId, SSamplers.m_sampler_02.c_str());
    m_samplers[E_SHADER_SAMPLER_03] = glGetUniformLocation(m_shaderId, SSamplers.m_sampler_03.c_str());
    m_samplers[E_SHADER_SAMPLER_04] = glGetUniformLocation(m_shaderId, SSamplers.m_sampler_04.c_str());
    m_samplers[E_SHADER_SAMPLER_05] = glGetUniformLocation(m_shaderId, SSamplers.m_sampler_05.c_str());
    m_samplers[E_SHADER_SAMPLER_06] = glGetUniformLocation(m_shaderId, SSamplers.m_sampler_06.c_str());
    m_samplers[E_SHADER_SAMPLER_07] = glGetUniformLocation(m_shaderId, SSamplers.m_sampler_07.c_str());
    m_samplers[E_SHADER_SAMPLER_08] = glGetUniformLocation(m_shaderId, SSamplers.m_sampler_08.c_str());
    
    m_attributes[E_SHADER_ATTRIBUTE_POSITION] = glGetAttribLocation(m_shaderId, SAttributes.m_position.c_str());
    m_attributes[E_SHADER_ATTRIBUTE_TEXCOORD] = glGetAttribLocation(m_shaderId, SAttributes.m_texcoord.c_str());
    m_attributes[E_SHADER_ATTRIBUTE_NORMAL] = glGetAttribLocation(m_shaderId, SAttributes.m_normal.c_str());
    m_attributes[E_SHADER_ATTRIBUTE_TANGENT] = glGetAttribLocation(m_shaderId, SAttributes.m_tangent.c_str());
    m_attributes[E_SHADER_ATTRIBUTE_COLOR] = glGetAttribLocation(m_shaderId, SAttributes.m_color.c_str());
    m_attributes[E_SHADER_ATTRIBUTE_EXTRA] = glGetAttribLocation(m_shaderId, SAttributes.m_extra.c_str());
    
    for(ui32 i = 0; i < E_SHADER_UNIFORM_MAX + E_SHADER_SAMPLER_MAX; ++i)
    {
        m_values[i] = nullptr;
    }
}

std::string CShader::getVSFilename(void) const
{
    return IResource::isLoaded() ? m_shaderData->getVSFilename() : "";
}

std::string CShader::getFSFilename(void) const
{
    return IResource::isLoaded() ? m_shaderData->getFSFilename() : "";
}

std::string CShader::getVSSourceCode(void) const
{
    return IResource::isLoaded() ? m_shaderData->getVSSourceCode() : "";
}

std::string CShader::getFSSourceCode(void) const
{
    return IResource::isLoaded() ? m_shaderData->getFSSourceCode() : "";
}

const i32* CShader::getAttributesRef(void) const
{
    return m_attributes;
}

void CShader::setMatrix3x3(const glm::mat3x3 &matrix, E_SHADER_UNIFORM uniform)
{
    if(IResource::isLoaded() && IResource::isCommited())
    {
        if(m_values[uniform] != nullptr && m_values[uniform]->getMatrix3x3() == matrix)
        {
            return;
        }
        else if(m_values[uniform] == nullptr)
        {
            m_values[uniform] = std::make_shared<CShaderUniform>(E_UNIFORM_CLASS_MAT3X3);
        }
        
        i32 handle = m_uniforms[uniform];
        glUniformMatrix3fv(handle, 1, 0, &matrix[0][0]);
        m_values[uniform]->setMatrix3x3(matrix);
    }
}

void CShader::setMatrix3x3Custom(const glm::mat3x3 &matrix, const std::string &uniform)
{
    if(IResource::isLoaded() && IResource::isCommited())
    {
        i32 handle = glGetUniformLocation(m_shaderId, uniform.c_str());
        glUniformMatrix3fv(handle, 1, 0, &matrix[0][0]);
    }
}

void CShader::setMatrix4x4(const glm::mat4x4 &matrix, E_SHADER_UNIFORM uniform)
{
    if(IResource::isLoaded() && IResource::isCommited())
    {
        if(m_values[uniform] != nullptr && m_values[uniform]->getMatrix4x4() == matrix)
        {
            return;
        }
        else if(m_values[uniform] == nullptr)
        {
            m_values[uniform] = std::make_shared<CShaderUniform>(E_UNIFORM_CLASS_MAT4X4);
        }

        i32 handle = m_uniforms[uniform];
        glUniformMatrix4fv(handle, 1, 0, &matrix[0][0]);
        m_values[uniform]->setMatrix4x4(matrix);
    }
}

void CShader::setMatrix4x4Custom(const glm::mat4x4 &matrix, const std::string &uniform)
{
    if(IResource::isLoaded() && IResource::isCommited())
    {
        i32 handle = glGetUniformLocation(m_shaderId, uniform.c_str());
        glUniformMatrix4fv(handle, 1, 0, &matrix[0][0]);
    }
}

void CShader::setMatrixArray4x4(const glm::mat4x4* matrix, ui32 size, E_SHADER_UNIFORM uniform)
{
    if(IResource::isLoaded() && IResource::isCommited())
    {
        i32 handle = m_uniforms[uniform];
        glUniformMatrix4fv(handle, size, 0, &matrix[0][0][0]);
    }
}

void CShader::setMatrixArray4x4Custom(const glm::mat4x4* matrix, ui32 size, const std::string& uniform)
{
    if(IResource::isLoaded() && IResource::isCommited())
    {
        i32 handle = glGetUniformLocation(m_shaderId, uniform.c_str());
        glUniformMatrix4fv(handle, size, 0, &matrix[0][0][0]);
    }
}

void CShader::setVector2(const glm::vec2 &vector, E_SHADER_UNIFORM uniform)
{
    if(IResource::isLoaded() && IResource::isCommited())
    {
        if(m_values[uniform] != nullptr && m_values[uniform]->getVector2() == vector)
        {
            return;
        }
        else if(m_values[uniform] == nullptr)
        {
            m_values[uniform] = std::make_shared<CShaderUniform>(E_UNIFORM_CLASS_VECTOR2);
        }
        
        i32 handle = m_uniforms[uniform];
        glUniform2fv(handle, 1, &vector[0]);
        m_values[uniform]->setVector2(vector);
    }
}

void CShader::setVector2Custom(const glm::vec2 &vector, const std::string &uniform)
{
    if(IResource::isLoaded() && IResource::isCommited())
    {
        i32 handle = glGetUniformLocation(m_shaderId, uniform.c_str());
        glUniform2fv(handle, 1, &vector[0]);
    }
}

void CShader::setVector3(const glm::vec3 &vector, E_SHADER_UNIFORM uniform)
{
    if(IResource::isLoaded() && IResource::isCommited())
    {
        if(m_values[uniform] != nullptr && m_values[uniform]->getVector3() == vector)
        {
            return;
        }
        else if(m_values[uniform] == nullptr)
        {
            m_values[uniform] = std::make_shared<CShaderUniform>(E_UNIFORM_CLASS_VECTOR3);
        }
        
        i32 handle = m_uniforms[uniform];
        glUniform3fv(handle, 1, &vector[0]);
        m_values[uniform]->setVector3(vector);
    }
}

void CShader::setVector3Custom(const glm::vec3 &vector, const std::string &uniform)
{
    if(IResource::isLoaded() && IResource::isCommited())
    {
        i32 handle = glGetUniformLocation(m_shaderId, uniform.c_str());
        glUniform3fv(handle, 1, &vector[0]);
    }
}

void CShader::setVector4(const glm::vec4 &vector, E_SHADER_UNIFORM uniform)
{
    if(IResource::isLoaded() && IResource::isCommited())
    {
        if(m_values[uniform] != nullptr && m_values[uniform]->getVector4() == vector)
        {
            return;
        }
        else if(m_values[uniform] == nullptr)
        {
            m_values[uniform] = std::make_shared<CShaderUniform>(E_UNIFORM_CLASS_VECTOR4);
        }

        i32 handle = m_uniforms[uniform];
        glUniform4fv(handle, 1, &vector[0]);
        m_values[uniform]->setVector4(vector);
    }
}

void CShader::setVector4Custom(const glm::vec4 &vector, const std::string &uniform)
{
    if(IResource::isLoaded() && IResource::isCommited())
    {
        i32 handle = glGetUniformLocation(m_shaderId, uniform.c_str());
        glUniform4fv(handle, 1, &vector[0]);
    }
}

void CShader::setFloat(f32 value, E_SHADER_UNIFORM uniform)
{
    if(IResource::isLoaded() && IResource::isCommited())
    {
        if(m_values[uniform] != nullptr && m_values[uniform]->getFloat() == value)
        {
            return;
        }
        else if(m_values[uniform] == nullptr)
        {
            m_values[uniform] = std::make_shared<CShaderUniform>(E_UNIFORM_CLASS_FLOAT);
        }

        i32 handle = m_uniforms[uniform];
        glUniform1f(handle, value);
        m_values[uniform]->setFloat(value);
    }
}

void CShader::setFloatCustom(f32 value, const std::string &uniform)
{
    if(IResource::isLoaded() && IResource::isCommited())
    {
        i32 handle = glGetUniformLocation(m_shaderId, uniform.c_str());
        glUniform1f(handle, value);
    }
}

void CShader::setInt(i32 value, E_SHADER_UNIFORM uniform)
{
    if(IResource::isLoaded() && IResource::isCommited())
    {
        if(m_values[uniform] != nullptr && m_values[uniform]->getInt() == value)
        {
            return;
        }
        else if(m_values[uniform] == nullptr)
        {
            m_values[uniform] = std::make_shared<CShaderUniform>(E_UNIFORM_CLASS_INT);
        }
        
        i32 handle = m_uniforms[uniform];
        glUniform1i(handle, value);
        m_values[uniform]->setInt(value);
    }
}

void CShader::setIntCustom(i32 value, const std::string &uniform)
{
    if(IResource::isLoaded() && IResource::isCommited())
    {
        i32 handle = glGetUniformLocation(m_shaderId, uniform.c_str());
        glUniform1i(handle, value);
    }
}

void CShader::setTexture(CSharedTextureRef texture, E_SHADER_SAMPLER sampler)
{
    if(IResource::isLoaded() && IResource::isCommited())
    {
        assert(sampler < E_SHADER_SAMPLER_MAX);
        glActiveTexture(GL_TEXTURE0 + sampler);
        texture->bind();
        glUniform1i(m_samplers[sampler], sampler);
    }
}

void CShader::bind(void) const
{
    if(IResource::isLoaded() && IResource::isCommited() && g_shaderId != m_shaderId)
    {
        g_shaderId = m_shaderId;
        glUseProgram(m_shaderId);
    }
}

void CShader::unbind(void) const
{

}

