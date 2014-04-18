//
//  COcean.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 7/24/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "COcean.h"
#include "CMaterial.h"
#include "CShader.h"
#include "CTexture.h"
#include "CCamera.h"
#include "CLight.h"
#include "CResourceAccessor.h"
#include "CConfigurationGameObjects.h"
#include "IScreenSpaceTextureAccessor.h"
#include "CMesh.h"
#include "CVertexBuffer.h"
#include "CIndexBuffer.h"

COcean::COcean(CSharedResourceAccessorRef resourceAccessor,
               ISharedScreenSpaceTextureAccessorRef screenSpaceTextureAccessor) :
IGameObject(resourceAccessor, screenSpaceTextureAccessor)
{
    m_zOrder = 5;
}

COcean::~COcean(void)
{
    
}

void COcean::onSceneUpdate(f32 deltatime)
{
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        m_waveGeneratorTimer += m_waveGeneratorInterval;
        IGameObject::onSceneUpdate(deltatime);
    }
}

void COcean::onResourceLoaded(ISharedResourceRef resource, bool success)
{
     IGameObject::onResourceLoaded(resource, success);
}

void COcean::onConfigurationLoaded(ISharedConfigurationRef configuration, bool success)
{
    IGameObject::onConfigurationLoaded(configuration, success);
    
    std::shared_ptr<CConfigurationOcean> oceanConfiguration = std::static_pointer_cast<CConfigurationOcean>(configuration);
    assert(m_resourceAccessor != nullptr);
    
    m_width = oceanConfiguration->getSize().x;
    m_height = oceanConfiguration->getSize().y;
    m_altitude = oceanConfiguration->getAltitude();
    
    m_waveGeneratorTimer = 0.0f;
    m_waveGeneratorInterval = oceanConfiguration->getWaveGenerationInterval();
    
    CSharedVertexBuffer vertexBuffer = std::make_shared<CVertexBuffer>(4, GL_STATIC_DRAW);
    SAttributeVertex* vertexData = vertexBuffer->lock();
    
    vertexData[0].m_position = glm::vec3(0.0f,  m_altitude,  0.0f);
    vertexData[1].m_position = glm::vec3(m_width, m_altitude,  0.0f);
    vertexData[2].m_position = glm::vec3(m_width, m_altitude,  m_height);
    vertexData[3].m_position = glm::vec3(0.0f,  m_altitude,  m_height);
    
    m_textureTileFactor = 8.0f;
    
    vertexData[0].m_texcoord = CVertexBuffer::compressVec2(glm::vec2(0.0f,  0.0f));
    vertexData[1].m_texcoord = CVertexBuffer::compressVec2(glm::vec2(1.0f,  0.0f));
    vertexData[2].m_texcoord = CVertexBuffer::compressVec2(glm::vec2(1.0f,  1.0f));
    vertexData[3].m_texcoord = CVertexBuffer::compressVec2(glm::vec2(0.0f,  1.0f));
    
    vertexBuffer->unlock();
    
    CSharedIndexBuffer indexBuffer = std::make_shared<CIndexBuffer>(6, GL_STATIC_DRAW);
    ui16* indexData = indexBuffer->lock();
    
    indexData[0] = 0;
    indexData[1] = 1;
    indexData[2] = 2;
    indexData[3] = 0;
    indexData[4] = 2;
    indexData[5] = 3;
    
    indexBuffer->unlock();
    
    m_mesh = CMesh::constructCustomMesh("ocean", vertexBuffer, indexBuffer,
                                     glm::vec3(4096.0), glm::vec3(4096.0));
    assert(m_mesh != nullptr);
    
	IGameObject::listenRenderMgr(m_isNeedToRender);
    m_status |= E_LOADING_STATUS_TEMPLATE_LOADED;
}

i32  COcean::zOrder(void)
{
    return m_zOrder;
}

bool COcean::checkOcclusion(void)
{
    return false;
}

ui32 COcean::numTriangles(void)
{
    return IGameObject::numTriangles();
}

void COcean::onBind(const std::string& mode)
{
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        assert(m_materials.find(mode) != m_materials.end());
        IGameObject::onBind(mode);
    }
}

void COcean::onDraw(const std::string& mode)
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
        material->getShader()->setMatrix4x4(m_camera->Get_MatrixNormal(), E_SHADER_UNIFORM_MATRIX_NORMAL);
        
        material->getShader()->setVector3(m_camera->Get_Position(), E_SHADER_UNIFORM_VECTOR_CAMERA_POSITION);
        material->getShader()->setFloat(m_camera->Get_Near(), E_SHADER_UNIFORM_FLOAT_CAMERA_NEAR);
        material->getShader()->setFloat(m_camera->Get_Far(), E_SHADER_UNIFORM_FLOAT_CAMERA_FAR);
        material->getShader()->setFloat(m_waveGeneratorTimer, E_SHADER_UNIFORM_FLOAT_TIMER);
        CSharedTexture heightmapTexture = m_screenSpaceTextureAccessor->getCustomTexture("landscape.heightmap");
        if(heightmapTexture)
        {
            material->setTexture(heightmapTexture, E_SHADER_SAMPLER_05);
        }
        
        IGameObject::onDraw(mode);
    }
}

void COcean::onUnbind(const std::string& mode)
{
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        assert(m_materials.find(mode) != m_materials.end());
        IGameObject::onUnbind(mode);
    }
}

void COcean::onBatch(const std::string& mode)
{
    IGameObject::onBatch(mode);
}
