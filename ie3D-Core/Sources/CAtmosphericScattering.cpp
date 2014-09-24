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
        ui32 index = 0;
        for(ui32 x = 0; x < m_size; x++)
        {
            const f32 offsetXZ = cosf( 1.0f ) * x / static_cast<f32>(m_size - 1.0);
            for(ui32 y = 0; y < m_size; y++)
            {
                const f32 offsetY = (M_PI * 2.0) * static_cast<f32>(y) / static_cast<f32>(m_size - 1.0);
                glm::vec3 position = glm::vec3(0.0, m_innerRadius + 1e-6, 0.0);
                
                position.x = sinf(offsetXZ) * cosf(offsetY) * m_outerRadius;
                position.y = cosf(offsetXZ) * m_outerRadius;
                position.z = sinf(offsetXZ) * sinf(offsetY) * m_outerRadius;
                
                glm::vec3 origin = position;
                glm::vec3 direction = origin - glm::vec3(0.0, m_innerRadius  + 1e-6, 0.0);
                f32 far = direction.length();
                direction /= far;
                
                glm::vec3 start = glm::vec3(0.0, m_innerRadius  + 1e-6, 0.0);
                f32 height = start.length();
                f32 depth = expf(m_scale_02 * (m_innerRadius - start.y));
                f32 startAngle = glm::dot(direction, start) / height;
                f32 startOffset = depth * CAtmosphericScattering::scaleAngle(startAngle);
                
                float fSampleLength = fFar / m_nSamples;
                float fScaledLength = fSampleLength * m_fScale;
                math::Vector3d v3SampleRay = v3Ray * fSampleLength;
                math::Vector3d v3SamplePoint = v3Start + v3SampleRay * 0.5f;
                
                math::Vector3d v3FrontColor = math::Vector3d(0.0f, 0.0f, 0.0f);
                for(unsigned int i=0; i<m_nSamples; i++)
                {
                    float fHeight = v3SamplePoint.length();
                    float fDepth = exp(m_fScaleOverScaleDepth * (m_fInnerRadius - fHeight));
                    float fLightAngle = math::dot(m_LightRef->GetDirection(),v3SamplePoint) / fHeight;
                    float fCameraAngle = math::dot(v3Ray, v3SamplePoint) / fHeight;
                    float fScatter = (fStartOffset + fDepth*(scale(fLightAngle) - scale(fCameraAngle)));
                    math::Vector3d v3Attenuate = expv((m_vInvWavelength4 * m_fKr4PI + math::Vector3d(m_fKm4PI, m_fKm4PI, m_fKm4PI)) * -fScatter);
                    v3FrontColor += v3Attenuate * (fDepth * fScaledLength);
                    v3SamplePoint += v3SampleRay;
                }
                
                if(x == m_nSize / 2)
                    Game::GetEnviromentControllerInstance()->GetCameraInstance()->m_FogColor = v3FrontColor;
                
                pBuffer[nIndex].x = v3FrontColor.x;//min( v3FrontColor.x, 6.5519996e4f);
                pBuffer[nIndex].y = v3FrontColor.y;//min( v3FrontColor.y, 6.5519996e4f);
                pBuffer[nIndex].z = v3FrontColor.z;//min( v3FrontColor.z, 6.5519996e4f);
                pBuffer[nIndex].w = 0.0f;
                
                nIndex++;
            }
        }
        m_MeshList[m_Name]->m_TextureArray[0]->Unlock();*/
        
        IGameObject::setPosition(m_camera->Get_Position());
        IGameObject::onSceneUpdate(deltatime);
    }
}

f32 CAtmosphericScattering::scaleAngle(f32 value)
{
    f32 x = 1.0 - value;
    return m_rayleighScaleDepth * exp(-0.00287 + x * (0.459 + x * (3.83 + x * (-6.80 + x * 5.25))));
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
    m_scale_01 = 1.0 / (m_outerRadius - m_innerRadius);
    m_rayleighScaleDepth = atmosphericScatteringConfiguration->getRayleighScaleDepthCoefficient();
    m_scale_02 = m_scale_01 / m_rayleighScaleDepth;
    
    CSharedVertexBuffer vertexBuffer = std::make_shared<CVertexBuffer>(numCols * numRows, GL_STATIC_DRAW);
    SAttributeVertex* vertexData = vertexBuffer->lock();
    
    ui32 index = 0;
    for(ui32 i = 0; i < numCols; i++)
    {
        const f32 offsetXZ = cosf(1.0) * static_cast<f32>(i) / static_cast<f32>(numCols - 1);
        for(ui32 j = 0; j < numRows; j++)
        {
            const f32 offsetY = (M_PI * 2.0) * static_cast<f32>(j) / static_cast<f32>(numRows - 1.0) ;
            
            vertexData[index].m_position.x = sinf(offsetXZ) * cosf(offsetY);
            vertexData[index].m_position.y = cosf(offsetXZ);
            vertexData[index].m_position.z = sinf(offsetXZ) * sinf(offsetY);
            vertexData[index].m_position *= outerRadius;
            
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
        assert(m_materials.find(mode) != m_materials.end());
        
        CSharedMaterial material = m_materials.find(mode)->second;
        assert(material->getShader() != nullptr);
        
        glm::vec3 currentRotation = m_rotation;
        if(material->isReflecting())
        {
            IGameObject::setRotation(glm::vec3(180.0, -glm::degrees(m_camera->Get_Rotation()) * 2.0, m_rotation.z));
            IGameObject::onSceneUpdate(0);
        }
        
        material->getShader()->setMatrix4x4(m_matrixWorld, E_SHADER_UNIFORM_MATRIX_WORLD);
        material->getShader()->setMatrix4x4(m_camera->Get_ProjectionMatrix(), E_SHADER_UNIFORM_MATRIX_PROJECTION);
        material->getShader()->setMatrix4x4(m_camera->Get_ViewMatrix(), E_SHADER_UNIFORM_MATRIX_VIEW);
        material->getShader()->setMatrix4x4(m_camera->Get_MatrixNormal(), E_SHADER_UNIFORM_MATRIX_NORMAL);
        
        material->getShader()->setVector3(m_camera->Get_Position(), E_SHADER_UNIFORM_VECTOR_CAMERA_POSITION);
        material->getShader()->setFloat(m_camera->Get_Near(), E_SHADER_UNIFORM_FLOAT_CAMERA_NEAR);
        material->getShader()->setFloat(m_camera->Get_Far(), E_SHADER_UNIFORM_FLOAT_CAMERA_FAR);
        
        IGameObject::onDraw(mode);
        
        if(material->isReflecting())
        {
            IGameObject::setRotation(currentRotation);
            IGameObject::onSceneUpdate(0);
        }
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
