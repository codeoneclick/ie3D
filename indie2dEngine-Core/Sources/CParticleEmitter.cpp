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

CParticleEmitter::CParticleEmitter(const std::shared_ptr<CResourceAccessor>& _resourceAccessor, const std::shared_ptr<IScreenSpaceTextureAccessor>& _screenSpaceTextureAccessor) :
IGameObject(_resourceAccessor, _screenSpaceTextureAccessor),
m_locked(0)
{
    m_settings = nullptr;
    m_lastEmittTimestamp = 0;
    m_renderQueuePosition = 16;
}

CParticleEmitter::~CParticleEmitter(void)
{
    delete[] m_particles;
}

void CParticleEmitter::_OnTemplateLoaded(std::shared_ptr<ITemplate> _template)
{
    m_settings = std::static_pointer_cast<SParticleEmitterTemplate>(_template);
    assert(m_resourceAccessor != nullptr);
    
    m_particles = new SParticle[m_settings->m_numParticles];

    std::shared_ptr<CVertexBuffer> vertexBuffer = std::make_shared<CVertexBuffer>(m_settings->m_numParticles * 4, GL_DYNAMIC_DRAW);
    SHardwareVertex* vertexData = vertexBuffer->Lock();
    for(ui32 i = 0; i < m_settings->m_numParticles; ++i)
    {
        m_particles[i].m_size = glm::vec2(0.0f, 0.0f);
        m_particles[i].m_color = glm::u8vec4(0, 0, 0, 0);
        
        vertexData[i * 4 + 0].m_texcoord = CVertexBuffer::CompressVec2(glm::vec2( 0.0f,  0.0f));
        vertexData[i * 4 + 1].m_texcoord = CVertexBuffer::CompressVec2(glm::vec2( 1.0f,  0.0f));
        vertexData[i * 4 + 2].m_texcoord = CVertexBuffer::CompressVec2(glm::vec2( 1.0f,  1.0f));
        vertexData[i * 4 + 3].m_texcoord = CVertexBuffer::CompressVec2(glm::vec2( 0.0f,  1.0f));
    }
    vertexBuffer->Unlock();
    
    std::shared_ptr<CIndexBuffer> indexBuffer = std::make_shared<CIndexBuffer>(m_settings->m_numParticles * 6, GL_STATIC_DRAW);
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

    for(const auto& materialTemplate : m_settings->m_materialsTemplates)
    {
        std::shared_ptr<CShader> shader = m_resourceAccessor->CreateShader(materialTemplate->m_shaderTemplate->m_vsFilename,
                                                                           materialTemplate->m_shaderTemplate->m_fsFilename);
        assert(shader != nullptr);
        shader->Register_LoadingHandler(shared_from_this());
        std::shared_ptr<CMaterial> material = std::make_shared<CMaterial>(shader, materialTemplate->m_filename);
        material->Serialize(materialTemplate, m_resourceAccessor, m_screenSpaceTextureAccessor, shared_from_this());
        m_materials.insert(std::make_pair(materialTemplate->m_renderMode, material));
        CParticleEmitter::_OnResourceLoaded(material, true);
    }

	IGameObject::ListenRenderMgr(m_isNeedToRender);
    m_status |= E_LOADING_STATUS_TEMPLATE_LOADED;
    IGameObject::_OnTemplateLoaded(_template);
}

void CParticleEmitter::_OnResourceLoaded(std::shared_ptr<IResource> _resource, bool _success)
{
    IGameObject::_OnResourceLoaded(_resource, _success);
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
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        if(m_locked == 0)
        {
            m_locked = 1;
            IGameObject::_OnSceneUpdate(_deltatime);
#if defined(__USE_GCDPP__)
            std::function<void(void)> function = [this]()
            {
#endif
                SHardwareVertex* vertexData = m_mesh->Get_VertexBuffer()->Lock();
                ui64 currentTime = CTimer::Get_TickCount();
                
                for(ui32 i = 0; i < m_settings->m_numParticles; ++i)
                {
                    ui64 particleAge = currentTime - m_particles[i].m_timestamp;
                    
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
                    
                    f32 particleClampAge = glm::clamp(static_cast<f32>(particleAge) / static_cast<f32>(m_settings->m_duration), 0.0f, 1.0f);
                    
                    f32 startVelocity = glm::length(m_particles[i].m_velocity);
                    f32 endVelocity = m_settings->m_endVelocity * startVelocity;
                    f32 velocityIntegral = startVelocity * particleClampAge + (endVelocity - startVelocity) * particleClampAge * particleClampAge / 2.0f;
                    m_particles[i].m_position += glm::normalize(m_particles[i].m_velocity) * velocityIntegral * static_cast<f32>(m_settings->m_duration);
                    m_particles[i].m_position += m_settings->m_gravity * static_cast<f32>(particleAge) * particleClampAge;
                    
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
#if defined(__USE_GCDPP__)
                std::function<void(void)> main = [this]()
                {
#endif
                    m_mesh->Get_VertexBuffer()->Unlock();
                    m_locked = 0;
#if defined(__USE_GCDPP__)
                };
                main();
                //gcdpp::impl::DispatchAsync(gcdpp::queue::GetMainQueue(), main);
            };
            function();
            //gcdpp::impl::DispatchAsync(gcdpp::queue::GetGlobalQueue(gcdpp::queue::GCDPP_DISPATCH_QUEUE_PRIORITY_LOW), function);
#endif
        }
    }
}

i32 CParticleEmitter::_OnQueuePosition(void)
{
    return m_renderQueuePosition;
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
