//
//  CModel.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/28/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CModel.h"
#include "CMaterial.h"
#include "CShader.h"
#include "CTexture.h"
#include "CCamera.h"
#include "CLight.h"
#include "CResourceAccessor.h"
#include "ITemplate.h"

CModel::CModel(std::shared_ptr<CResourceAccessor> _resourceFabricator) :
IGameObject(_resourceFabricator)
{
    
}

CModel::~CModel(void)
{
    
}

void CModel::_OnTemplateLoaded(std::shared_ptr<ITemplate> _template)
{
    std::shared_ptr<SModelTemplate> modelTemplate = std::static_pointer_cast<SModelTemplate>(_template);
    
    assert(m_resourceFabricator != nullptr);
    m_mesh = m_resourceFabricator->CreateMesh(modelTemplate->m_meshFilename);
    assert(m_mesh != nullptr);
    
    for(auto materialTemplate : modelTemplate->m_materialsTemplates)
    {
        std::shared_ptr<CShader> shader = m_resourceFabricator->CreateShader(materialTemplate->m_shaderTemplate->m_vsFilename,
                                                                             materialTemplate->m_shaderTemplate->m_fsFilename);
        assert(shader != nullptr);
        
        std::shared_ptr<CMaterial> material = std::make_shared<CMaterial>(shader);
        material->Set_RenderState(E_RENDER_STATE_CULL_MODE, materialTemplate->m_isCullFace);
        material->Set_RenderState(E_RENDER_STATE_DEPTH_TEST, materialTemplate->m_isDepthTest);
        material->Set_RenderState(E_RENDER_STATE_DEPTH_MASK, materialTemplate->m_isDepthMask);
        material->Set_RenderState(E_RENDER_STATE_BLEND_MODE, materialTemplate->m_isBlend);
        
        material->Set_CullFaceMode(materialTemplate->m_cullFaceMode);
        material->Set_BlendFunctionSource(materialTemplate->m_blendFunctionSource);
        material->Set_BlendFunctionDest(materialTemplate->m_blendFunctionDestination);
        
        for(auto textureTemplate : materialTemplate->m_texturesTemplates)
        {
            std::shared_ptr<CTexture> texture = m_resourceFabricator->CreateTexture(textureTemplate->m_filename);
            assert(texture != nullptr);
            texture->Set_Wrap(textureTemplate->m_wrap);
            assert(textureTemplate->m_sampler >= 0 && textureTemplate->m_sampler < E_SHADER_SAMPLER_MAX);
            material->Set_Texture(texture, static_cast<E_SHADER_SAMPLER>(textureTemplate->m_sampler));
        }
        m_materials.insert(std::make_pair(materialTemplate->m_renderMode, material));
    }
    m_isLoaded = true;
}

void CModel::_OnSceneUpdate(f32 _deltatime)
{
    if(m_isLoaded)
    {
        IGameObject::_OnSceneUpdate(_deltatime);
    }
}

i32 CModel::_OnQueuePosition(void)
{
    return 0;
}

void CModel::_OnBind(const std::string& _renderMode)
{
    if(m_isLoaded)
    {
        assert(m_materials.find(_renderMode) != m_materials.end());
        IGameObject::_OnBind(_renderMode);
    }
}

void CModel::_OnDraw(const std::string& _renderMode)
{
    if(m_isLoaded)
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
        material->Get_Shader()->Set_Vector4(glm::vec4(0.0f, 1.0f, 0.0f, FLT_MAX), E_SHADER_UNIFORM_VECTOR_CLIP_PLANE);
        
        IGameObject::_OnDraw(_renderMode);
    }
}

void CModel::_OnUnbind(const std::string& _renderMode)
{
    if(m_isLoaded)
    {
        assert(m_materials.find(_renderMode) != m_materials.end());
        IGameObject::_OnUnbind(_renderMode);
    }
}


