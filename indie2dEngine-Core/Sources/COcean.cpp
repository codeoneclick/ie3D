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
#include "ITemplate.h"
#include "CMesh.h"
#include "CVertexBuffer.h"
#include "CIndexBuffer.h"

COcean::COcean(std::shared_ptr<CResourceAccessor> _resourceFabricator) :
IGameObject(_resourceFabricator)
{

}

COcean::~COcean(void)
{
    
}

void COcean::_OnTemplateLoaded(std::shared_ptr<ITemplate> _template)
{
    std::shared_ptr<SOceanTemplate> oceanTemplate = std::static_pointer_cast<SOceanTemplate>(_template);

    assert(m_resourceFabricator != nullptr);
    
    m_width = oceanTemplate->m_width;
    m_height = oceanTemplate->m_height;
    m_altitude = oceanTemplate->m_altitude;
    
    m_waveGeneratorTimer = 0.0f;
    m_waveGeneratorInterval = oceanTemplate->m_waveGeneratorInterval;
    
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
    
    for(const auto& materialTemplate : oceanTemplate->m_materialsTemplates)
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
    return 0;
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
        assert(m_light != nullptr);
        assert(m_materials.find(_renderMode) != m_materials.end());
        
        std::shared_ptr<CMaterial> material = m_materials.find(_renderMode)->second;
        assert(material->Get_Shader() != nullptr);
        
        material->Get_Shader()->Set_Matrix4x4(m_matrixWorld, E_SHADER_UNIFORM_MATRIX_WORLD);
        material->Get_Shader()->Set_Matrix4x4(m_camera->Get_ProjectionMatrix(), E_SHADER_UNIFORM_MATRIX_PROJECTION);
        material->Get_Shader()->Set_Matrix4x4(m_camera->Get_ViewMatrix(), E_SHADER_UNIFORM_MATRIX_VIEW);
        
        material->Get_Shader()->Set_Vector3(m_camera->Get_Position(), E_SHADER_UNIFORM_VECTOR_CAMERA_POSITION);
        material->Get_Shader()->Set_Vector3(m_light->Get_Position(), E_SHADER_UNIFORM_VECTOR_LIGHT_POSITION);
        material->Get_Shader()->Set_Float(m_camera->Get_Near(), E_SHADER_UNIFORM_FLOAT_CAMERA_NEAR);
        material->Get_Shader()->Set_Float(m_camera->Get_Far(), E_SHADER_UNIFORM_FLOAT_CAMERA_FAR);
        material->Get_Shader()->Set_FloatCustom(m_waveGeneratorTimer, "OUT_Timer");
        
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
