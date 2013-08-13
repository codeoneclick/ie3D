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
#include "CAABoundBox.h"
#include "CMesh.h"
#include "CAnimationMixer.h"

CModel::CModel(std::shared_ptr<CResourceAccessor> _resourceFabricator) :
IGameObject(_resourceFabricator),
m_animationMixer(nullptr)
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
    
    if(modelTemplate->m_skeletonFilename.size() != 0)
    {
        std::shared_ptr<CSkeleton> skeleton = m_resourceFabricator->CreateSkeleton(modelTemplate->m_skeletonFilename);
        assert(skeleton != nullptr);
        m_animationMixer = std::make_shared<CAnimationMixer>(m_mesh, skeleton);
    }
    
    for(auto materialTemplate : modelTemplate->m_materialsTemplates)
    {
        std::shared_ptr<CShader> shader = m_resourceFabricator->CreateShader(materialTemplate->m_shaderTemplate->m_vsFilename,
                                                                             materialTemplate->m_shaderTemplate->m_fsFilename);
        assert(shader != nullptr);
        
        std::shared_ptr<CMaterial> material = std::make_shared<CMaterial>(shader);
        material->Serialize(materialTemplate, m_resourceFabricator, m_renderMgr);
        m_materials.insert(std::make_pair(materialTemplate->m_renderMode, material));
    }
    
    for(auto animationTemplate : modelTemplate->m_sequencesFilenames)
    {
        std::shared_ptr<CSequence> sequence = m_resourceFabricator->CreateSequence(animationTemplate);
        assert(sequence != nullptr);
        m_animationMixer->AddSequence(animationTemplate, sequence);
    }
    
    m_boundBox = m_mesh->CreateBoundBox();
    assert(m_boundBox != nullptr);
    
    std::shared_ptr<CShader> shader = m_resourceFabricator->CreateShader(k_vsBoundBoxFilename,
                                                                         k_fsBoundBoxFilename);
    assert(shader != nullptr);
    m_debugBoundBoxMaterial = std::make_shared<CMaterial>(shader);
    m_debugBoundBoxMaterial->Set_RenderState(E_RENDER_STATE_CULL_MODE, false);
    m_debugBoundBoxMaterial->Set_RenderState(E_RENDER_STATE_DEPTH_TEST, false);
    m_debugBoundBoxMaterial->Set_RenderState(E_RENDER_STATE_DEPTH_MASK, true);
    m_debugBoundBoxMaterial->Set_RenderState(E_RENDER_STATE_BLEND_MODE, true);
    
    m_debugBoundBoxMaterial->Set_CullFaceMode(GL_FRONT);
    m_debugBoundBoxMaterial->Set_BlendFunctionSource(GL_SRC_ALPHA);
    m_debugBoundBoxMaterial->Set_BlendFunctionDest(GL_ONE_MINUS_SRC_ALPHA);
    
    IGameObject::_LazyListenRenderMgr();
    m_isLoaded = true;
}

void CModel::Set_Animation(const std::string &_name)
{
    if(m_animationMixer != nullptr)
    {
        m_animationMixer->SetAnimation(_name);
    }
}

void CModel::_OnSceneUpdate(f32 _deltatime)
{
    if(m_isLoaded)
    {
        IGameObject::_OnSceneUpdate(_deltatime);
        if(m_animationMixer != nullptr)
        {
            m_animationMixer->OnUpdate(_deltatime);
        }
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
        material->Get_Shader()->Set_Matrix4x4(!material->Get_IsReflected() ? m_camera->Get_ViewMatrix() : m_camera->Get_ViewReflectionMatrix(), E_SHADER_UNIFORM_MATRIX_VIEW);
        
        material->Get_Shader()->Set_Vector3(m_camera->Get_Position(), E_SHADER_UNIFORM_VECTOR_CAMERA_POSITION);
        material->Get_Shader()->Set_Vector3(m_light->Get_Position(), E_SHADER_UNIFORM_VECTOR_LIGHT_POSITION);
        material->Get_Shader()->Set_Vector4(material->Get_Clipping(), E_SHADER_UNIFORM_VECTOR_CLIP_PLANE);
        material->Get_Shader()->Set_Float(m_camera->Get_Near(), E_SHADER_UNIFORM_FLOAT_CAMERA_NEAR);
        material->Get_Shader()->Set_Float(m_camera->Get_Far(), E_SHADER_UNIFORM_FLOAT_CAMERA_FAR);
        
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


