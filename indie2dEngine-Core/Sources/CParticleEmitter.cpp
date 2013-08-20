//
//  CParticleEmitter.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/24/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CParticleEmitter.h"
#include "CMaterial.h"
#include "CShader.h"
#include "CTexture.h"
#include "CCamera.h"
#include "CLight.h"
#include "CResourceAccessor.h"
#include "CVertexBuffer.h"
#include "CIndexBuffer.h"
#include "CMesh.h"
#include "ITemplate.h"
#include "CCommonOS.h"
#include "CTimer.h"

CParticleEmitter::CParticleEmitter(std::shared_ptr<CResourceAccessor> _resourceFabricator) :
IGameObject(_resourceFabricator)
{
    m_settings = nullptr;
    m_lastEmittTimestamp = 0;
}

CParticleEmitter::~CParticleEmitter(void)
{
    delete[] m_particles;
}

void CParticleEmitter::_OnTemplateLoaded(std::shared_ptr<ITemplate> _template)
{
    m_settings = std::static_pointer_cast<SParticleEmitterTemplate>(_template);
    assert(m_resourceFabricator != nullptr);
    
    m_particles = new SParticle[m_settings->m_numParticles];
    std::shared_ptr<CVertexBuffer> vertexBuffer = std::make_shared<CVertexBuffer>(m_settings->m_numParticles * 4, GL_STREAM_DRAW);
    SVertex* vertexData = vertexBuffer->Lock();
    
    for(ui32 i = 0; i < m_settings->m_numParticles; ++i)
    {
        m_particles[i].m_size = glm::vec2(0.0f, 0.0f);
        m_particles[i].m_color = glm::u8vec4(0, 0, 0, 0);
        
        vertexData[i * 4 + 0].m_texcoord = glm::vec2( 0.0f,  0.0f);
        vertexData[i * 4 + 1].m_texcoord = glm::vec2( 1.0f,  0.0f);
        vertexData[i * 4 + 2].m_texcoord = glm::vec2( 1.0f,  1.0f);
        vertexData[i * 4 + 3].m_texcoord = glm::vec2( 0.0f,  1.0f);
    }
    vertexBuffer->Unlock();
    
    std::shared_ptr<CIndexBuffer> indexBuffer = std::make_shared<CIndexBuffer>(m_settings->m_numParticles * 6, GL_STREAM_DRAW);
    ui16* indexData = indexBuffer->Lock();
    
    for(ui32 i = 0; i < m_settings->m_numParticles; ++i)
    {
        indexData[i * 6 + 0] = static_cast<ui16>(i * 4 + 0);
        indexData[i * 6 + 1] = static_cast<ui16>(i * 4 + 1);
        indexData[i * 6 + 2] = static_cast<ui16>(i * 4 + 2);
        
        indexData[i * 6 + 3] = static_cast<ui16>(i * 4 + 0);
        indexData[i * 6 + 4] = static_cast<ui16>(i * 4 + 2);
        indexData[i * 6 + 5] = static_cast<ui16>(i * 4 + 3);
    }
    
    indexBuffer->Unlock();
    
    m_mesh = std::make_shared<CMesh>("particle.emitter", vertexBuffer, indexBuffer);
    assert(m_mesh != nullptr);
    
    for(auto materialTemplate : m_settings->m_materialsTemplates)
    {
        std::shared_ptr<CShader> shader = m_resourceFabricator->CreateShader(materialTemplate->m_shaderTemplate->m_vsFilename,
                                                                             materialTemplate->m_shaderTemplate->m_fsFilename);
        assert(shader != nullptr);
        
        std::shared_ptr<CMaterial> material = std::make_shared<CMaterial>(shader);
        material->Serialize(materialTemplate, m_resourceFabricator, m_renderMgr);
        m_materials.insert(std::make_pair(materialTemplate->m_renderMode, material));
    }
    IGameObject::_ListenRenderMgr();
    m_status |= E_LOADING_STATUS_TEMPLATE_LOADED;
}

void CParticleEmitter::_EmittParticle(ui32 _index)
{
    m_particles[_index].m_position = m_position;
    m_particles[_index].m_velocity = glm::vec3(0.0f, 0.0f, 0.0f);
    
    m_particles[_index].m_size = m_settings->m_startSize;
    m_particles[_index].m_color = m_settings->m_startColor;
    
    m_particles[_index].m_timestamp = CTimer::Get_TickCount();
    
    f32 horizontalVelocity = glm::mix(m_settings->m_minHorizontalVelocity, m_settings->m_maxHorizontalVelocity, Get_Random(0.0f, 1.0f));
    
    f32 horizontalAngle = Get_Random(0.0f, 1.0f) * M_PI * 2.0f;
    
    m_particles[_index].m_velocity.x += horizontalVelocity * cosf(horizontalAngle);
    m_particles[_index].m_velocity.z += horizontalVelocity * sinf(horizontalAngle);
    
    m_particles[_index].m_velocity.y += glm::mix(m_settings->m_minVerticalVelocity, m_settings->m_maxVerticalVelocity, Get_Random(0.0f, 1.0f));
    m_particles[_index].m_velocity *= m_settings->m_velocitySensitivity;
}

void CParticleEmitter::_OnSceneUpdate(f32 _deltatime)
{
    if( m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        IGameObject::_OnSceneUpdate(_deltatime);
        
        SVertex* vertexData = m_mesh->Get_VertexBuffer()->Lock();
        f32 currentTime = CTimer::Get_TickCount();
        
        for(ui32 i = 0; i < m_settings->m_numParticles; ++i)
        {
            f32 particleAge = currentTime - m_particles[i].m_timestamp;
            
            if(particleAge > m_settings->m_duration)
            {
                if((currentTime - m_lastEmittTimestamp) > Get_Random(m_settings->m_minParticleEmittInterval, m_settings->m_maxParticleEmittInterval))
                {
                    m_lastEmittTimestamp = currentTime;
                    CParticleEmitter::_EmittParticle(i);
                }
                else
                {
                    m_particles[i].m_size = glm::vec2(0.0f, 0.0f);
                    m_particles[i].m_color = glm::u8vec4(0, 0, 0, 0);
                }
            }
            
            f32 particleClampAge = glm::clamp( particleAge / m_settings->m_duration, 0.0f, 1.0f);
            
            f32 startVelocity = glm::length(m_particles[i].m_velocity);
            f32 endVelocity = m_settings->m_endVelocity * startVelocity;
            f32 velocityIntegral = startVelocity * particleClampAge + (endVelocity - startVelocity) * particleClampAge * particleClampAge / 2.0f;
            m_particles[i].m_position += glm::normalize(m_particles[i].m_velocity) * velocityIntegral * m_settings->m_duration;
            m_particles[i].m_position += m_settings->m_gravity * particleAge * particleClampAge;
            
            f32 randomValue = Get_Random(0.0f, 1.0f);
            f32 startSize = glm::mix(m_settings->m_startSize.x, m_settings->m_startSize.y, randomValue);
            f32 endSize = glm::mix(m_settings->m_endSize.x, m_settings->m_endSize.y, randomValue);
            m_particles[i].m_size = glm::vec2(glm::mix(startSize, endSize, particleClampAge));
            
            m_particles[i].m_color = glm::mix(m_settings->m_startColor, m_settings->m_endColor, particleClampAge);
            m_particles[i].m_color.a = glm::mix(m_settings->m_startColor.a, m_settings->m_endColor.a, particleClampAge);
            
            glm::mat4x4 matrixSpherical = m_camera->Get_SphericalMatrixForPosition(m_particles[i].m_position);
            
            glm::vec4 position = glm::vec4(-m_particles[i].m_size.x, -m_particles[i].m_size.y, 0.0f, 1.0f);
            position = matrixSpherical * position;
            vertexData[i * 4 + 0].m_position = glm::vec3(position.x, position.y, position.z);
            
            position = glm::vec4(m_particles[i].m_size.x, -m_particles[i].m_size.y, 0.0f, 1.0f);
            position = matrixSpherical * position;
            vertexData[i * 4 + 1].m_position = glm::vec3(position.x, position.y, position.z);
            
            position = glm::vec4(m_particles[i].m_size.x, m_particles[i].m_size.y, 0.0f, 1.0f);
            position = matrixSpherical * position;
            vertexData[i * 4 + 2].m_position = glm::vec3(position.x, position.y, position.z);
            
            position = glm::vec4(-m_particles[i].m_size.x, m_particles[i].m_size.y, 0.0f, 1.0f);
            position = matrixSpherical * position;
            vertexData[i * 4 + 3].m_position = glm::vec3(position.x, position.y, position.z);
            
            vertexData[i * 4 + 0].m_color = m_particles[i].m_color;
            vertexData[i * 4 + 1].m_color = m_particles[i].m_color;
            vertexData[i * 4 + 2].m_color = m_particles[i].m_color;
            vertexData[i * 4 + 3].m_color = m_particles[i].m_color;
        }
        m_mesh->Get_VertexBuffer()->Unlock();
    }
}

i32 CParticleEmitter::_OnQueuePosition(void)
{
    return 128;
}

void CParticleEmitter::_OnBind(const std::string& _renderMode)
{
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        assert(m_materials.find(_renderMode) != m_materials.end());
        IGameObject::_OnBind(_renderMode);
    }
}

void CParticleEmitter::_OnDraw(const std::string& _renderMode)
{
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        assert(m_camera != nullptr);
        assert(m_light != nullptr);
        assert(m_materials.find(_renderMode) != m_materials.end());
        
        std::shared_ptr<CMaterial> material = m_materials.find(_renderMode)->second;
        assert(material->Get_Shader() != nullptr);
        
        material->Get_Shader()->Set_Matrix4x4(m_matrixWorld, E_SHADER_UNIFORM_MATRIX_WORLD);
        material->Get_Shader()->Set_Matrix4x4(m_camera->Get_ProjectionMatrix(), E_SHADER_UNIFORM_MATRIX_PROJECTION);
        material->Get_Shader()->Set_Matrix4x4(m_camera->Get_ViewMatrix(), E_SHADER_UNIFORM_MATRIX_VIEW);
        
        IGameObject::_OnDraw(_renderMode);
    }
}

void CParticleEmitter::_OnUnbind(const std::string &_renderMode)
{
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        assert(m_materials.find(_renderMode) != m_materials.end());
        IGameObject::_OnUnbind(_renderMode);
    }
}
