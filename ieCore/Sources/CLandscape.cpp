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
#include "CTemplateGameObjects.h"
#include "CAABoundBox.h"
#include "CRenderMgr.h"
#include "CBatchingMgr.h"
#include "CMesh.h"
#include "CLandscapeChunk.h"
#include "CLandscapeEdges.h"

CLandscape::CLandscape(const std::shared_ptr<CResourceAccessor>& _resourceAccessor, const std::shared_ptr<IScreenSpaceTextureAccessor>& _screenSpaceTextureAccessor) :
IGameObject(_resourceAccessor, _screenSpaceTextureAccessor),
m_splattingDiffuseMaterial(nullptr),
m_splattingNormalMaterial(nullptr),
m_isSplattingDiffuseTextureCommited(false),
m_isSplattingNormalTextureCommited(false),
m_edges(std::make_shared<CLandscapeEdges>(_resourceAccessor, _screenSpaceTextureAccessor))
{

}

CLandscape::~CLandscape(void)
{

}

void CLandscape::_OnTemplateLoaded(std::shared_ptr<I_RO_TemplateCommon> _template)
{
    std::shared_ptr<CLandscapeTemplate> landscapeTemplate = std::static_pointer_cast<CLandscapeTemplate>(_template);
    assert(m_resourceAccessor != nullptr);
    assert(m_screenSpaceTextureAccessor != nullptr);
    
    m_isSplattingDiffuseTextureCommited = false;
    m_isSplattingNormalTextureCommited = false;
    
    m_heightmapProcessor = std::make_shared<CHeightmapProcessor>(m_screenSpaceTextureAccessor, landscapeTemplate);
    
    m_heightmapProcessor->PreprocessSplattingTexture();
    m_heightmapProcessor->PreprocessHeightmapTexture();
    m_heightmapProcessor->PreprocessEdgesMaskTexture();
    
    std::shared_ptr<CMaterialTemplate> materialTemplate = std::static_pointer_cast<CMaterialTemplate>(landscapeTemplate->Get_SplattingDiffuseMaterialTemplate());
    m_splattingDiffuseMaterial = std::make_shared<CMaterial>(materialTemplate->Get_RenderOperationName());
    m_splattingDiffuseMaterial->Serialize(landscapeTemplate->Get_SplattingDiffuseMaterialTemplate(), m_resourceAccessor, m_screenSpaceTextureAccessor, shared_from_this());
    m_splattingDiffuseMaterial->Set_Texture(m_heightmapProcessor->Get_SplattingTexture(), E_SHADER_SAMPLER_04);
    CLandscape::_OnResourceLoaded(m_splattingDiffuseMaterial, true);
    
    materialTemplate = std::static_pointer_cast<CMaterialTemplate>(landscapeTemplate->Get_SplattingNormalMaterialTemplate());
    m_splattingNormalMaterial = std::make_shared<CMaterial>(materialTemplate->Get_RenderOperationName());
    m_splattingNormalMaterial->Serialize(landscapeTemplate->Get_SplattingNormalMaterialTemplate(), m_resourceAccessor, m_screenSpaceTextureAccessor, shared_from_this());
    m_splattingNormalMaterial->Set_Texture(m_heightmapProcessor->Get_SplattingTexture(), E_SHADER_SAMPLER_04);
    CLandscape::_OnResourceLoaded(m_splattingNormalMaterial, true);
    
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
    m_edges->_OnTemplateLoaded(_template);
    m_edges->_Set_EdgeTexture(m_heightmapProcessor->Get_EdgesMaskTexture());
    
    CLandscape::Set_Camera(m_camera);
    CLandscape::Set_RenderMgr(m_renderMgr);
    CLandscape::Set_SceneUpdateMgr(m_sceneUpdateMgr);
	CLandscape::ListenRenderMgr(m_isNeedToRender);
    CLandscape::ListenSceneUpdateMgr(m_isNeedToUpdate);
    
    m_status |= E_LOADING_STATUS_TEMPLATE_LOADED;
    IGameObject::_OnTemplateLoaded(_template);
}

void CLandscape::_OnResourceLoaded(std::shared_ptr<IResource> _resource, bool _success)
{
    IGameObject::_OnResourceLoaded(_resource, _success);
}

void CLandscape::_OnSceneUpdate(f32 _deltatime)
{
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        assert(m_splattingDiffuseMaterial != nullptr);
        if(!m_isSplattingDiffuseTextureCommited && m_splattingDiffuseMaterial->IsCommited())
        {
            assert(m_heightmapProcessor != nullptr);
            std::shared_ptr<CTexture> splattingDiffuseTexture = m_heightmapProcessor->PreprocessSplattingDiffuseTexture(m_splattingDiffuseMaterial);
            for(ui32 i = 0; i < m_numChunkRows; ++i)
            {
                for(ui32 j = 0; j < m_numChunkCells; ++j)
                {
                    assert(m_chunks.size() != 0);
                    assert(m_chunks[i + j * m_numChunkRows] != nullptr);
                    m_chunks[i + j * m_numChunkRows]->_Set_SplattingDiffuseTexture(splattingDiffuseTexture);
                }
            }
            m_isSplattingDiffuseTextureCommited = true;
        }
        
        assert(m_splattingNormalMaterial != nullptr);
        if(!m_isSplattingNormalTextureCommited && m_splattingNormalMaterial->IsCommited())
        {
            assert(m_heightmapProcessor != nullptr);
            std::shared_ptr<CTexture> splattingNormalTexture = m_heightmapProcessor->PreprocessSplattingNormalTexture(m_splattingNormalMaterial);
            for(ui32 i = 0; i < m_numChunkRows; ++i)
            {
                for(ui32 j = 0; j < m_numChunkCells; ++j)
                {
                    assert(m_chunks.size() != 0);
                    assert(m_chunks[i + j * m_numChunkRows] != nullptr);
                    m_chunks[i + j * m_numChunkRows]->_Set_SplattingNormalTexture(splattingNormalTexture);
                }
            }
            m_isSplattingNormalTextureCommited = true;
        }
    }
}

void CLandscape::_OnBatch(const std::string& _mode)
{
    
}

i32 CLandscape::_OnQueuePosition(void)
{
    return 0;
}

void CLandscape::_OnBind(const std::string& _mode)
{

}

void CLandscape::_OnDraw(const std::string& _mode)
{

}

void CLandscape::_OnUnbind(const std::string& _mode)
{

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
    assert(m_edges != nullptr);
    m_edges->Set_Camera(_camera);
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
    assert(m_edges != nullptr);
    m_edges->Set_Light(_light, _id);
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
    assert(m_edges != nullptr);
    m_edges->Set_RenderMgr(_renderMgr);
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
    assert(m_edges != nullptr);
    m_edges->Set_SceneUpdateMgr(_sceneUpdateMgr);
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
    assert(m_edges != nullptr);
    m_edges->ListenRenderMgr(_value);
}

void CLandscape::ListenSceneUpdateMgr(bool _value)
{
    IGameObject::ListenSceneUpdateMgr(_value);
    for(ui32 i = 0; i < m_numChunkRows; ++i)
    {
        for(ui32 j = 0; j < m_numChunkCells; ++j)
        {
            assert(m_chunks.size() != 0);
            assert(m_chunks[i + j * m_numChunkRows] != nullptr);
            m_chunks[i + j * m_numChunkRows]->ListenSceneUpdateMgr(_value);
        }
    }
    assert(m_edges != nullptr);
    m_edges->ListenSceneUpdateMgr(_value);
}

ui32 CLandscape::Get_NumTriangles(void)
{
    ui32 numTriangles = 0;
    for(ui32 i = 0; i < m_numChunkRows; ++i)
    {
        for(ui32 j = 0; j < m_numChunkCells; ++j)
        {
            assert(m_chunks.size() != 0);
            assert(m_chunks[i + j * m_numChunkRows] != nullptr);
            numTriangles += m_chunks[i + j * m_numChunkRows]->Get_NumTriangles();
        }
    }
    return numTriangles;
}

