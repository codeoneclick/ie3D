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
#include "CFrustum.h"

CLandscape::CLandscape(CSharedResourceAccessorRef resourceAccessor,
                       ISharedScreenSpaceTextureAccessorRef screenSpaceTextureAccessor) :
IGameObject(resourceAccessor, screenSpaceTextureAccessor),
m_splattingDiffuseTexture(nullptr),
m_splattingNormalTexture(nullptr),
m_splattingDiffuseMaterial(nullptr),
m_splattingNormalMaterial(nullptr),
m_isSplattingDiffuseTextureProcessed(false),
m_isSplattingNormalTextureProcessed(false),
m_configuration(nullptr),
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
        m_heightmapProcessor->update();
        
        CLandscape::processSplattingDiffuseTexture();
        CLandscape::processSplattingNormalTexture();
        
        ui32 chunkSizeX = m_heightmapProcessor->getChunkSizeX();
        ui32 chunkSizeZ = m_heightmapProcessor->getChunkSizeZ();
        
        ui32 numChunksX = m_heightmapProcessor->getNumChunksX();
        ui32 numChunksZ = m_heightmapProcessor->getNumChunksZ();
        
        for(ui32 i = 0; i < numChunksX; ++i)
        {
            for(ui32 j = 0; j < numChunksZ; ++j)
            {
                glm::vec3 maxBound = std::get<0>(m_heightmapProcessor->getChunkBounds(i, j));
                glm::vec3 minBound = std::get<1>(m_heightmapProcessor->getChunkBounds(i, j));
                
                i32 result = m_camera->Get_Frustum()->IsBoundBoxInFrustum(maxBound,
                                                                          minBound);
                if(result == E_FRUSTUM_BOUND_RESULT_INSIDE ||
                   result == E_FRUSTUM_BOUND_RESULT_INTERSECT)
                {
                    if(m_chunks[i + j * numChunksZ] == nullptr)
                    {
                        std::shared_ptr<CMesh> mesh = m_heightmapProcessor->getChunk(i, j);
                        m_chunks[i + j * numChunksZ] = std::make_shared<CLandscapeChunk>(m_resourceAccessor, m_screenSpaceTextureAccessor);
                        
                        m_chunks[i + j * numChunksZ]->setCamera(m_camera);
                        
                        m_chunks[i + j * numChunksZ]->setRenderMgr(m_renderMgr);
                        m_chunks[i + j * numChunksZ]->setSceneUpdateMgr(m_sceneUpdateMgr);
                        
                        m_chunks[i + j * numChunksZ]->listenRenderMgr(m_isNeedToRender);
                        m_chunks[i + j * numChunksZ]->listenSceneUpdateMgr(m_isNeedToUpdate);
                        
                        m_chunks[i + j * numChunksZ]->setMesh(mesh, chunkSizeX, chunkSizeZ);
                        m_chunks[i + j * numChunksZ]->onConfigurationLoaded(m_configuration, true);
                        
                        if(m_splattingDiffuseTexture != nullptr)
                        {
                            m_chunks[i + j * numChunksZ]->setSplattingDiffuseTexture(m_splattingDiffuseTexture);
                        }
                        if(m_splattingNormalTexture != nullptr)
                        {
                            m_chunks[i + j * numChunksZ]->setSplattingNormalTexture(m_splattingNormalTexture);
                        }
                    }
                }
                else if(m_chunks[i + j * numChunksZ] != nullptr)
                {
                    m_chunks[i + j * numChunksZ]->listenRenderMgr(false);
                    m_chunks[i + j * numChunksZ]->listenSceneUpdateMgr(false);
                    m_heightmapProcessor->freeChunk(m_chunks[i + j * numChunksZ]->m_mesh, i, j);
                    m_chunks[i + j * numChunksZ] = nullptr;
                }
            }
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
    m_configuration = configuration;
    assert(m_resourceAccessor != nullptr);
    assert(m_screenSpaceTextureAccessor != nullptr);
    
    m_isSplattingDiffuseTextureProcessed = false;
    m_isSplattingNormalTextureProcessed = false;
    
    m_heightmapProcessor = std::make_shared<CHeightmapProcessor>(m_screenSpaceTextureAccessor, landscapeConfiguration);
    
    m_screenSpaceTextureAccessor->addCustomTexture(m_heightmapProcessor->PreprocessSplattingTexture());
    m_screenSpaceTextureAccessor->addCustomTexture(m_heightmapProcessor->PreprocessHeightmapTexture());
    m_screenSpaceTextureAccessor->addCustomTexture(m_heightmapProcessor->PreprocessEdgesMaskTexture());
    
    CSharedConfigurationMaterial materialConfiguration = std::static_pointer_cast<CConfigurationMaterial>(landscapeConfiguration->getSplattingDiffuseMaterialConfiguration());
    m_splattingDiffuseMaterial = std::make_shared<CMaterial>();
    CMaterial::setupMaterial(m_splattingDiffuseMaterial, materialConfiguration, m_resourceAccessor, m_screenSpaceTextureAccessor, shared_from_this());
    m_splattingDiffuseMaterial->setTexture(m_heightmapProcessor->Get_SplattingTexture(), E_SHADER_SAMPLER_04);
    
    materialConfiguration = std::static_pointer_cast<CConfigurationMaterial>(landscapeConfiguration->getSplattingNormalMaterialConfiguration());
    m_splattingNormalMaterial = std::make_shared<CMaterial>();
    CMaterial::setupMaterial(m_splattingNormalMaterial, materialConfiguration, m_resourceAccessor, m_screenSpaceTextureAccessor, shared_from_this());
    m_splattingNormalMaterial->setTexture(m_heightmapProcessor->Get_SplattingTexture(), E_SHADER_SAMPLER_04);
    
    m_chunks.resize(m_heightmapProcessor->getNumChunksX() * m_heightmapProcessor->getNumChunksZ());
    
    m_edges->onConfigurationLoaded(configuration, success);
    m_edges->setEdgeTexture(m_heightmapProcessor->Get_EdgesMaskTexture());
    
    m_status |= E_LOADING_STATUS_TEMPLATE_LOADED;
}

void CLandscape::processSplattingDiffuseTexture(void)
{
    if(m_splattingDiffuseMaterial->isCommited() && !m_isSplattingDiffuseTextureProcessed)
    {
        ui32 numChunksX = m_heightmapProcessor->getNumChunksX();
        ui32 numChunksZ = m_heightmapProcessor->getNumChunksZ();
        
        m_splattingDiffuseTexture = m_heightmapProcessor->PreprocessSplattingDiffuseTexture(m_splattingDiffuseMaterial);
        m_isSplattingDiffuseTextureProcessed = true;
        for(ui32 i = 0; i < numChunksX; ++i)
        {
            for(ui32 j = 0; j < numChunksZ; ++j)
            {
                if(m_chunks[i + j * numChunksZ] != nullptr)
                {
                    m_chunks[i + j * numChunksZ]->setSplattingDiffuseTexture(m_splattingDiffuseTexture);
                }
            }
        }
    }
}

void CLandscape::processSplattingNormalTexture(void)
{
    if(m_splattingNormalMaterial->isCommited() && !m_isSplattingNormalTextureProcessed)
    {
        ui32 numChunksX = m_heightmapProcessor->getNumChunksX();
        ui32 numChunksZ = m_heightmapProcessor->getNumChunksZ();
        
        m_splattingNormalTexture = m_heightmapProcessor->PreprocessSplattingNormalTexture(m_splattingNormalMaterial);
        m_isSplattingNormalTextureProcessed = true;
        for(ui32 i = 0; i < numChunksX; ++i)
        {
            for(ui32 j = 0; j < numChunksZ; ++j)
            {
                if(m_chunks[i + j * numChunksZ] != nullptr)
                {
                    m_chunks[i + j * numChunksZ]->setSplattingNormalTexture(m_splattingNormalTexture);
                }
            }
        }
    }
}

std::vector<ISharedGameObject> CLandscape::getChunks(void) const
{
    std::vector<ISharedGameObject> chunks;
    std::for_each(m_chunks.begin(), m_chunks.end(), [&chunks](CSharedLandscapeChunk chunk){
        chunks.push_back(chunk);
    });
    return chunks;
}

i32 CLandscape::zOrder(void)
{
    return m_zOrder;
}

bool CLandscape::checkOcclusion(void)
{
    return IGameObject::checkOcclusion();
}

ui32 CLandscape::numTriangles(void)
{
    ui32 numChunksX = m_heightmapProcessor->getNumChunksX();
    ui32 numChunksZ = m_heightmapProcessor->getNumChunksZ();
    
    ui32 numTriangles = 0;
    for(ui32 i = 0; i < numChunksX; ++i)
    {
        for(ui32 j = 0; j < numChunksZ; ++j)
        {
            assert(m_chunks.size() != 0);
            assert(m_chunks[i + j * numChunksZ] != nullptr);
            numTriangles += m_chunks[i + j * numChunksZ]->numTriangles();
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
    assert(m_edges != nullptr);
    m_edges->setCamera(camera);
}

void CLandscape::setLightSource(CSharedLightSourceRef lightSource,
                                E_LIGHT_SOURCE index)
{
    IGameObject::setLightSource(lightSource, index);
    assert(m_edges != nullptr);
    m_edges->setLightSource(lightSource, index);
}

void CLandscape::setRenderMgr(CSharedRenderMgrRef renderMgr)
{
    IGameObject::setRenderMgr(renderMgr);
    assert(m_edges != nullptr);
    m_edges->setRenderMgr(renderMgr);
}

void CLandscape::setSceneUpdateMgr(CSharedSceneUpdateMgrRef sceneUpdateMgr)
{
    IGameObject::setSceneUpdateMgr(sceneUpdateMgr);
    assert(m_edges != nullptr);
    m_edges->setSceneUpdateMgr(sceneUpdateMgr);
}

void CLandscape::listenRenderMgr(bool value)
{
    m_isNeedToRender = value;
    assert(m_edges != nullptr);
    m_edges->listenRenderMgr(value);
}

void CLandscape::listenSceneUpdateMgr(bool value)
{
    IGameObject::listenSceneUpdateMgr(value);
    assert(m_edges != nullptr);
    m_edges->listenSceneUpdateMgr(value);
}

CSharedTexture CLandscape::getHeightmapTexture(void) const
{
    assert(m_heightmapProcessor != nullptr);
    assert(m_heightmapProcessor->Get_HeightmapTexture() != nullptr);
    return m_heightmapProcessor->Get_HeightmapTexture();
}

ui32 CLandscape::getHeightmapSizeX(void) const
{
    assert(m_heightmapProcessor != nullptr);
    assert(m_heightmapProcessor->getSizeX() != 0);
    return m_heightmapProcessor->getSizeX();
}

ui32 CLandscape::getHeightmapSizeZ(void) const
{
    assert(m_heightmapProcessor != nullptr);
    assert(m_heightmapProcessor->getSizeZ() != 0);
    return m_heightmapProcessor->getSizeZ();
}

