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
#include "CConfigurationGameObjects.h"
#include "CRenderMgr.h"
#include "CBatchingMgr.h"
#include "CMesh.h"
#include "CLandscapeChunk.h"
#include "CLandscapeEdges.h"
#include "CHeightmapProcessor.h"

CLandscape::CLandscape(CSharedResourceAccessorRef resourceAccessor,
                       ISharedScreenSpaceTextureAccessorRef screenSpaceTextureAccessor) :
IGameObject(resourceAccessor, screenSpaceTextureAccessor),
m_splattingDiffuseMaterial(nullptr),
m_splattingNormalMaterial(nullptr),
m_isSplattingDiffuseTextureCommited(false),
m_isSplattingNormalTextureCommited(false),
m_edges(std::make_shared<CLandscapeEdges>(resourceAccessor, screenSpaceTextureAccessor))
{

}

CLandscape::~CLandscape(void)
{

}

void CLandscape::onSceneUpdate(f32 deltatime)
{
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        assert(m_splattingDiffuseMaterial != nullptr);
        if(!m_isSplattingDiffuseTextureCommited && m_splattingDiffuseMaterial->isCommited())
        {
            assert(m_heightmapProcessor != nullptr);
            std::shared_ptr<CTexture> splattingDiffuseTexture = m_heightmapProcessor->PreprocessSplattingDiffuseTexture(m_splattingDiffuseMaterial);
            for(ui32 i = 0; i < m_numChunkRows; ++i)
            {
                for(ui32 j = 0; j < m_numChunkCells; ++j)
                {
                    assert(m_chunks.size() != 0);
                    assert(m_chunks[i + j * m_numChunkRows] != nullptr);
                    m_chunks[i + j * m_numChunkRows]->setSplattingDiffuseTexture(splattingDiffuseTexture);
                }
            }
            m_isSplattingDiffuseTextureCommited = true;
        }
        
        assert(m_splattingNormalMaterial != nullptr);
        if(!m_isSplattingNormalTextureCommited && m_splattingNormalMaterial->isCommited())
        {
            assert(m_heightmapProcessor != nullptr);
            std::shared_ptr<CTexture> splattingNormalTexture = m_heightmapProcessor->PreprocessSplattingNormalTexture(m_splattingNormalMaterial);
            for(ui32 i = 0; i < m_numChunkRows; ++i)
            {
                for(ui32 j = 0; j < m_numChunkCells; ++j)
                {
                    assert(m_chunks.size() != 0);
                    assert(m_chunks[i + j * m_numChunkRows] != nullptr);
                    m_chunks[i + j * m_numChunkRows]->setSplattingNormalTexture(splattingNormalTexture);
                }
            }
            m_isSplattingNormalTextureCommited = true;
        }
    }
}

void CLandscape::onResourceLoaded(ISharedResourceRef resource, bool success)
{
    IGameObject::onResourceLoaded(resource, success);
}

void CLandscape::onConfigurationLoaded(ISharedConfigurationRef configuration, bool success)
{
    std::shared_ptr<CConfigurationLandscape> landscapeConfiguration = std::static_pointer_cast<CConfigurationLandscape>(configuration);
    assert(m_resourceAccessor != nullptr);
    assert(m_screenSpaceTextureAccessor != nullptr);
    
    m_isSplattingDiffuseTextureCommited = false;
    m_isSplattingNormalTextureCommited = false;
    
    m_heightmapProcessor = std::make_shared<CHeightmapProcessor>(m_screenSpaceTextureAccessor, landscapeConfiguration);
    
    m_heightmapProcessor->PreprocessSplattingTexture();
    m_heightmapProcessor->PreprocessHeightmapTexture();
    m_heightmapProcessor->PreprocessEdgesMaskTexture();
    
    CSharedConfigurationMaterial materialConfiguration = std::static_pointer_cast<CConfigurationMaterial>(landscapeConfiguration->getSplattingDiffuseMaterialConfiguration());
    m_splattingDiffuseMaterial = std::make_shared<CMaterial>(materialConfiguration->getRenderOperationName());
    IGameObject::setupMaterial(m_splattingDiffuseMaterial, materialConfiguration);
    m_splattingDiffuseMaterial->setTexture(m_heightmapProcessor->Get_SplattingTexture(), E_SHADER_SAMPLER_04);
    
    materialConfiguration = std::static_pointer_cast<CConfigurationMaterial>(landscapeConfiguration->getSplattingNormalMaterialConfiguration());
    m_splattingNormalMaterial = std::make_shared<CMaterial>(materialConfiguration->getRenderOperationName());
    IGameObject::setupMaterial(m_splattingNormalMaterial, materialConfiguration);
    m_splattingNormalMaterial->setTexture(m_heightmapProcessor->Get_SplattingTexture(), E_SHADER_SAMPLER_04);
    
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
            m_chunks[i + j * m_numChunkRows]->setMesh(mesh, chunkWidth, chunkHeight);
            m_chunks[i + j * m_numChunkRows]->onConfigurationLoaded(configuration, success);
        }
    }
    m_edges->onConfigurationLoaded(configuration, success);
    m_edges->setEdgeTexture(m_heightmapProcessor->Get_EdgesMaskTexture());
    
    CLandscape::setCamera(m_camera);
    CLandscape::setRenderMgr(m_renderMgr);
    CLandscape::setSceneUpdateMgr(m_sceneUpdateMgr);
	CLandscape::listenRenderMgr(m_isNeedToRender);
    CLandscape::listenSceneUpdateMgr(m_isNeedToUpdate);
    
    m_status |= E_LOADING_STATUS_TEMPLATE_LOADED;
}

i32  CLandscape::getZOrder(void)
{
    return m_renderQueuePosition;
}

bool CLandscape::checkOcclusion(void)
{
    return IGameObject::checkOcclusion();
}

ui32 CLandscape::numTriangles(void)
{
    ui32 numTriangles = 0;
    for(ui32 i = 0; i < m_numChunkRows; ++i)
    {
        for(ui32 j = 0; j < m_numChunkCells; ++j)
        {
            assert(m_chunks.size() != 0);
            assert(m_chunks[i + j * m_numChunkRows] != nullptr);
            numTriangles += m_chunks[i + j * m_numChunkRows]->numTriangles();
        }
    }
    return numTriangles;
}

void CLandscape::onBind(const std::string& mode)
{
    
}

void CLandscape::onDraw(const std::string& mode)
{
    
}

void CLandscape::onUnbind(const std::string& mode)
{
    
}

void CLandscape::onBatch(const std::string& mode)
{
    
}

void CLandscape::setCamera(CSharedCameraRef camera)
{
    IGameObject::setCamera(camera);
    
    for(ui32 i = 0; i < m_numChunkRows; ++i)
    {
        for(ui32 j = 0; j < m_numChunkCells; ++j)
        {
            assert(m_chunks.size() != 0);
            assert(m_chunks[i + j * m_numChunkRows] != nullptr);
            m_chunks[i + j * m_numChunkRows]->setCamera(camera);
        }
    }
    assert(m_edges != nullptr);
    m_edges->setCamera(camera);
}

void CLandscape::setLightSource(CSharedLightSourceRef lightSource,
                                E_LIGHT_SOURCE index)
{
    IGameObject::setLightSource(lightSource, index);
    
    for(ui32 i = 0; i < m_numChunkRows; ++i)
    {
        for(ui32 j = 0; j < m_numChunkCells; ++j)
        {
            assert(m_chunks.size() != 0);
            assert(m_chunks[i + j * m_numChunkRows] != nullptr);
            m_chunks[i + j * m_numChunkRows]->setLightSource(lightSource, index);
        }
    }
    assert(m_edges != nullptr);
    m_edges->setLightSource(lightSource, index);
}

void CLandscape::setRenderMgr(CSharedRenderMgrRef renderMgr)
{
    IGameObject::setRenderMgr(renderMgr);
    
    for(ui32 i = 0; i < m_numChunkRows; ++i)
    {
        for(ui32 j = 0; j < m_numChunkCells; ++j)
        {
            assert(m_chunks.size() != 0);
            assert(m_chunks[i + j * m_numChunkRows] != nullptr);
            m_chunks[i + j * m_numChunkRows]->setRenderMgr(renderMgr);
        }
    }
    assert(m_edges != nullptr);
    m_edges->setRenderMgr(renderMgr);
}

void CLandscape::setSceneUpdateMgr(CSharedSceneUpdateMgrRef sceneUpdateMgr)
{
    IGameObject::setSceneUpdateMgr(sceneUpdateMgr);
    
    for(ui32 i = 0; i < m_numChunkRows; ++i)
    {
        for(ui32 j = 0; j < m_numChunkCells; ++j)
        {
            assert(m_chunks.size() != 0);
            assert(m_chunks[i + j * m_numChunkRows] != nullptr);
            m_chunks[i + j * m_numChunkRows]->setSceneUpdateMgr(sceneUpdateMgr);
        }
    }
    assert(m_edges != nullptr);
    m_edges->setSceneUpdateMgr(sceneUpdateMgr);
}

void CLandscape::listenRenderMgr(bool value)
{
    m_isNeedToRender = value;
    for(ui32 i = 0; i < m_numChunkRows; ++i)
    {
        for(ui32 j = 0; j < m_numChunkCells; ++j)
        {
            assert(m_chunks.size() != 0);
            assert(m_chunks[i + j * m_numChunkRows] != nullptr);
            m_chunks[i + j * m_numChunkRows]->listenRenderMgr(value);
        }
    }
    assert(m_edges != nullptr);
    m_edges->listenRenderMgr(value);
}

void CLandscape::listenSceneUpdateMgr(bool value)
{
    IGameObject::listenSceneUpdateMgr(value);
    for(ui32 i = 0; i < m_numChunkRows; ++i)
    {
        for(ui32 j = 0; j < m_numChunkCells; ++j)
        {
            assert(m_chunks.size() != 0);
            assert(m_chunks[i + j * m_numChunkRows] != nullptr);
            m_chunks[i + j * m_numChunkRows]->listenSceneUpdateMgr(value);
        }
    }
    assert(m_edges != nullptr);
    m_edges->listenSceneUpdateMgr(value);
}

CSharedTexture CLandscape::getHeightmapTexture(void) const
{
    assert(m_heightmapProcessor != nullptr);
    assert(m_heightmapProcessor->Get_HeightmapTexture() != nullptr);
    return m_heightmapProcessor->Get_HeightmapTexture();
}

f32* CLandscape::getHeightmapData(void) const
{
    assert(m_heightmapProcessor != nullptr);
    assert(m_heightmapProcessor->Get_HeightmapData() != nullptr);
    return m_heightmapProcessor->Get_HeightmapData();
}

ui32 CLandscape::getHeightmapWidth(void) const
{
    assert(m_heightmapProcessor != nullptr);
    assert(m_heightmapProcessor->Get_Width() != 0);
    return m_heightmapProcessor->Get_Width();
}

ui32 CLandscape::getHeightmapHeight(void) const
{
    assert(m_heightmapProcessor != nullptr);
    assert(m_heightmapProcessor->Get_Height() != 0);
    return m_heightmapProcessor->Get_Height();
}

