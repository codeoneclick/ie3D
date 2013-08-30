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
#include "CRenderMgr.h"
#include "CBatchingMgr.h"
#include "CMesh.h"
#include "CSkeleton.h"
#include "CSequence.h"
#include "CAnimationMixer.h"

CModel::CModel(std::shared_ptr<CResourceAccessor> _resourceFabricator) :
IGameObject(_resourceFabricator),
m_animationMixer(nullptr),
m_skeleton(nullptr)
{
    
}

CModel::~CModel(void)
{
    m_sequences.clear();
}

void CModel::_OnTemplateLoaded(std::shared_ptr<ITemplate> _template)
{
    std::shared_ptr<SModelTemplate> modelTemplate = std::static_pointer_cast<SModelTemplate>(_template);
    assert(m_resourceFabricator != nullptr);
    m_mesh = m_resourceFabricator->CreateMesh(modelTemplate->m_meshFilename);
    m_mesh->Register_LoadingHandler(shared_from_this());
    assert(m_mesh != nullptr);
    
    if(modelTemplate->m_skeletonFilename.size() != 0)
    {
        m_skeleton = m_resourceFabricator->CreateSkeleton(modelTemplate->m_skeletonFilename);
        assert(m_skeleton != nullptr);
        m_skeleton->Register_LoadingHandler(shared_from_this());
    }
    
    for(const auto& materialTemplate : modelTemplate->m_materialsTemplates)
    {
        std::shared_ptr<CShader> shader = m_resourceFabricator->CreateShader(materialTemplate->m_shaderTemplate->m_vsFilename,
                                                                             materialTemplate->m_shaderTemplate->m_fsFilename);
        assert(shader != nullptr);
        
        std::shared_ptr<CMaterial> material = std::make_shared<CMaterial>(shader);
        material->Serialize(materialTemplate, m_resourceFabricator, m_renderMgr);
        material->Set_IsBatching(false);
        m_materials.insert(std::make_pair(materialTemplate->m_renderMode, material));
    }
    
    for(const auto& name : modelTemplate->m_sequencesFilenames)
    {
        std::shared_ptr<CSequence> sequence = m_resourceFabricator->CreateSequence(name);
        assert(sequence != nullptr);
        sequence->Set_Name(name);
        m_sequences.insert(sequence);
        sequence->Register_LoadingHandler(shared_from_this());
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
    
    IGameObject::_ListenRenderMgr();
    m_status |= E_LOADING_STATUS_TEMPLATE_LOADED;
}

void CModel::_OnResourceLoaded(std::shared_ptr<IResource> _resource, bool _success)
{
    IGameObject::_OnResourceLoaded(_resource, _success);
    if(((_resource->Get_Class() == E_RESOURCE_CLASS_SKELETON && (m_status & E_LOADING_STATUS_MESH_LOADED)) ||
        (_resource->Get_Class() == E_RESOURCE_CLASS_MESH && (m_status & E_LOADING_STATUS_SKELETON_LOADED))) &&
        m_animationMixer == nullptr)
    {
        m_animationMixer = std::make_shared<CAnimationMixer>(m_mesh, m_skeleton);
        for(const auto& sequence : m_sequences)
        {
            if(sequence->IsCommited() && sequence->IsLoaded())
            {
                m_animationMixer->AddSequence(sequence->Get_Name(), sequence);
            }
        }
    }
    if(_resource->Get_Class() == E_RESOURCE_CLASS_SEQUENCE)
    {
        auto sequence = std::find(m_sequences.begin(), m_sequences.end(), _resource);
        assert(sequence != m_sequences.end());
        if(m_animationMixer != nullptr)
        {
            m_animationMixer->AddSequence((*sequence)->Get_Name(), (*sequence));
        }
    }
    
    if(_resource->Get_Class() == E_RESOURCE_CLASS_MESH)
    {
        if(!m_materials.begin()->second->Get_IsBatching())
        {
            m_mesh->CreateHardwareBuffers(GL_DYNAMIC_DRAW, GL_STATIC_DRAW);
        }
    }
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
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
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
    return 16;
}

void CModel::_OnBind(const std::string& _renderMode)
{
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        assert(m_materials.find(_renderMode) != m_materials.end());
        IGameObject::_OnBind(_renderMode);
    }
}

void CModel::_OnDraw(const std::string& _renderMode)
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
        material->Get_Shader()->Set_Matrix4x4(!material->Get_IsReflected() ? m_camera->Get_ViewMatrix() : m_camera->Get_ViewReflectionMatrix(), E_SHADER_UNIFORM_MATRIX_VIEW);
        
        material->Get_Shader()->Set_Vector3(m_camera->Get_Position(), E_SHADER_UNIFORM_VECTOR_CAMERA_POSITION);
        material->Get_Shader()->Set_Vector3(m_light->Get_Position(), E_SHADER_UNIFORM_VECTOR_LIGHT_POSITION);
        material->Get_Shader()->Set_Vector4(material->Get_Clipping(), E_SHADER_UNIFORM_VECTOR_CLIP_PLANE);
        material->Get_Shader()->Set_Float(m_camera->Get_Near(), E_SHADER_UNIFORM_FLOAT_CAMERA_NEAR);
        material->Get_Shader()->Set_Float(m_camera->Get_Far(), E_SHADER_UNIFORM_FLOAT_CAMERA_FAR);
        if(!material->Get_IsBatching() && m_animationMixer != nullptr)
        {
            material->Get_Shader()->Set_MatrixArray4x4(m_animationMixer->Get_Transformations(), m_animationMixer->Get_TransformationSize(), E_SHADER_UNIFORM_MATRIX_BONES);
            m_animationMixer->OnDraw();
            IGameObject::_OnDraw(_renderMode);
        }
        else if(m_mesh->IsLoaded() && m_animationMixer != nullptr)
        {
            material->Get_Shader()->Set_Matrix4x4(glm::mat4x4(1.0f), E_SHADER_UNIFORM_MATRIX_WORLD);
            m_renderMgr->Get_BatchingMgr()->Batch(std::make_tuple(m_animationMixer->Get_VertexBufferGuid(), m_animationMixer->Get_IndexBufferGuid(), m_mesh), material, m_matrixWorld);
        }
    }
}

void CModel::_OnUnbind(const std::string& _renderMode)
{
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        assert(m_materials.find(_renderMode) != m_materials.end());
        IGameObject::_OnUnbind(_renderMode);
    }
}


