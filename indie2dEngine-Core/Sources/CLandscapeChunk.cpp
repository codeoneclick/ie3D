//
//  CLandscapeChunk.cpp
//  iGaia
//
//  Created by Sergey Sergeev on 3/14/13.
//
//

#include "CLandscapeChunk.h"
#include "CMaterial.h"
#include "CShader.h"
#include "CTexture.h"
#include "CCamera.h"
#include "CLight.h"
#include "CResourceAccessor.h"
#include "CTemplateGameObjects.h"
#include "CAABoundBox.h"
#include "CRenderMgr.h"
#include "CBatchingMgr.h"
#include "CMesh.h"
#include "CQuadTree.h"

CLandscapeChunk::CLandscapeChunk(const std::shared_ptr<CResourceAccessor>& _resourceAccessor, const std::shared_ptr<IScreenSpaceTextureAccessor>& _screenSpaceTextureAccessor) :
IGameObject(_resourceAccessor, _screenSpaceTextureAccessor),
m_width(0),
m_height(0),
m_numIndexesToRender(0)
{
    m_renderQueuePosition = 4;
    
    m_materialImposer = [this](std::shared_ptr<CMaterial> _material)
    {
        _material->Get_Shader()->Set_Matrix4x4(m_isBatching ? glm::mat4x4(1.0f) : m_matrixWorld, E_SHADER_UNIFORM_MATRIX_WORLD);
        _material->Get_Shader()->Set_Matrix4x4(m_camera->Get_ProjectionMatrix(), E_SHADER_UNIFORM_MATRIX_PROJECTION);
        _material->Get_Shader()->Set_Matrix4x4(!_material->Get_IsReflecting() ? m_camera->Get_ViewMatrix() : m_camera->Get_ViewReflectionMatrix(), E_SHADER_UNIFORM_MATRIX_VIEW);
        _material->Get_Shader()->Set_Matrix4x4(m_camera->Get_MatrixNormal(), E_SHADER_UNIFORM_MATRIX_NORMAL);
        
        ui32 count = 0;
        for(ui32 i = 0; i < E_LIGHT_MAX; ++i)
        {
            if(m_lights[i] != nullptr)
            {
                _material->Get_Shader()->Set_Vector3(m_lights[i]->Get_Position(), static_cast<E_SHADER_UNIFORM>(E_SHADER_UNIFORM_VECTOR_LIGHT_01_POSITION + i));
                count++;
            }
        }
        
        _material->Get_Shader()->Set_Vector3(m_camera->Get_Position(), E_SHADER_UNIFORM_VECTOR_CAMERA_POSITION);
        _material->Get_Shader()->Set_Vector4(_material->Get_ClippingPlane(), E_SHADER_UNIFORM_VECTOR_CLIP_PLANE);
        _material->Get_Shader()->Set_Float(m_camera->Get_Near(), E_SHADER_UNIFORM_FLOAT_CAMERA_NEAR);
        _material->Get_Shader()->Set_Float(m_camera->Get_Far(), E_SHADER_UNIFORM_FLOAT_CAMERA_FAR);
    };
}

CLandscapeChunk::~CLandscapeChunk(void)
{
    
}

void CLandscapeChunk::_Set_Mesh(const std::shared_ptr<CMesh> &_mesh, ui32 _width, ui32 _height)
{
    assert(m_mesh == nullptr);
    assert(_mesh != nullptr);
    m_mesh = _mesh;
    m_width = _width;
    m_height = _height;
}

void CLandscapeChunk::_Set_SplattingDiffuseTexture(const std::shared_ptr<CTexture>& _texture)
{
    for(const auto& material : m_materials)
    {
        material.second->Set_Texture(_texture, E_SHADER_SAMPLER_01);
    }
}

void CLandscapeChunk::_Set_SplattingNormalTexture(const std::shared_ptr<CTexture>& _texture)
{
    for(const auto& material : m_materials)
    {
        material.second->Set_Texture(_texture, E_SHADER_SAMPLER_02);
    }
}

void CLandscapeChunk::_OnTemplateLoaded(std::shared_ptr<I_RO_TemplateCommon> _template)
{
    std::shared_ptr<CLandscapeTemplate> landscapeTemplate = std::static_pointer_cast<CLandscapeTemplate>(_template);
    assert(m_resourceAccessor != nullptr);
    
    for(const auto& iterator : landscapeTemplate->Get_MaterialsTemplates())
    {
        std::shared_ptr<CMaterialTemplate> materialTemplate = std::static_pointer_cast<CMaterialTemplate>(iterator);
        std::shared_ptr<CMaterial> material = std::make_shared<CMaterial>(materialTemplate->Get_RenderOperationName());
		material->Serialize(materialTemplate, m_resourceAccessor, m_screenSpaceTextureAccessor, shared_from_this());
        m_materials.insert(std::make_pair(materialTemplate->Get_RenderOperationName(), material));
        CLandscapeChunk::_OnResourceLoaded(material, true);
    }
    
    m_boundBox = m_mesh->CreateBoundBox();
    assert(m_boundBox != nullptr);
    
    m_quadTree = std::make_shared<CQuadTree>();
    m_quadTree->Generate(m_mesh->Get_VertexBuffer(), m_mesh->Get_IndexBuffer(), m_mesh->Get_MaxBound(), m_mesh->Get_MinBound(), 4, m_width);
    
	IGameObject::ListenRenderMgr(m_isNeedToRender);
    m_status |= E_LOADING_STATUS_TEMPLATE_LOADED;
    IGameObject::_OnTemplateLoaded(_template);
}

void CLandscapeChunk::_OnResourceLoaded(std::shared_ptr<IResource> _resource, bool _success)
{
    IGameObject::_OnResourceLoaded(_resource, _success);
}

void CLandscapeChunk::_OnSceneUpdate(f32 _deltatime)
{
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        IGameObject::_OnSceneUpdate(_deltatime);
        m_numIndexesToRender = m_quadTree->OnUpdate(m_camera->Get_Frustum());
    }
}

void CLandscapeChunk::_OnBatch(const std::string& _mode)
{
    
}

i32 CLandscapeChunk::_OnQueuePosition(void)
{
    return m_renderQueuePosition;
}

void CLandscapeChunk::_OnBind(const std::string& _mode)
{
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        assert(m_materials.find(_mode) != m_materials.end());
        IGameObject::_OnBind(_mode);
    }
}

void CLandscapeChunk::_OnDraw(const std::string& _mode)
{
    if((m_status & E_LOADING_STATUS_TEMPLATE_LOADED) && m_numIndexesToRender != 0)
    {
        assert(m_camera != nullptr);
        assert(m_materials.find(_mode) != m_materials.end());
        
        std::shared_ptr<CMaterial> material = m_materials.find(_mode)->second;
        assert(material->Get_Shader() != nullptr);
        
        m_materialImposer(material);
        
        assert(m_mesh != nullptr);
        m_mesh->Draw(m_numIndexesToRender);
    }
}

void CLandscapeChunk::_OnUnbind(const std::string& _mode)
{
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        assert(m_materials.find(_mode) != m_materials.end());
        IGameObject::_OnUnbind(_mode);
    }
}
