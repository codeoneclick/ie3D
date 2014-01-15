//
//  CLandscapeEdges.cpp
//  iGaia
//
//  Created by sergey sergeev on 7/11/12.
//
//

#include "CLandscapeEdges.h"
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

CLandscapeEdges::CLandscapeEdges(const std::shared_ptr<CResourceAccessor>& _resourceAccessor, const std::shared_ptr<IScreenSpaceTextureAccessor>& _screenSpaceTextureAccessor) :
IGameObject(_resourceAccessor, _screenSpaceTextureAccessor),
m_width(0),
m_height(0),
m_heightBounds(glm::vec2(0.0f, 0.0f))
{
    m_renderQueuePosition = 4;
    
    m_materialImposer = [this](std::shared_ptr<CMaterial> _material)
    {
        _material->Get_Shader()->Set_Matrix4x4(m_matrixWorld, E_SHADER_UNIFORM_MATRIX_WORLD);
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

CLandscapeEdges::~CLandscapeEdges(void)
{
    
}

void CLandscapeEdges::_Set_EdgeTexture(const std::shared_ptr<CTexture>& _texture)
{
    for(const auto& material : m_materials)
    {
        material.second->Set_Texture(_texture, E_SHADER_SAMPLER_01);
    }
}

void CLandscapeEdges::_OnTemplateLoaded(std::shared_ptr<I_RO_TemplateCommon> _template)
{
    std::shared_ptr<CLandscapeTemplate> landscapeTemplate = std::static_pointer_cast<CLandscapeTemplate>(_template);
    assert(m_resourceAccessor != nullptr);
    
    m_width = landscapeTemplate->Get_Size().x;
    m_height = landscapeTemplate->Get_Size().y;
    m_heightBounds = glm::vec2(landscapeTemplate->Get_EdgesSize().x, landscapeTemplate->Get_EdgesSize().y);
    
    for(const auto& iterator : landscapeTemplate->Get_EdgesMaterialsTemplates())
    {
        std::shared_ptr<CMaterialTemplate> materialTemplate = std::static_pointer_cast<CMaterialTemplate>(iterator);
        std::shared_ptr<CMaterial> material = std::make_shared<CMaterial>(materialTemplate->Get_RenderOperationName());
		material->Serialize(materialTemplate, m_resourceAccessor, m_screenSpaceTextureAccessor, shared_from_this());
        m_materials.insert(std::make_pair(materialTemplate->Get_RenderOperationName(), material));
        CLandscapeEdges::_OnResourceLoaded(material, true);
    }
    
    std::shared_ptr<CVertexBuffer> vertexBuffer =std::make_shared<CVertexBuffer>(16, GL_STATIC_DRAW);
    SHardwareVertex* vertexData = vertexBuffer->Lock();
    
    glm::vec3 boundMin = glm::vec3(0.0f, m_heightBounds.x, 0.0f);
    glm::vec3 boundMax = glm::vec3(m_width, m_heightBounds.y, m_height);
    
    vertexData[0].m_position = glm::vec3(boundMin.x,  boundMin.y, boundMax.z);
    vertexData[1].m_position = glm::vec3(boundMax.x,  boundMin.y, boundMax.z);
    vertexData[2].m_position = glm::vec3(boundMax.x,  boundMax.y, boundMax.z);
    vertexData[3].m_position = glm::vec3(boundMin.x,  boundMax.y, boundMax.z);
    
    vertexData[4].m_position = glm::vec3(boundMin.x,  boundMin.y,  boundMin.z);
    vertexData[5].m_position = glm::vec3(boundMin.x,  boundMax.y,  boundMin.z);
    vertexData[6].m_position = glm::vec3(boundMax.x,  boundMax.y,  boundMin.z);
    vertexData[7].m_position = glm::vec3(boundMax.x,  boundMin.y,  boundMin.z);
    
    vertexData[8].m_position = glm::vec3(boundMax.x,  boundMin.y,   boundMax.z);
    vertexData[9].m_position = glm::vec3(boundMax.x,  boundMin.y,   boundMin.z);
    vertexData[10].m_position = glm::vec3(boundMax.x,  boundMax.y,  boundMin.z);
    vertexData[11].m_position = glm::vec3(boundMax.x,  boundMax.y,  boundMax.z);
    
    vertexData[12].m_position = glm::vec3(boundMin.x,  boundMin.y,  boundMin.z);
    vertexData[13].m_position = glm::vec3(boundMin.x,  boundMin.y,  boundMax.z);
    vertexData[14].m_position = glm::vec3(boundMin.x,  boundMax.y,  boundMax.z);
    vertexData[15].m_position = glm::vec3(boundMin.x,  boundMax.y,  boundMin.z);
    
    vertexData[0].m_texcoord = CVertexBuffer::CompressVec2(glm::vec2(0.0f, 1.0f / 4.0f));
    vertexData[1].m_texcoord = CVertexBuffer::CompressVec2(glm::vec2(1.0f - 0.001f, 1.0f / 4.0f));
    vertexData[2].m_texcoord = CVertexBuffer::CompressVec2(glm::vec2(1.0f - 0.001f, (1.0f / 4.0f) * 2.0f - 0.001f));
    vertexData[3].m_texcoord = CVertexBuffer::CompressVec2(glm::vec2(0.0f, (1.0f / 4.0f) * 2.0f - 0.001f));
    
    vertexData[4].m_texcoord = CVertexBuffer::CompressVec2(glm::vec2(1.0f - 0.001f, 0.0f));
    vertexData[5].m_texcoord = CVertexBuffer::CompressVec2(glm::vec2(1.0f - 0.001f, 1.0f / 4.0f - 0.001f));
    vertexData[6].m_texcoord = CVertexBuffer::CompressVec2(glm::vec2(0.0f, 1.0f / 4.0f - 0.001f));
    vertexData[7].m_texcoord = CVertexBuffer::CompressVec2(glm::vec2(0.0f, 0.0f));
    
    vertexData[8].m_texcoord =  CVertexBuffer::CompressVec2(glm::vec2(0.0f, (1.0f / 4.0f) * 3.0f));
    vertexData[9].m_texcoord =  CVertexBuffer::CompressVec2(glm::vec2(1.0f - 0.001f, (1.0f / 4.0f) * 3.0f));
    vertexData[10].m_texcoord = CVertexBuffer::CompressVec2(glm::vec2(1.0f - 0.001f, 1.0f - 0.001f));
    vertexData[11].m_texcoord = CVertexBuffer::CompressVec2(glm::vec2(0.0f, 1.0f - 0.001f));
    
    vertexData[12].m_texcoord = CVertexBuffer::CompressVec2(glm::vec2(0.0f, (1.0f / 4.0f) * 2.0f));
    vertexData[13].m_texcoord = CVertexBuffer::CompressVec2(glm::vec2(1.0f - 0.001f, (1.0f / 4.0f) * 2.0f));
    vertexData[14].m_texcoord = CVertexBuffer::CompressVec2(glm::vec2(1.0f - 0.001f, (1.0f / 4.0f) * 3.0f - 0.001f));
    vertexData[15].m_texcoord = CVertexBuffer::CompressVec2(glm::vec2(0.0f, (1.0f / 4.0f) * 3.0f - 0.001f));
    
    vertexBuffer->Unlock();
    
    std::shared_ptr<CIndexBuffer> indexBuffer = std::make_shared<CIndexBuffer>(24, GL_STATIC_DRAW);
    ui16* indexData = indexBuffer->Lock();
    
    indexData[0] = 0;
    indexData[1] = 1;
    indexData[2] = 2;
    indexData[3] = 0;
    indexData[4] = 2;
    indexData[5] = 3;
    
    indexData[6] = 4;
    indexData[7] = 5;
    indexData[8] = 6;
    indexData[9] = 4;
    indexData[10] = 6;
    indexData[11] = 7;
    
    indexData[12] = 8;
    indexData[13] = 9;
    indexData[14] = 10;
    indexData[15] = 8;
    indexData[16] = 10;
    indexData[17] = 11;
    
    indexData[18] = 12;
    indexData[19] = 13;
    indexData[20] = 14;
    indexData[21] = 12;
    indexData[22] = 14;
    indexData[23] = 15;
    
    indexBuffer->Unlock();
    
    m_mesh = std::make_shared<CMesh>("landscape.edges", vertexBuffer, indexBuffer);
    
	IGameObject::ListenRenderMgr(m_isNeedToRender);
    m_status |= E_LOADING_STATUS_TEMPLATE_LOADED;
    IGameObject::_OnTemplateLoaded(_template);
}

void CLandscapeEdges::_OnResourceLoaded(std::shared_ptr<IResource> _resource, bool _success)
{
    IGameObject::_OnResourceLoaded(_resource, _success);
}

void CLandscapeEdges::_OnSceneUpdate(f32 _deltatime)
{
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        IGameObject::_OnSceneUpdate(_deltatime);
    }
}

void CLandscapeEdges::_OnBatch(const std::string& _mode)
{
    
}

i32 CLandscapeEdges::_OnQueuePosition(void)
{
    return m_renderQueuePosition;
}

void CLandscapeEdges::_OnBind(const std::string& _mode)
{
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        assert(m_materials.find(_mode) != m_materials.end());
        IGameObject::_OnBind(_mode);
    }
}

void CLandscapeEdges::_OnDraw(const std::string& _mode)
{
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        assert(m_camera != nullptr);
        assert(m_materials.find(_mode) != m_materials.end());
        
        std::shared_ptr<CMaterial> material = m_materials.find(_mode)->second;
        assert(material->Get_Shader() != nullptr);
        
        m_materialImposer(material);
        
        assert(m_mesh != nullptr);
        IGameObject::_OnDraw(_mode);
    }
}

void CLandscapeEdges::_OnUnbind(const std::string& _mode)
{
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        assert(m_materials.find(_mode) != m_materials.end());
        IGameObject::_OnUnbind(_mode);
    }
}


