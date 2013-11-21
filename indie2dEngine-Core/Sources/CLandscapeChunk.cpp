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
#include "ITemplate.h"
#include "CAABoundBox.h"
#include "CRenderMgr.h"
#include "CBatchingMgr.h"
#include "CMesh.h"
#include "CQuadTree.h"

CLandscapeChunk::CLandscapeChunk(const std::shared_ptr<CResourceAccessor>& _resourceAccessor, const std::shared_ptr<IScreenSpaceTextureAccessor>& _screenSpaceTextureAccessor) :
IGameObject(_resourceAccessor, _screenSpaceTextureAccessor),
m_width(0),
m_height(0)
{
    
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

void CLandscapeChunk::_OnTemplateLoaded(std::shared_ptr<ITemplate> _template)
{
    std::shared_ptr<SLandscapeTemplate> landscapeTemplate = std::static_pointer_cast<SLandscapeTemplate>(_template);
    assert(m_resourceAccessor != nullptr);
    
    for(const auto& materialTemplate : landscapeTemplate->m_materialsTemplates)
    {
        std::shared_ptr<CShader> shader = m_resourceAccessor->CreateShader(materialTemplate->m_shaderTemplate->m_vsFilename,
                                                                           materialTemplate->m_shaderTemplate->m_fsFilename);
        assert(shader != nullptr);
        shader->Register_LoadingHandler(shared_from_this());
        std::shared_ptr<CMaterial> material = std::make_shared<CMaterial>(shader, materialTemplate->m_renderMode);
		material->Serialize(materialTemplate, m_resourceAccessor, m_screenSpaceTextureAccessor, shared_from_this());
        m_materials.insert(std::make_pair(materialTemplate->m_renderMode, material));
        CLandscapeChunk::_OnResourceLoaded(material, true);
    }
    
    m_boundBox = m_mesh->CreateBoundBox();
    assert(m_boundBox != nullptr);
    
    m_quadTree = std::make_shared<CQuadTree>();
    m_quadTree->Generate(m_mesh->Get_VertexBuffer(), m_mesh->Get_IndexBuffer(), m_mesh->Get_MaxBound(), m_mesh->Get_MinBound(), 4, m_width);
    
	IGameObject::ListenRenderMgr(m_isNeedToRender);
    m_status |= E_LOADING_STATUS_TEMPLATE_LOADED;
    
    std::set<std::string> modes;
    for(auto material : m_materials)
    {
        modes.insert(material.first);
    }
    for(TEMPLATE_LOADING_HANDLER handler : m_templateLoadingHandlers)
    {
        (*handler)(modes);
    }
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
        m_quadTree->OnUpdate(m_camera->Get_Frustum());
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
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        assert(m_camera != nullptr);
        assert(m_materials.find(_mode) != m_materials.end());
        
        std::shared_ptr<CMaterial> material = m_materials.find(_mode)->second;
        assert(material->Get_Shader() != nullptr);
        
        m_materialImposer(material);
        IGameObject::_OnDraw(_mode);
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
