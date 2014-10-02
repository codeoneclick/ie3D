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
#include "CLight.h"
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
        IGameObject::setPosition(glm::vec3(m_camera->Get_Position().x, m_camera->Get_Position().y, m_camera->Get_Position().z));
        IGameObject::setRotation(glm::vec3(0.0, glm::degrees(m_camera->Get_Rotation()) + 45.0, 0.0));
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
    
    //i32 numCols = atmosphericScatteringConfiguration->getNumCols();
    //i32 numRows = atmosphericScatteringConfiguration->getNumRows();
    
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
    
    std::vector<glm::vec3> vertexes;
    glm::vec3 *pVertex = nullptr;
    
    size_t sLevel = 0, sNumInLevel = 0, i =-1;
    size_t sCurrentRaduis = 0;
    float fCurRadiusAlpha = 0.0f, fCurRadiusBetta = 0.0f;
    size_t m_sGridLevelsCount = 32;
    f32 m_fGridRadius = 32;
    
    // проход по уровням
    for( sLevel = 0; sLevel < m_sGridLevelsCount + 1; sLevel++ )
    {
        // выделяем место для новой строки
        vertexes.resize(vertexes.size() + m_sGridLevelsCount - sLevel + 1 );
        
        // проходим по вершинам в уровне
        for( sNumInLevel = 0; sNumInLevel < m_sGridLevelsCount - sLevel + 1; sNumInLevel++ )
        {
            // новая вершина
            i++;
            // получаем
            pVertex = &vertexes[i];
            
            // текущий радиус
            sCurrentRaduis  = sNumInLevel + sLevel;
            
            // считаем угол смещения в радиусе ( сектор в радианах делим на число вершин )
            fCurRadiusAlpha  = sCurrentRaduis == 0 ? 0.0f : sLevel * M_PI_2 / sCurrentRaduis;
            // угол смещения по высоте
            fCurRadiusBetta  = M_PI_2 - sCurrentRaduis * M_PI_2 / m_sGridLevelsCount;
            
            // позиция
            pVertex->x  = m_fGridRadius * cosf( fCurRadiusBetta ) * cosf( fCurRadiusAlpha );
            pVertex->y  = m_fGridRadius * sinf( fCurRadiusBetta );
            pVertex->z  = m_fGridRadius * cosf( fCurRadiusBetta ) * sinf( fCurRadiusAlpha );
        }
    }
    
    CSharedVertexBuffer vertexBuffer = std::make_shared<CVertexBuffer>(vertexes.size(), GL_STATIC_DRAW);
    SAttributeVertex* vertexData = vertexBuffer->lock();
    for(size_t i = 0; i < vertexes.size(); ++i)
    {
        vertexData[i].m_position = vertexes.at(i);
    }
    vertexBuffer->unlock();
    
    std::vector<ui16> indexes;
    
    sLevel = 0, sNumInLevel = 0, i = 0;
    
    size_t sCurLevelBegin = 0, sNextLevelBegin = 0, sCurLevelVertexesCount = 0;
    
    // проход по уровням
    for( sLevel = 0; sLevel < m_sGridLevelsCount; sLevel++ )
    {
        // число вершин в уровне
        sCurLevelVertexesCount = m_sGridLevelsCount - sLevel + 1;
        
        // выделяем место для новой строки
        indexes.resize( indexes.size() + ( m_sGridLevelsCount - sLevel - 1) * 6 + 3 );
        
        // первая вершина следующего уровня
        sNextLevelBegin = sCurLevelBegin + sCurLevelVertexesCount;
        
        // проходим по вершинам в уровне кроме последней
        for( sNumInLevel = 0; sNumInLevel < m_sGridLevelsCount - sLevel; sNumInLevel++ )
        {
            // 2 грани на 4 точки
            indexes[i++] = sNextLevelBegin + sNumInLevel;
            indexes[i++] = sCurLevelBegin + sNumInLevel;
            indexes[i++] = sCurLevelBegin + sNumInLevel + 1;
            
            if( sNumInLevel != m_sGridLevelsCount - sLevel - 1 )
            {
                indexes[i++] = sCurLevelBegin + sNumInLevel + 1;
                indexes[i++] = sNextLevelBegin + sNumInLevel + 1;
                indexes[i++] = sNextLevelBegin + sNumInLevel;
            }
        }
        
        // заканчиваем
        sCurLevelBegin = sNextLevelBegin;
    }
    
    CSharedIndexBuffer indexBuffer = std::make_shared<CIndexBuffer>(indexes.size(), GL_STATIC_DRAW);
    ui16* indexData = indexBuffer->lock();
    for (ui32 i = 0; i < indexes.size(); ++i) {
        indexData[i] = indexes.at(i);
    }
    indexBuffer->unlock();
    
    /*ui32 index = 0;
    for(ui32 i = 0; i < numCols; i++)
    {
        const f32 offsetXZ = cosf(1.0) * static_cast<f32>(i) / static_cast<f32>(numCols - 1);
        for(ui32 j = 0; j < numRows; j++)
        {
            const f32 offsetY = (M_PI * 2.0) * static_cast<f32>(j) / static_cast<f32>(numRows - 1.0) ;
            
            vertexData[index].m_position.x = sinf(offsetXZ) * cosf(offsetY);
            vertexData[index].m_position.y = cosf(offsetXZ);
            vertexData[index].m_position.z = sinf(offsetXZ) * sinf(offsetY);
            vertexData[index].m_position *= m_outerRadius;
            
            vertexData[index].m_texcoord = CVertexBuffer::compressVec2(glm::vec2(j / (numRows - 1.0),
                                                                                 i / (numCols - 1.0)));
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
    indexBuffer->unlock();*/
    
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
        assert(m_materials.find(mode) != m_materials.end());
        
        CSharedMaterial material = m_materials.find(mode)->second;
        assert(material->getShader() != nullptr);
        
        material->getShader()->setMatrix4x4(m_matrixWorld, E_SHADER_UNIFORM_MATRIX_WORLD);
        material->getShader()->setMatrix4x4(m_camera->Get_ProjectionMatrix(), E_SHADER_UNIFORM_MATRIX_PROJECTION);
        material->getShader()->setMatrix4x4(m_camera->Get_ViewMatrix(), E_SHADER_UNIFORM_MATRIX_VIEW);
        material->getShader()->setMatrix4x4(m_camera->Get_MatrixNormal(), E_SHADER_UNIFORM_MATRIX_NORMAL);
        
        static f32 angle = 0.0;
        angle += 0.01;
        glm::vec3 position(0.0);
        position.y = cosf(angle) * -100.0;
        position.x = sinf(angle) * -100.0;
        position.z = 0.;
        
        material->getShader()->setVector3(m_camera->Get_Position(), E_SHADER_UNIFORM_VECTOR_CAMERA_POSITION);
        material->getShader()->setVector3(position, E_SHADER_UNIFORM_VECTOR_LIGHT_01_POSITION);
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
