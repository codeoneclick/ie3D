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
#include "CGlobalLightSource.h"
#include "CResourceAccessor.h"
#include "CMesh.h"
#include "CVertexBuffer.h"
#include "CIndexBuffer.h"
#include "CConfigurationAccessor.h"

COcean::COcean(CSharedResourceAccessorRef resourceAccessor,
               ISharedRenderTechniqueAccessorRef renderTechniqueAccessor) :
IGameObject(resourceAccessor, renderTechniqueAccessor)
{
    m_isNeedBoundingBox = false;
    m_zOrder = E_GAME_OBJECT_Z_ORDER_OCEAN;
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
    
    std::shared_ptr<CConfigurationOcean> configurationOcean = std::static_pointer_cast<CConfigurationOcean>(configuration);
    assert(m_resourceAccessor != nullptr);
    
    m_size = configurationOcean->getSize();
    m_altitude = configurationOcean->getAltitude();
    
    m_waveGeneratorTimer = 0.0f;
    m_waveGeneratorInterval = configurationOcean->getWaveGenerationInterval();
    
    CSharedVertexBuffer vertexBuffer = std::make_shared<CVertexBuffer>(9 * 4, GL_STATIC_DRAW);
    SAttributeVertex* vertexData = vertexBuffer->lock();
    ui32 index = 0;
    for(i32 i = -1; i <= 1; ++i)
    {
        for(i32 j = -1; j <= 1; ++j)
        {
            vertexData[index * 4 + 0].m_position = glm::vec3(m_size * i, m_altitude, m_size * j);
            vertexData[index * 4 + 1].m_position = glm::vec3(m_size * i + m_size, m_altitude, m_size * j);
            vertexData[index * 4 + 2].m_position = glm::vec3(m_size * i + m_size, m_altitude, m_size * j + m_size);
            vertexData[index * 4 + 3].m_position = glm::vec3(m_size * i, m_altitude, m_size * j + m_size);
            
            vertexData[index * 4 + 0].m_extra = glm::u8vec4(i == 0 && j == 0 ? 1 : 0, i == 0 && j == 0 ? 1 : 2, 0, 0);
            vertexData[index * 4 + 1].m_extra = glm::u8vec4(i == 0 && j == 0 ? 1 : 0, i == 0 && j == 0 ? 1 : 2, 0, 0);
            vertexData[index * 4 + 2].m_extra = glm::u8vec4(i == 0 && j == 0 ? 1 : 0, i == 0 && j == 0 ? 1 : 2, 0, 0);
            vertexData[index * 4 + 3].m_extra = glm::u8vec4(i == 0 && j == 0 ? 1 : 0, i == 0 && j == 0 ? 1 : 2, 0, 0);
            
            vertexData[index * 4 + 0].m_normal = glm::packSnorm4x8(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
            vertexData[index * 4 + 1].m_normal = glm::packSnorm4x8(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
            vertexData[index * 4 + 2].m_normal = glm::packSnorm4x8(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
            vertexData[index * 4 + 3].m_normal = glm::packSnorm4x8(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
            
            vertexData[index * 4 + 0].m_texcoord = glm::packUnorm2x16(glm::vec2(0.0f,  0.0f));
            vertexData[index * 4 + 1].m_texcoord = glm::packUnorm2x16(glm::vec2(1.0f,  0.0f));
            vertexData[index * 4 + 2].m_texcoord = glm::packUnorm2x16(glm::vec2(1.0f,  1.0f));
            vertexData[index * 4 + 3].m_texcoord = glm::packUnorm2x16(glm::vec2(0.0f,  1.0f));
            
            index++;
        }
    }
    vertexBuffer->unlock();
    
    CSharedIndexBuffer indexBuffer = std::make_shared<CIndexBuffer>(9 * 6, GL_STATIC_DRAW);
    ui16* indexData = indexBuffer->lock();
    index = 0;
    for(i32 i = -1; i <= 1; ++i)
    {
        for(i32 j = -1; j <= 1; ++j)
        {
            indexData[index * 6 + 0] = index * 4 + 0;
            indexData[index * 6 + 1] = index * 4 + 1;
            indexData[index * 6 + 2] = index * 4 + 2;
            indexData[index * 6 + 3] = index * 4 + 0;
            indexData[index * 6 + 4] = index * 4 + 2;
            indexData[index * 6 + 5] = index * 4 + 3;
            
            index++;
        }
    }
    indexBuffer->unlock();
    
    m_mesh = CMesh::constructCustomMesh("ocean", vertexBuffer, indexBuffer,
                                        glm::vec3(4096.0), glm::vec3(-4096.0));
    assert(m_mesh != nullptr);
    
    IGameObject::enableRender(m_isNeedToRender);
    IGameObject::enableUpdate(m_isNeedToUpdate);
    
    m_status |= E_LOADING_STATUS_TEMPLATE_LOADED;
}

i32 COcean::zOrder(void)
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
        IGameObject::onBind(mode);
    }
}

void COcean::onDraw(const std::string& mode)
{
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        IGameObject::onDraw(mode);
    }
}

void COcean::onUnbind(const std::string& mode)
{
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        IGameObject::onUnbind(mode);
    }
}

void COcean::onBatch(const std::string& mode)
{

}

void COcean::bindCustomShaderUniforms(CSharedMaterialRef material)
{
    IGameObject::bindCustomShaderUniforms(material);
    
    material->getShader()->setFloat(m_waveGeneratorTimer, E_SHADER_UNIFORM_FLOAT_TIMER);
    material->getShader()->setFloatCustom(256.0, "IN_fogLinearStart");
    material->getShader()->setFloatCustom(512.0, "IN_fogLinearEnd");
    
    CSharedTexture heightmapTexture = m_resourceAccessor->getTexture("landscape.heightmap.texture");
    if(heightmapTexture)
    {
        material->setTexture(heightmapTexture, E_SHADER_SAMPLER_04);
    }
    glm::mat4x4 matrixViewInverse = glm::inverse(m_camera->Get_ViewMatrix());
    material->getShader()->setMatrix4x4Custom(matrixViewInverse, "u_matrixViewInverse");
}

void COcean::onOcclusionQueryDraw(CSharedMaterialRef material)
{
    
}

void COcean::onOcclusionQueryUpdate(void)
{
    
}
