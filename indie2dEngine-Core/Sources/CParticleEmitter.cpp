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
#include "CConfigurationGameObjects.h"
#include "CCommonOS.h"
#include "CTimer.h"

CParticleEmitter::CParticleEmitter(CSharedResourceAccessorRef resourceAccessor,
                                   ISharedScreenSpaceTextureAccessorRef screenSpaceTextureAccessor) :
IGameObject(resourceAccessor, screenSpaceTextureAccessor)
{
    m_settings = nullptr;
    m_lastEmittTimestamp = 0;
    m_zOrder = 5;
}

CParticleEmitter::~CParticleEmitter(void)
{
    delete[] m_particles;
}

void CParticleEmitter::emittParticle(ui32 index)
{
    m_particles[index].m_position = m_position;
    m_particles[index].m_velocity = glm::vec3(0.0f, 0.0f, 0.0f);
    
    m_particles[index].m_size = m_settings->getSourceSize();
    m_particles[index].m_color = m_settings->getSourceColor();
    
    m_particles[index].m_timestamp = CTimer::Get_TickCount();
    
    f32 horizontalVelocity = glm::mix(m_settings->getMinHorizontalVelocity(), m_settings->getMaxHorizontalVelocity(), Get_Random(0.0f, 1.0f));
    
    f32 horizontalAngle = Get_Random(0.0f, 1.0f) * M_PI * 2.0f;
    
    m_particles[index].m_velocity.x += horizontalVelocity * cosf(horizontalAngle);
    m_particles[index].m_velocity.z += horizontalVelocity * sinf(horizontalAngle);
    
    m_particles[index].m_velocity.y += glm::mix(m_settings->getMinVerticalVelocity(), m_settings->getMaxVerticalVelocity(), Get_Random(0.0f, 1.0f));
    m_particles[index].m_velocity *= m_settings->getVelocitySensitivity();
}

void CParticleEmitter::onSceneUpdate(f32 deltatime)
{
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        IGameObject::onSceneUpdate(deltatime);
        SAttributeVertex* vertexData = m_mesh->getVertexBuffer()->lock();
        ui64 currentTime = CTimer::Get_TickCount();
        
        for(ui32 i = 0; i < m_settings->getNumParticles(); ++i)
        {
            ui64 particleAge = currentTime - m_particles[i].m_timestamp;
            
            if(particleAge > m_settings->getDuration())
            {
                if((currentTime - m_lastEmittTimestamp) > Get_Random(m_settings->getMinEmittInterval(), m_settings->getMaxEmittInterval()))
                {
                    m_lastEmittTimestamp = currentTime;
                    CParticleEmitter::emittParticle(i);
                    particleAge = 0;
                }
                else
                {
                    m_particles[i].m_size = glm::vec2(0.0f, 0.0f);
                    m_particles[i].m_color = glm::u8vec4(0, 0, 0, 0);
                }
            }
            
            f32 particleClampAge = glm::clamp(static_cast<f32>(particleAge) / static_cast<f32>(m_settings->getDuration()), 0.0f, 1.0f);
            
            f32 startVelocity = glm::length(m_particles[i].m_velocity);
            f32 endVelocity = m_settings->getEndVelocity() * startVelocity;
            f32 velocityIntegral = startVelocity * particleClampAge + (endVelocity - startVelocity) * particleClampAge * particleClampAge / 2.0f;
            m_particles[i].m_position += glm::normalize(m_particles[i].m_velocity) * velocityIntegral * static_cast<f32>(m_settings->getDuration());
            m_particles[i].m_position += m_settings->getGravity() * static_cast<f32>(particleAge) * particleClampAge;
            
            f32 randomValue = Get_Random(0.0f, 1.0f);
            f32 startSize = glm::mix(m_settings->getSourceSize().x, m_settings->getSourceSize().y, randomValue);
            f32 endSize = glm::mix(m_settings->getDestinationSize().x, m_settings->getDestinationSize().y, randomValue);
            m_particles[i].m_size = glm::vec2(glm::mix(startSize, endSize, particleClampAge));
            
            m_particles[i].m_color = glm::mix(m_settings->getSourceColor(), m_settings->getDestinationColor(), particleClampAge);
            m_particles[i].m_color.a = glm::mix(m_settings->getSourceColor().a, m_settings->getDestinationColor().a, particleClampAge);
            
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
        m_mesh->getVertexBuffer()->unlock();
    }
    
}

void CParticleEmitter::onResourceLoaded(ISharedResourceRef resource,
                                        bool success)
{
    IGameObject::onResourceLoaded(resource, success);
}

void CParticleEmitter::onConfigurationLoaded(ISharedConfigurationRef configuration,
                                             bool success)
{
    IGameObject::onConfigurationLoaded(configuration, success);
    m_settings = std::static_pointer_cast<CConfigurationParticleEmitter>(configuration);
    assert(m_resourceAccessor != nullptr);
    
    m_particles = new SParticle[m_settings->getNumParticles()];
    
    CSharedVertexBuffer vertexBuffer = std::make_shared<CVertexBuffer>(m_settings->getNumParticles() * 4, GL_STREAM_DRAW);
    SAttributeVertex* vertexData = vertexBuffer->lock();
    for(ui32 i = 0; i < m_settings->getNumParticles(); ++i)
    {
        m_particles[i].m_size = glm::vec2(0.0f, 0.0f);
        m_particles[i].m_color = glm::u8vec4(0, 0, 0, 0);
        
        vertexData[i * 4 + 0].m_texcoord = CVertexBuffer::compressVec2(glm::vec2( 0.0f,  0.0f));
        vertexData[i * 4 + 1].m_texcoord = CVertexBuffer::compressVec2(glm::vec2( 1.0f,  0.0f));
        vertexData[i * 4 + 2].m_texcoord = CVertexBuffer::compressVec2(glm::vec2( 1.0f,  1.0f));
        vertexData[i * 4 + 3].m_texcoord = CVertexBuffer::compressVec2(glm::vec2( 0.0f,  1.0f));
    }
    vertexBuffer->unlock();
    
    CSharedIndexBuffer indexBuffer = std::make_shared<CIndexBuffer>(m_settings->getNumParticles() * 6, GL_STATIC_DRAW);
    ui16* indexData = indexBuffer->lock();
    for(ui32 i = 0; i < m_settings->getNumParticles(); ++i)
    {
        indexData[i * 6 + 0] = static_cast<ui16>(i * 4 + 0);
        indexData[i * 6 + 1] = static_cast<ui16>(i * 4 + 1);
        indexData[i * 6 + 2] = static_cast<ui16>(i * 4 + 2);
        
        indexData[i * 6 + 3] = static_cast<ui16>(i * 4 + 0);
        indexData[i * 6 + 4] = static_cast<ui16>(i * 4 + 2);
        indexData[i * 6 + 5] = static_cast<ui16>(i * 4 + 3);
    }
    indexBuffer->unlock();
    
    m_mesh = CMesh::constructCustomMesh("particle.emitter", vertexBuffer, indexBuffer,
                                     glm::vec3(0.0), glm::vec3(0.0));
    assert(m_mesh != nullptr);
    
	IGameObject::listenRenderMgr(m_isNeedToRender);
    m_status |= E_LOADING_STATUS_TEMPLATE_LOADED;
}

i32  CParticleEmitter::zOrder(void)
{
    return m_zOrder;
}

bool CParticleEmitter::checkOcclusion(void)
{
    return IGameObject::checkOcclusion();
}

ui32 CParticleEmitter::numTriangles(void)
{
    return IGameObject::numTriangles();
}

void CParticleEmitter::onBind(const std::string& mode)
{
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        assert(m_materials.find(mode) != m_materials.end());
        IGameObject::onBind(mode);
    }
}

void CParticleEmitter::onDraw(const std::string& mode)
{
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        assert(m_camera != nullptr);
        assert(m_materials.find(mode) != m_materials.end());
        
        std::shared_ptr<CMaterial> material = m_materials.find(mode)->second;
        assert(material->getShader() != nullptr);
        
        material->getShader()->setMatrix4x4(m_matrixWorld, E_SHADER_UNIFORM_MATRIX_WORLD);
        material->getShader()->setMatrix4x4(m_camera->Get_ProjectionMatrix(), E_SHADER_UNIFORM_MATRIX_PROJECTION);
        material->getShader()->setMatrix4x4(m_camera->Get_ViewMatrix(), E_SHADER_UNIFORM_MATRIX_VIEW);
        
        IGameObject::onDraw(mode);
    }
}

void CParticleEmitter::onUnbind(const std::string& mode)
{
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        assert(m_materials.find(mode) != m_materials.end());
        IGameObject::onUnbind(mode);
    }
}

void CParticleEmitter::onBatch(const std::string& mode)
{
    IGameObject::onBatch(mode);
}
