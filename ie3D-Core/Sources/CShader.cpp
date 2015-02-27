//
//  CShader.cpp
//  gEngine-Core
//
//  Created by Sergey Sergeev on 5/7/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CShader.h"
#include "CTexture.h"
#include "CShaderCompiler_GLSL.h"

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
    std::string m_matrixM;
    std::string m_matrixV;
    std::string m_matrixP;
    std::string m_matrixN;
    std::string m_matrixVP;
    std::string m_matrixMVP;
    std::string m_bonesMatrix;
    std::string m_cameraPosition;
    std::string m_globalLightPosition;
    std::string m_globalLightProjectionMatrix;
    std::string m_globalLightViewMatrix;
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
    "u_matrixM",
    "u_matrixV",
    "u_matrixP",
    "u_matrixN",
    "u_matrixVP",
    "u_matrixMVP",
    "MATRIX_Bones",
    "VECTOR_CameraPosition",
    "VECTOR_GlobalLightPosition",
    "MATRIX_GlobalLightProjection",
    "MATRIX_GlobalLightView",
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
m_mat3x3_value(0.0),
m_mat4x4_value(0.0),
m_vec2_value(0.0),
m_vec3_value(0.0),
m_vec4_value(0.0),
m_f32_value(0.0),
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

CSharedShader CShader::constructCustomShader(const std::string& guid,
                                             const std::string& vsSourceCode,
                                             const std::string& fsSourceCode)
{
    CSharedShader shader = std::make_shared<CShader>(guid);
    
    std::string outMessage = "";
    bool outSuccess = false;
    ui32 vsHandle = CShaderCompiler_GLSL::compile(vsSourceCode, GL_VERTEX_SHADER, &outMessage, &outSuccess);
    if(!outSuccess)
    {
        std::cout<<outSuccess<<std::endl;
        return nullptr;
    }
    
    ui32 fsHandle = CShaderCompiler_GLSL::compile(fsSourceCode, GL_FRAGMENT_SHADER, &outMessage, &outSuccess);
    if(!outSuccess)
    {
        std::cout<<outSuccess<<std::endl;
        return nullptr;
    }
    
    ui32 shaderId = CShaderCompiler_GLSL::link(vsHandle, fsHandle, &outMessage, &outSuccess);
    if(!outSuccess)
    {
        std::cout<<outSuccess<<std::endl;
        return nullptr;
    }
    
    shader->m_shaderData = std::make_shared<CShaderData>(shaderId);
    shader->m_shaderId = shaderId;
    assert(shaderId != 0);
    shader->setupUniforms();
    
    shader->m_status |= E_RESOURCE_STATUS_LOADED;
    shader->m_status |= E_RESOURCE_STATUS_COMMITED;
    return shader;

}

CShader::~CShader(void)
{
    glDeleteProgram(m_shaderId);
}

void CShader::onResourceDataSerializationFinished(ISharedResourceDataRef resourceData)
{
    assert(resourceData != nullptr);
    switch(resourceData->getResourceDataClass())
    {
        case E_RESOURCE_DATA_CLASS_SHADER_DATA:
        {
            m_shaderData = std::static_pointer_cast<CShaderData>(resourceData);
            m_status |= E_RESOURCE_STATUS_LOADED;
        }
            break;
        default:
        {
            assert(false);
        }
            break;
    }
}

void CShader::onResourceDataCommitFinished(ISharedResourceDataRef resourceData)
{
    assert(resourceData != nullptr);
    switch(resourceData->getResourceDataClass())
    {
        case E_RESOURCE_DATA_CLASS_SHADER_DATA:
        {
            CSharedShaderData shaderData = std::static_pointer_cast<CShaderData>(resourceData);
            m_shaderId = shaderData->getShaderId();
            assert(m_shaderId != 0);
            CShader::setupUniforms();
            m_status |= E_RESOURCE_STATUS_COMMITED;
        }
            break;
        default:
        {
            assert(false);
        }
            break;
    }
}

void CShader::setupUniforms(void)
{
    m_uniforms[E_SHADER_UNIFORM_MATRIX_M] = ieGetUniformLocation(m_shaderId, SUniforms.m_matrixM.c_str());
    m_uniforms[E_SHADER_UNIFORM_MATRIX_V] = ieGetUniformLocation(m_shaderId, SUniforms.m_matrixV.c_str());
    m_uniforms[E_SHADER_UNIFORM_MATRIX_P] = ieGetUniformLocation(m_shaderId, SUniforms.m_matrixP.c_str());
    m_uniforms[E_SHADER_UNIFORM_MATRIX_N] = ieGetUniformLocation(m_shaderId, SUniforms.m_matrixN.c_str());
    m_uniforms[E_SHADER_UNIFORM_MATRIX_VP] = ieGetUniformLocation(m_shaderId, SUniforms.m_matrixVP.c_str());
    m_uniforms[E_SHADER_UNIFORM_MATRIX_MVP] = ieGetUniformLocation(m_shaderId, SUniforms.m_matrixMVP.c_str());
    m_uniforms[E_SHADER_UNIFORM_MATRIX_BONES] = ieGetUniformLocation(m_shaderId, SUniforms.m_bonesMatrix.c_str());
    m_uniforms[E_SHADER_UNIFORM_VECTOR_CAMERA_POSITION] = ieGetUniformLocation(m_shaderId, SUniforms.m_cameraPosition.c_str());
    m_uniforms[E_SHADER_UNIFORM_VECTOR_GLOBAL_LIGHT_POSITION] = ieGetUniformLocation(m_shaderId, SUniforms.m_globalLightPosition.c_str());
    m_uniforms[E_SHADER_UNIFORM_MATRIX_GLOBAL_LIGHT_PROJECTION] = ieGetUniformLocation(m_shaderId, SUniforms.m_globalLightProjectionMatrix.c_str());
    m_uniforms[E_SHADER_UNIFORM_MATRIX_GLOBAL_LIGHT_VIEW] = ieGetUniformLocation(m_shaderId, SUniforms.m_globalLightViewMatrix.c_str());
    m_uniforms[E_SHADER_UNIFORM_VECTOR_CLIP_PLANE] = ieGetUniformLocation(m_shaderId, SUniforms.m_clipPlane.c_str());
    m_uniforms[E_SHADER_UNIFORM_FLOAT_CAMERA_NEAR] = ieGetUniformLocation(m_shaderId, SUniforms.m_cameraNear.c_str());
    m_uniforms[E_SHADER_UNIFORM_FLOAT_CAMERA_FAR] = ieGetUniformLocation(m_shaderId, SUniforms.m_cameraFar.c_str());
    m_uniforms[E_SHADER_UNIFORM_FLOAT_TIMER] = ieGetUniformLocation(m_shaderId, SUniforms.m_timer.c_str());
    m_uniforms[E_SHADER_UNIFORM_INT_LIGHTS_COUNT] = ieGetUniformLocation(m_shaderId, SUniforms.m_lightsCount.c_str());
    
    m_uniforms[E_SHADER_UNIFORM_INT_FLAG_01] = ieGetUniformLocation(m_shaderId, SUniforms.m_flag01.c_str());
    m_uniforms[E_SHADER_UNIFORM_INT_FLAG_02] = ieGetUniformLocation(m_shaderId, SUniforms.m_flag02.c_str());
    m_uniforms[E_SHADER_UNIFORM_INT_FLAG_03] = ieGetUniformLocation(m_shaderId, SUniforms.m_flag03.c_str());
    m_uniforms[E_SHADER_UNIFORM_INT_FLAG_04] = ieGetUniformLocation(m_shaderId, SUniforms.m_flag04.c_str());
    
    m_samplers[E_SHADER_SAMPLER_01] = ieGetUniformLocation(m_shaderId, SSamplers.m_sampler_01.c_str());
    m_samplers[E_SHADER_SAMPLER_02] = ieGetUniformLocation(m_shaderId, SSamplers.m_sampler_02.c_str());
    m_samplers[E_SHADER_SAMPLER_03] = ieGetUniformLocation(m_shaderId, SSamplers.m_sampler_03.c_str());
    m_samplers[E_SHADER_SAMPLER_04] = ieGetUniformLocation(m_shaderId, SSamplers.m_sampler_04.c_str());
    m_samplers[E_SHADER_SAMPLER_05] = ieGetUniformLocation(m_shaderId, SSamplers.m_sampler_05.c_str());
    m_samplers[E_SHADER_SAMPLER_06] = ieGetUniformLocation(m_shaderId, SSamplers.m_sampler_06.c_str());
    m_samplers[E_SHADER_SAMPLER_07] = ieGetUniformLocation(m_shaderId, SSamplers.m_sampler_07.c_str());
    m_samplers[E_SHADER_SAMPLER_08] = ieGetUniformLocation(m_shaderId, SSamplers.m_sampler_08.c_str());
    
    m_attributes.at(E_SHADER_ATTRIBUTE_POSITION) = ieGetAttribLocation(m_shaderId, SAttributes.m_position.c_str());
    m_attributes.at(E_SHADER_ATTRIBUTE_TEXCOORD) = ieGetAttribLocation(m_shaderId, SAttributes.m_texcoord.c_str());
    m_attributes.at(E_SHADER_ATTRIBUTE_NORMAL) = ieGetAttribLocation(m_shaderId, SAttributes.m_normal.c_str());
    m_attributes.at(E_SHADER_ATTRIBUTE_TANGENT) = ieGetAttribLocation(m_shaderId, SAttributes.m_tangent.c_str());
    m_attributes.at(E_SHADER_ATTRIBUTE_COLOR) = ieGetAttribLocation(m_shaderId, SAttributes.m_color.c_str());
    m_attributes.at(E_SHADER_ATTRIBUTE_EXTRA) = ieGetAttribLocation(m_shaderId, SAttributes.m_extra.c_str());
    
    for(ui32 i = 0; i < E_SHADER_UNIFORM_MAX + E_SHADER_SAMPLER_MAX; ++i)
    {
        m_cachedUniformValues[i] = nullptr;
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

const std::array<i32, E_SHADER_ATTRIBUTE_MAX>& CShader::getAttributes(void) const
{
    return m_attributes;
}

i32 CShader::getCustomUniform(const std::string& uniform)
{
    i32 handle = -1;
    const auto iterator = m_customUniforms.find(uniform);
    if(iterator != m_customUniforms.end())
    {
        handle = iterator->second;
    }
    else
    {
        handle = ieGetUniformLocation(m_shaderId, uniform.c_str());
        m_customUniforms.insert(std::make_pair(uniform, handle));
    }
    return handle;
}

void CShader::setMatrix3x3(const glm::mat3x3 &matrix, E_SHADER_UNIFORM uniform)
{
    if(IResource::isLoaded() && IResource::isCommited())
    {
        if(m_cachedUniformValues[uniform] != nullptr && m_cachedUniformValues[uniform]->getMatrix3x3() == matrix)
        {
            return;
        }
        else if(m_cachedUniformValues[uniform] == nullptr)
        {
            m_cachedUniformValues[uniform] = std::make_shared<CShaderUniform>(E_UNIFORM_CLASS_MAT3X3);
        }
        
        i32 handle = m_uniforms[uniform];
        ieUniformMatrix3fv(handle, 1, 0, &matrix[0][0]);
        m_cachedUniformValues[uniform]->setMatrix3x3(matrix);
    }
}

void CShader::setMatrix3x3Custom(const glm::mat3x3 &matrix, const std::string &uniform)
{
    if(IResource::isLoaded() && IResource::isCommited())
    {
        ieUniformMatrix3fv(CShader::getCustomUniform(uniform), 1, 0, &matrix[0][0]);
    }
}

void CShader::setMatrix4x4(const glm::mat4x4 &matrix, E_SHADER_UNIFORM uniform)
{
    if(IResource::isLoaded() && IResource::isCommited())
    {
        if(m_cachedUniformValues[uniform] != nullptr && m_cachedUniformValues[uniform]->getMatrix4x4() == matrix)
        {
            return;
        }
        else if(m_cachedUniformValues[uniform] == nullptr)
        {
            m_cachedUniformValues[uniform] = std::make_shared<CShaderUniform>(E_UNIFORM_CLASS_MAT4X4);
        }

        i32 handle = m_uniforms[uniform];
        ieUniformMatrix4fv(handle, 1, 0, &matrix[0][0]);
        m_cachedUniformValues[uniform]->setMatrix4x4(matrix);
    }
}

void CShader::setMatrix4x4Custom(const glm::mat4x4 &matrix, const std::string &uniform)
{
    if(IResource::isLoaded() && IResource::isCommited())
    {
        ieUniformMatrix4fv(CShader::getCustomUniform(uniform), 1, 0, &matrix[0][0]);
    }
}

void CShader::setMatrixArray4x4(const glm::mat4x4* matrix, ui32 size, E_SHADER_UNIFORM uniform)
{
    if(IResource::isLoaded() && IResource::isCommited())
    {
        i32 handle = m_uniforms[uniform];
        ieUniformMatrix4fv(handle, size, 0, &matrix[0][0][0]);
    }
}

void CShader::setMatrixArray4x4Custom(const glm::mat4x4* matrix, ui32 size, const std::string& uniform)
{
    if(IResource::isLoaded() && IResource::isCommited())
    {
        ieUniformMatrix4fv(CShader::getCustomUniform(uniform), size, 0, &matrix[0][0][0]);
    }
}

void CShader::setVector2(const glm::vec2 &vector, E_SHADER_UNIFORM uniform)
{
    if(IResource::isLoaded() && IResource::isCommited())
    {
        if(m_cachedUniformValues[uniform] != nullptr && m_cachedUniformValues[uniform]->getVector2() == vector)
        {
            return;
        }
        else if(m_cachedUniformValues[uniform] == nullptr)
        {
            m_cachedUniformValues[uniform] = std::make_shared<CShaderUniform>(E_UNIFORM_CLASS_VECTOR2);
        }
        
        i32 handle = m_uniforms[uniform];
        ieUniform2fv(handle, 1, &vector[0]);
        m_cachedUniformValues[uniform]->setVector2(vector);
    }
}

void CShader::setVector2Custom(const glm::vec2 &vector, const std::string &uniform)
{
    if(IResource::isLoaded() && IResource::isCommited())
    {
        ieUniform2fv(CShader::getCustomUniform(uniform), 1, &vector[0]);
    }
}

void CShader::setVector2ArrayCustom(const glm::vec2* vectors, ui32 size, const std::string& uniform)
{
    if(IResource::isLoaded() && IResource::isCommited())
    {
        ieUniform2fv(CShader::getCustomUniform(uniform), size, &vectors[0][0]);
    }
}

void CShader::setVector3(const glm::vec3 &vector, E_SHADER_UNIFORM uniform)
{
    if(IResource::isLoaded() && IResource::isCommited())
    {
        if(m_cachedUniformValues[uniform] != nullptr && m_cachedUniformValues[uniform]->getVector3() == vector)
        {
            return;
        }
        else if(m_cachedUniformValues[uniform] == nullptr)
        {
            m_cachedUniformValues[uniform] = std::make_shared<CShaderUniform>(E_UNIFORM_CLASS_VECTOR3);
        }
        
        i32 handle = m_uniforms[uniform];
        ieUniform3fv(handle, 1, &vector[0]);
        m_cachedUniformValues[uniform]->setVector3(vector);
    }
}

void CShader::setVector3Custom(const glm::vec3 &vector, const std::string &uniform)
{
    if(IResource::isLoaded() && IResource::isCommited())
    {
        ieUniform3fv(CShader::getCustomUniform(uniform), 1, &vector[0]);
    }
}

void CShader::setVector3ArrayCustom(const glm::vec3* vectors, ui32 size, const std::string& uniform)
{
    if(IResource::isLoaded() && IResource::isCommited())
    {
        ieUniform3fv(CShader::getCustomUniform(uniform), size, &vectors[0][0]);
    }
}

void CShader::setVector4(const glm::vec4 &vector, E_SHADER_UNIFORM uniform)
{
    if(IResource::isLoaded() && IResource::isCommited())
    {
        if(m_cachedUniformValues[uniform] != nullptr && m_cachedUniformValues[uniform]->getVector4() == vector)
        {
            return;
        }
        else if(m_cachedUniformValues[uniform] == nullptr)
        {
            m_cachedUniformValues[uniform] = std::make_shared<CShaderUniform>(E_UNIFORM_CLASS_VECTOR4);
        }

        i32 handle = m_uniforms[uniform];
        ieUniform4fv(handle, 1, &vector[0]);
        m_cachedUniformValues[uniform]->setVector4(vector);
    }
}

void CShader::setVector4Custom(const glm::vec4 &vector, const std::string &uniform)
{
    if(IResource::isLoaded() && IResource::isCommited())
    {
        ieUniform4fv(CShader::getCustomUniform(uniform), 1, &vector[0]);
    }
}

void CShader::setFloat(f32 value, E_SHADER_UNIFORM uniform)
{
    if(IResource::isLoaded() && IResource::isCommited())
    {
        if(m_cachedUniformValues[uniform] != nullptr && m_cachedUniformValues[uniform]->getFloat() == value)
        {
            return;
        }
        else if(m_cachedUniformValues[uniform] == nullptr)
        {
            m_cachedUniformValues[uniform] = std::make_shared<CShaderUniform>(E_UNIFORM_CLASS_FLOAT);
        }

        i32 handle = m_uniforms[uniform];
        ieUniform1f(handle, value);
        m_cachedUniformValues[uniform]->setFloat(value);
    }
}

void CShader::setFloatCustom(f32 value, const std::string &uniform)
{
    if(IResource::isLoaded() && IResource::isCommited())
    {
        ieUniform1f(CShader::getCustomUniform(uniform), value);
    }
}

void CShader::setInt(i32 value, E_SHADER_UNIFORM uniform)
{
    if(IResource::isLoaded() && IResource::isCommited())
    {
        if(m_cachedUniformValues[uniform] != nullptr && m_cachedUniformValues[uniform]->getInt() == value)
        {
            return;
        }
        else if(m_cachedUniformValues[uniform] == nullptr)
        {
            m_cachedUniformValues[uniform] = std::make_shared<CShaderUniform>(E_UNIFORM_CLASS_INT);
        }
        
        i32 handle = m_uniforms[uniform];
        ieUniform1i(handle, value);
        m_cachedUniformValues[uniform]->setInt(value);
    }
}

void CShader::setIntCustom(i32 value, const std::string &uniform)
{
    if(IResource::isLoaded() && IResource::isCommited())
    {
        ieUniform1i(CShader::getCustomUniform(uniform), value);
    }
}

void CShader::setTexture(CSharedTextureRef texture, E_SHADER_SAMPLER sampler)
{
    if(IResource::isLoaded() && IResource::isCommited())
    {
        assert(sampler < E_SHADER_SAMPLER_MAX);
        ieActiveTexture(GL_TEXTURE0 + sampler);
        texture->bind();
        ieUniform1i(m_samplers[sampler], sampler);
    }
}

void CShader::bind(void) const
{
    if(IResource::isLoaded() && IResource::isCommited() && g_shaderId != m_shaderId)
    {
        g_shaderId = m_shaderId;
        ieUseProgram(m_shaderId);
    }
}

void CShader::unbind(void) const
{

}

