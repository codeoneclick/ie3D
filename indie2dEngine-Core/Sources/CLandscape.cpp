//
//  CLandscape.cpp
//  iGaia
//
//  Created by sergey.sergeev on 1/16/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "CLandscape.h"
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
#include "CLandscapeChunk.h"

CLandscape::CLandscape(const std::shared_ptr<CResourceAccessor>& _resourceAccessor, const std::shared_ptr<IScreenSpaceTextureAccessor>& _screenSpaceTextureAccessor) :
IGameObject(_resourceAccessor, _screenSpaceTextureAccessor),
m_splattingDiffuseMaterial(nullptr),
m_splattingNormalMaterial(nullptr),
m_splattingDiffuseMaterialShader(nullptr),
m_splattingNormalMaterialShader(nullptr)
{

}

CLandscape::~CLandscape(void)
{

}

void CLandscape::_OnTemplateLoaded(std::shared_ptr<ITemplate> _template)
{
    std::shared_ptr<SLandscapeTemplate> landscapeTemplate = std::static_pointer_cast<SLandscapeTemplate>(_template);
    assert(m_resourceAccessor != nullptr);
    assert(m_screenSpaceTextureAccessor != nullptr);
    
    m_heightmapProcessor = std::make_shared<CHeightmapProcessor>(m_screenSpaceTextureAccessor, landscapeTemplate);
    
    m_splattingDiffuseMaterialShader = m_resourceAccessor->CreateShader(landscapeTemplate->m_splattingDiffuseMaterial->m_shaderTemplate->m_vsFilename,
                                                                        landscapeTemplate->m_splattingDiffuseMaterial->m_shaderTemplate->m_fsFilename);
    assert(m_splattingDiffuseMaterialShader != nullptr);
    m_splattingDiffuseMaterial = std::make_shared<CMaterial>(m_splattingDiffuseMaterialShader, landscapeTemplate->m_splattingDiffuseMaterial->m_renderMode);
    m_splattingDiffuseMaterial->Serialize(landscapeTemplate->m_splattingDiffuseMaterial, m_resourceAccessor, m_screenSpaceTextureAccessor, shared_from_this());
    CLandscape::_OnResourceLoaded(m_splattingDiffuseMaterial, true);
    m_splattingDiffuseMaterialShader->Register_LoadingHandler(shared_from_this());
    
    m_splattingNormalMaterialShader = m_resourceAccessor->CreateShader(landscapeTemplate->m_splattingNormalMaterial->m_shaderTemplate->m_vsFilename,
                                                                       landscapeTemplate->m_splattingNormalMaterial->m_shaderTemplate->m_fsFilename);
    assert(m_splattingNormalMaterialShader != nullptr);
    m_splattingNormalMaterial = std::make_shared<CMaterial>(m_splattingNormalMaterialShader, landscapeTemplate->m_splattingNormalMaterial->m_renderMode);
    m_splattingNormalMaterial->Serialize(landscapeTemplate->m_splattingNormalMaterial, m_resourceAccessor, m_screenSpaceTextureAccessor, shared_from_this());
    CLandscape::_OnResourceLoaded(m_splattingNormalMaterial, true);
    m_splattingNormalMaterialShader->Register_LoadingHandler(shared_from_this());
    
    m_heightmapProcessor->PreprocessSplattingTexture();
    m_heightmapProcessor->PreprocessHeightmapTexture();

    m_numChunkRows = m_heightmapProcessor->Get_NumChunkRows();
    m_numChunkCells = m_heightmapProcessor->Get_NumChunkCells();
    
    ui32 chunkWidth = m_heightmapProcessor->Get_ChunkWidth();
    ui32 chunkHeight = m_heightmapProcessor->Get_ChunkHeight();
    
    m_chunks.resize(m_numChunkRows * m_numChunkCells);
    
    for(ui32 i = 0; i < m_numChunkRows; ++i)
    {
        for(ui32 j = 0; j < m_numChunkCells; ++j)
        {
            std::shared_ptr<CMesh> mesh = m_heightmapProcessor->Get_Chunk(i, j);
            m_chunks[i + j * m_numChunkRows] = std::make_shared<CLandscapeChunk>(m_resourceAccessor, m_screenSpaceTextureAccessor);
            m_chunks[i + j * m_numChunkRows]->_Set_Mesh(mesh, chunkWidth, chunkHeight);
            m_chunks[i + j * m_numChunkRows]->_OnTemplateLoaded(_template);
        }
    }
    
    CLandscape::Set_Camera(m_camera);
    CLandscape::Set_RenderMgr(m_renderMgr);
    CLandscape::Set_SceneUpdateMgr(m_sceneUpdateMgr);
	CLandscape::ListenRenderMgr(m_isNeedToRender);
    CLandscape::ListenSceneUpdateMgr(m_isNeedToUpdate);
    
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

void CLandscape::_OnResourceLoaded(std::shared_ptr<IResource> _resource, bool _success)
{
    IGameObject::_OnResourceLoaded(_resource, _success);
}

void CLandscape::_OnSceneUpdate(f32 _deltatime)
{
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        IGameObject::_OnSceneUpdate(_deltatime);
    }
}

void CLandscape::_OnBatch(const std::string& _mode)
{
    
}

i32 CLandscape::_OnQueuePosition(void)
{
    return m_renderQueuePosition;
}

void CLandscape::_OnBind(const std::string& _mode)
{
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        assert(m_materials.find(_mode) != m_materials.end());
        IGameObject::_OnBind(_mode);
    }
}

void CLandscape::_OnDraw(const std::string& _mode)
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

void CLandscape::_OnUnbind(const std::string& _mode)
{
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        assert(m_materials.find(_mode) != m_materials.end());
        IGameObject::_OnUnbind(_mode);
    }
}

void CLandscape::Set_Camera(std::shared_ptr<CCamera> _camera)
{
    IGameObject::Set_Camera(_camera);
    
    for(ui32 i = 0; i < m_numChunkRows; ++i)
    {
        for(ui32 j = 0; j < m_numChunkCells; ++j)
        {
            assert(m_chunks.size() != 0);
            assert(m_chunks[i + j * m_numChunkRows] != nullptr);
            m_chunks[i + j * m_numChunkRows]->Set_Camera(_camera);
        }
    }
}

void CLandscape::Set_Light(std::shared_ptr<CLight> _light, E_LIGHTS _id)
{
    IGameObject::Set_Light(_light, _id);
    
    for(ui32 i = 0; i < m_numChunkRows; ++i)
    {
        for(ui32 j = 0; j < m_numChunkCells; ++j)
        {
            assert(m_chunks.size() != 0);
            assert(m_chunks[i + j * m_numChunkRows] != nullptr);
            m_chunks[i + j * m_numChunkRows]->Set_Light(_light, _id);
        }
    }
}

void CLandscape::Set_RenderMgr(std::shared_ptr<CRenderMgr> _renderMgr)
{
    IGameObject::Set_RenderMgr(_renderMgr);
    
    for(ui32 i = 0; i < m_numChunkRows; ++i)
    {
        for(ui32 j = 0; j < m_numChunkCells; ++j)
        {
            assert(m_chunks.size() != 0);
            assert(m_chunks[i + j * m_numChunkRows] != nullptr);
            m_chunks[i + j * m_numChunkRows]->Set_RenderMgr(_renderMgr);
        }
    }
}

void CLandscape::Set_SceneUpdateMgr(std::shared_ptr<CSceneUpdateMgr> _sceneUpdateMgr)
{
    IGameObject::Set_SceneUpdateMgr(_sceneUpdateMgr);
    
    for(ui32 i = 0; i < m_numChunkRows; ++i)
    {
        for(ui32 j = 0; j < m_numChunkCells; ++j)
        {
            assert(m_chunks.size() != 0);
            assert(m_chunks[i + j * m_numChunkRows] != nullptr);
            m_chunks[i + j * m_numChunkRows]->Set_SceneUpdateMgr(_sceneUpdateMgr);
        }
    }
}

void CLandscape::ListenRenderMgr(bool _value)
{
    m_isNeedToRender = _value;
    
    for(ui32 i = 0; i < m_numChunkRows; ++i)
    {
        for(ui32 j = 0; j < m_numChunkCells; ++j)
        {
            assert(m_chunks.size() != 0);
            assert(m_chunks[i + j * m_numChunkRows] != nullptr);
            m_chunks[i + j * m_numChunkRows]->ListenRenderMgr(_value);
        }
    }
}

void CLandscape::ListenSceneUpdateMgr(bool _value)
{
    m_isNeedToUpdate = _value;
    
    for(ui32 i = 0; i < m_numChunkRows; ++i)
    {
        for(ui32 j = 0; j < m_numChunkCells; ++j)
        {
            assert(m_chunks.size() != 0);
            assert(m_chunks[i + j * m_numChunkRows] != nullptr);
            m_chunks[i + j * m_numChunkRows]->ListenSceneUpdateMgr(_value);
        }
    }
}

