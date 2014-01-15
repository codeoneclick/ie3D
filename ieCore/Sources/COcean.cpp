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
#include "CTemplateGameObjects.h"
#include "CMesh.h"
#include "CVertexBuffer.h"
#include "CIndexBuffer.h"

COcean::COcean(const std::shared_ptr<CResourceAccessor>& _resourceAccessor, const std::shared_ptr<IScreenSpaceTextureAccessor>& _screenSpaceTextureAccessor) :
IGameObject(_resourceAccessor, _screenSpaceTextureAccessor)
{
    m_renderQueuePosition = 0;
}

COcean::~COcean(void)
{
    
}

void COcean::_OnTemplateLoaded(std::shared_ptr<I_RO_TemplateCommon> _template)
{
    std::shared_ptr<COceanTemplate> oceanTemplate = std::static_pointer_cast<COceanTemplate>(_template);

    assert(m_resourceAccessor != nullptr);
    
    m_width = oceanTemplate->Get_Size().x;
    m_height = oceanTemplate->Get_Size().y;
    m_altitude = oceanTemplate->Get_Altitude();
    
    m_waveGeneratorTimer = 0.0f;
    m_waveGeneratorInterval = oceanTemplate->Get_WaveGenerationInterval();
    
    std::shared_ptr<CVertexBuffer> vertexBuffer = std::make_shared<CVertexBuffer>(4, GL_STATIC_DRAW);
    SHardwareVertex* vertexData = vertexBuffer->Lock();
    
    vertexData[0].m_position = glm::vec3(0.0f,  m_altitude,  0.0f);
    vertexData[1].m_position = glm::vec3(m_width, m_altitude,  0.0f);
    vertexData[2].m_position = glm::vec3(m_width, m_altitude,  m_height);
    vertexData[3].m_position = glm::vec3(0.0f,  m_altitude,  m_height);
    
    m_textureTileFactor = 8.0f;
    
    vertexData[0].m_texcoord = CVertexBuffer::CompressVec2(glm::vec2(0.0f,  0.0f));
    vertexData[1].m_texcoord = CVertexBuffer::CompressVec2(glm::vec2(1.0f,  0.0f));
    vertexData[2].m_texcoord = CVertexBuffer::CompressVec2(glm::vec2(1.0f,  1.0f));
    vertexData[3].m_texcoord = CVertexBuffer::CompressVec2(glm::vec2(0.0f,  1.0f));
    
    vertexBuffer->Unlock();
    
    std::shared_ptr<CIndexBuffer> indexBuffer = std::make_shared<CIndexBuffer>(6, GL_STATIC_DRAW);
    ui16* indexData = indexBuffer->Lock();
    
    indexData[0] = 0;
    indexData[1] = 1;
    indexData[2] = 2;
    indexData[3] = 0;
    indexData[4] = 2;
    indexData[5] = 3;
    
    indexBuffer->Unlock();
    
    m_mesh = std::make_shared<CMesh>("ocean", vertexBuffer, indexBuffer);
    assert(m_mesh != nullptr);
    
    for(const auto& iterator : oceanTemplate->Get_MaterialsTemplates())
    {
        std::shared_ptr<CMaterialTemplate> materialTemplate = std::static_pointer_cast<CMaterialTemplate>(iterator);
        std::shared_ptr<CMaterial> material = std::make_shared<CMaterial>(materialTemplate->Get_RenderOperationName());
        material->Serialize(materialTemplate, m_resourceAccessor, m_screenSpaceTextureAccessor, shared_from_this());
        m_materials.insert(std::make_pair(materialTemplate->Get_RenderOperationName(), material));
        COcean::_OnResourceLoaded(material, true);
    }
    
	IGameObject::ListenRenderMgr(m_isNeedToRender);
    m_status |= E_LOADING_STATUS_TEMPLATE_LOADED;
    IGameObject::_OnTemplateLoaded(_template);
}

void COcean::_OnResourceLoaded(std::shared_ptr<IResource> _resource, bool _success)
{
    IGameObject::_OnResourceLoaded(_resource, _success);
}

void COcean::_OnSceneUpdate(f32 _deltatime)
{
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        m_waveGeneratorInterval = m_waveGeneratorTimer > 2.0 ? -m_waveGeneratorInterval : m_waveGeneratorInterval;
        m_waveGeneratorInterval = m_waveGeneratorTimer < 0.0 ? -m_waveGeneratorInterval : m_waveGeneratorInterval;
        m_waveGeneratorTimer += m_waveGeneratorInterval;
        IGameObject::_OnSceneUpdate(_deltatime);
    }
}

i32 COcean::_OnQueuePosition(void)
{
    return m_renderQueuePosition;
}

void COcean::_OnBind(const std::string& _renderMode)
{
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        assert(m_materials.find(_renderMode) != m_materials.end());
        IGameObject::_OnBind(_renderMode);
    }
}

void COcean::_OnDraw(const std::string& _renderMode)
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
        material->Get_Shader()->Set_Matrix4x4(m_camera->Get_MatrixNormal(), E_SHADER_UNIFORM_MATRIX_NORMAL);
        
        ui32 count = 0;
        for(ui32 i = 0; i < E_LIGHT_MAX; ++i)
        {
            if(m_lights[i] != nullptr)
            {
                material->Get_Shader()->Set_Vector3(m_lights[i]->Get_Position(), static_cast<E_SHADER_UNIFORM>(E_SHADER_UNIFORM_VECTOR_LIGHT_01_POSITION + i));
                count++;
            }
        }
        
        material->Get_Shader()->Set_Vector3(m_camera->Get_Position(), E_SHADER_UNIFORM_VECTOR_CAMERA_POSITION);
        material->Get_Shader()->Set_Float(m_camera->Get_Near(), E_SHADER_UNIFORM_FLOAT_CAMERA_NEAR);
        material->Get_Shader()->Set_Float(m_camera->Get_Far(), E_SHADER_UNIFORM_FLOAT_CAMERA_FAR);
        material->Get_Shader()->Set_Float(m_waveGeneratorTimer, E_SHADER_UNIFORM_FLOAT_TIMER);
        material->Get_Shader()->Set_Int(count, E_SHADER_UNIFORM_INT_LIGHTS_COUNT);
        
        IGameObject::_OnDraw(_renderMode);
    }
}

void COcean::_OnUnbind(const std::string& _renderMode)
{
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        assert(m_materials.find(_renderMode) != m_materials.end());
        IGameObject::_OnUnbind(_renderMode);
    }
}
