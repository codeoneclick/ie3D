//
//  CAtmosphericScattering.cpp
//  ie3D-Core
//
//  Created by sergey.sergeev on 9/24/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#include "CAtmosphericScattering.h"
#include "CMaterial.h"
#include "CShader.h"
#include "CTexture.h"
#include "CCamera.h"
#include "CGlobalLightSource.h"
#include "CResourceAccessor.h"
#include "CConfigurationGameObjects.h"
#include "CMesh.h"
#include "CVertexBuffer.h"
#include "CIndexBuffer.h"

CAtmosphericScattering::CAtmosphericScattering(CSharedResourceAccessorRef resourceAccessor,
                                               ISharedRenderTechniqueAccessorRef renderTechniqueAccessor) :
IGameObject(resourceAccessor, renderTechniqueAccessor)
{
    m_zOrder = E_GAME_OBJECT_Z_ORDER_ATMOSPHERIC_SCATTERING;
}

CAtmosphericScattering::~CAtmosphericScattering(void)
{
    
}

void CAtmosphericScattering::onSceneUpdate(f32 deltatime)
{
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        IGameObject::onSceneUpdate(deltatime);
    }
}

void CAtmosphericScattering::onResourceLoaded(ISharedResourceRef resource, bool success)
{
    IGameObject::onResourceLoaded(resource, success);
}

void CAtmosphericScattering::onConfigurationLoaded(ISharedConfigurationRef configuration, bool success)
{
    IGameObject::onConfigurationLoaded(configuration, success);
    
    std::shared_ptr<CConfigurationAtmosphericScattering> atmosphericScatteringConfiguration = std::static_pointer_cast<CConfigurationAtmosphericScattering>(configuration);
    assert(m_resourceAccessor != nullptr);
    
    i32 numCols = atmosphericScatteringConfiguration->getNumCols();
    i32 numRows = atmosphericScatteringConfiguration->getNumRows();
    
    m_innerRadius = atmosphericScatteringConfiguration->getInnerRadius();
    m_outerRadius = atmosphericScatteringConfiguration->getOuterRadius();
    m_size = atmosphericScatteringConfiguration->getSize();
    m_numSamples = atmosphericScatteringConfiguration->getNumSamples();
    m_scale_01 = 1.0 / (m_outerRadius - m_innerRadius);
    m_rayleighScaleDepth = atmosphericScatteringConfiguration->getRayleighScaleDepthCoefficient();
    m_scale_02 = m_scale_01 / m_rayleighScaleDepth;
    
    glm::vec3 waveLength = atmosphericScatteringConfiguration->getWaveLength();
    m_invWaveLength.x = 1.0 / powf(waveLength.x, 4.0);
    m_invWaveLength.y = 1.0 / powf(waveLength.y, 4.0);
    m_invWaveLength.z = 1.0 / powf(waveLength.z, 4.0);
    
    f32 kr = atmosphericScatteringConfiguration->getKrCoefficient();
    f32 km = atmosphericScatteringConfiguration->getKmCoefficient();
    m_kr4PI = kr * 4.0 * M_PI;
    m_km4PI = km * 4.0 * M_PI;
    
    CSharedVertexBuffer vertexBuffer = std::make_shared<CVertexBuffer>(numCols * numRows, GL_STATIC_DRAW);
    SAttributeVertex* vertexData = vertexBuffer->lock();
    
    ui32 index = 0;
    for(ui32 i = 0; i < numCols; i++)
    {
        const f32 offsetXZ = cosf(1.0) * static_cast<f32>(i) / static_cast<f32>(numCols - 1);
        for(ui32 j = 0; j < numRows; j++)
        {
            const f32 offsetY = (M_PI * 2.0) * static_cast<f32>(j) / static_cast<f32>(numRows - 1.0) ;
            vertexData[index].m_position.x = sinf(offsetXZ) * cosf(offsetY) * m_outerRadius;
            vertexData[index].m_position.y = cosf(offsetXZ) * m_outerRadius - m_innerRadius;
            vertexData[index].m_position.z = sinf(offsetXZ) * sinf(offsetY) * m_outerRadius;
            index++;
        }
    }
    vertexBuffer->unlock();
    CSharedIndexBuffer indexBuffer = std::make_shared<CIndexBuffer>((numCols - 1) * (numRows - 1) * 6, GL_STATIC_DRAW);
    ui16* indexData = indexBuffer->lock();
    for(ui32 i = 0; i < numRows - 1; i++)
    {
        for(ui32 j = 0; j < numCols - 1; j++)
        {
            *(indexData++) = i * numRows + j;
            *(indexData++) = (i + 1) * numRows + j;
            *(indexData++) = (i + 1) * numRows + j + 1;
            *(indexData++) = (i + 1) * numRows + j + 1;
            *(indexData++) = i * numRows + j + 1;
            *(indexData++) = i * numRows + j;
        }
    }
    indexBuffer->unlock();
    
    m_mesh = CMesh::constructCustomMesh("atmosphericScattering", vertexBuffer, indexBuffer,
                                        glm::vec3(4096.0), glm::vec3(4096.0));
    assert(m_mesh != nullptr);
    
    IGameObject::enableRender(m_isNeedToRender);
    IGameObject::enableUpdate(m_isNeedToUpdate);
    
    m_status |= E_LOADING_STATUS_TEMPLATE_LOADED;
}

i32 CAtmosphericScattering::zOrder(void)
{
    return m_zOrder;
}

bool CAtmosphericScattering::checkOcclusion(void)
{
    return false;
}

ui32 CAtmosphericScattering::numTriangles(void)
{
    return IGameObject::numTriangles();
}

void CAtmosphericScattering::onBind(const std::string& mode)
{
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        assert(m_materials.find(mode) != m_materials.end());
        IGameObject::onBind(mode);
    }
}

void CAtmosphericScattering::onDraw(const std::string& mode)
{
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        assert(m_camera != nullptr);
        assert(m_globalLightSource != nullptr);
        assert(m_materials.find(mode) != m_materials.end());
        
        IGameObject::setPosition(glm::vec3(m_camera->Get_Position().x, m_camera->Get_Position().y, m_camera->Get_Position().z));
        IGameObject::onSceneUpdate(0);
        
        CSharedMaterial material = m_materials.find(mode)->second;
        assert(material->getShader() != nullptr);
        
        material->getShader()->setMatrix4x4(m_matrixWorld, E_SHADER_UNIFORM_MATRIX_WORLD);
        material->getShader()->setMatrix4x4(m_camera->Get_ProjectionMatrix(), E_SHADER_UNIFORM_MATRIX_PROJECTION);
        material->getShader()->setMatrix4x4(m_camera->Get_ViewMatrix(), E_SHADER_UNIFORM_MATRIX_VIEW);
        material->getShader()->setMatrix4x4(m_camera->Get_MatrixNormal(), E_SHADER_UNIFORM_MATRIX_NORMAL);
        
        material->getShader()->setVector3(m_camera->Get_Position(), E_SHADER_UNIFORM_VECTOR_CAMERA_POSITION);
        material->getShader()->setVector3(m_globalLightSource->getPosition(), E_SHADER_UNIFORM_VECTOR_GLOBAL_LIGHT_POSITION);
        material->getShader()->setFloat(m_camera->Get_Near(), E_SHADER_UNIFORM_FLOAT_CAMERA_NEAR);
        material->getShader()->setFloat(m_camera->Get_Far(), E_SHADER_UNIFORM_FLOAT_CAMERA_FAR);
        
        IGameObject::onDraw(mode);
    }
}

void CAtmosphericScattering::onUnbind(const std::string& mode)
{
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        assert(m_materials.find(mode) != m_materials.end());
        IGameObject::onUnbind(mode);
    }
}

void CAtmosphericScattering::onBatch(const std::string& mode)
{
    IGameObject::onBatch(mode);
}
