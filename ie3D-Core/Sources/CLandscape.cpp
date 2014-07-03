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
#include "CBatchingMgr.h"
#include "CMesh.h"
#include "CLandscapeChunk.h"
#include "CLandscapeEdges.h"
#include "CHeightmapProcessor.h"
#include "CFrustum.h"

CLandscape::CLandscape(CSharedResourceAccessorRef resourceAccessor,
                       ISharedRenderTechniqueAccessorRef renderTechniqueAccessor) :
IGameObject(resourceAccessor, renderTechniqueAccessor),
m_prerenderedSplattingDiffuseTexture(nullptr),
m_prerenderedSplattingNormalTexture(nullptr),
m_splattingDiffuseMaterial(nullptr),
m_splattingNormalMaterial(nullptr),
m_isSplattingDiffuseTexturePrerendered(false),
m_isSplattingNormalTexturePrerendered(false),
m_configuration(nullptr),
m_edges(std::make_shared<CLandscapeEdges>(resourceAccessor, renderTechniqueAccessor))
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
        
        CLandscape::prerenderSplattingDiffuseTexture();
        CLandscape::prerenderSplattingNormalTexture();
        
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
                        CSharedMesh mesh = m_heightmapProcessor->getChunk(i, j);
                        CSharedLandscapeChunk chunk = std::make_shared<CLandscapeChunk>(m_resourceAccessor, m_renderTechniqueAccessor);
                        
                        chunk->setCamera(m_camera);
                        
                        chunk->setRenderTechniqueImporter(m_renderTechniqueImporter);
                        chunk->setRenderTechniqueAccessor(m_renderTechniqueAccessor);
                        chunk->setSceneUpdateMgr(m_sceneUpdateMgr);
                        
                        chunk->enableRender(m_isNeedToRender);
                        chunk->enableUpdate(m_isNeedToUpdate);
                        
                        ui32 chunkSizeX = m_heightmapProcessor->getChunkSizeX(i, j);
                        ui32 chunkSizeZ = m_heightmapProcessor->getChunkSizeZ(i, j);
                        
                        chunk->setMesh(mesh,
                                       chunkSizeX, chunkSizeZ,
                                       m_heightmapProcessor->getSizeX(),
                                       m_heightmapProcessor->getSizeZ());
                        chunk->setSplattingSettings(16.0);
                        chunk->onConfigurationLoaded(m_configuration, true);
                        
                        if(m_prerenderedSplattingDiffuseTexture != nullptr)
                        {
                            chunk->setPrerenderedSplattingDiffuseTexture(m_prerenderedSplattingDiffuseTexture);
                        }
                        if(m_prerenderedSplattingNormalTexture != nullptr)
                        {
                            chunk->setPrerenderedSplattingNormalTexture(m_prerenderedSplattingNormalTexture);
                        }
                        chunk->setSplattinMaskTexture(m_heightmapProcessor->Get_SplattingTexture());
                        m_chunks[i + j * numChunksZ] = chunk;
                    }
                }
                else if(m_chunks[i + j * numChunksZ] != nullptr)
                {
                    CSharedLandscapeChunk chunk = m_chunks[i + j * numChunksZ];
                    chunk->enableRender(false);
                    chunk->enableUpdate(false);
                    m_heightmapProcessor->freeChunk(chunk->m_mesh, i, j);
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
    assert(m_renderTechniqueAccessor != nullptr);
    
    m_isSplattingDiffuseTexturePrerendered = false;
    m_isSplattingNormalTexturePrerendered = false;
    
    m_heightmapProcessor = std::make_shared<CHeightmapProcessor>(m_renderTechniqueAccessor, landscapeConfiguration);
    IEditableLandscape::setHeightmapProcessor(m_heightmapProcessor);
    
    m_resourceAccessor->addCustomTexture("landscape.splatting.texture" , m_heightmapProcessor->createSplattingTexture());
    m_resourceAccessor->addCustomTexture("landscape.heightmap.texture", m_heightmapProcessor->createHeightmapTexture());
    m_resourceAccessor->addCustomTexture("landscape.edgesmask.texture", m_heightmapProcessor->createEdgesMaskTexture());
    
    CSharedConfigurationMaterial materialConfiguration = std::static_pointer_cast<CConfigurationMaterial>(landscapeConfiguration->getSplattingDiffuseMaterialConfiguration());
    m_splattingDiffuseMaterial = std::make_shared<CMaterial>();
    CMaterial::setupMaterial(m_splattingDiffuseMaterial, materialConfiguration, m_resourceAccessor, m_renderTechniqueAccessor, shared_from_this());
    m_splattingDiffuseMaterial->setTexture(m_heightmapProcessor->Get_SplattingTexture(), E_SHADER_SAMPLER_04);
    
    materialConfiguration = std::static_pointer_cast<CConfigurationMaterial>(landscapeConfiguration->getSplattingNormalMaterialConfiguration());
    m_splattingNormalMaterial = std::make_shared<CMaterial>();
    CMaterial::setupMaterial(m_splattingNormalMaterial, materialConfiguration, m_resourceAccessor, m_renderTechniqueAccessor, shared_from_this());
    m_splattingNormalMaterial->setTexture(m_heightmapProcessor->Get_SplattingTexture(), E_SHADER_SAMPLER_04);
    
    m_chunks.resize(m_heightmapProcessor->getNumChunksX() * m_heightmapProcessor->getNumChunksZ());
    
    m_edges->onConfigurationLoaded(configuration, success);
    m_edges->setEdgeTexture(m_heightmapProcessor->Get_EdgesMaskTexture());
    
    m_status |= E_LOADING_STATUS_TEMPLATE_LOADED;
}

void CLandscape::prerenderSplattingDiffuseTexture(void)
{
    if(m_splattingDiffuseMaterial->isCommited() && !m_isSplattingDiffuseTexturePrerendered)
    {
        ui32 numChunksX = m_heightmapProcessor->getNumChunksX();
        ui32 numChunksZ = m_heightmapProcessor->getNumChunksZ();
        
        m_prerenderedSplattingDiffuseTexture = m_heightmapProcessor->PreprocessSplattingDiffuseTexture(m_splattingDiffuseMaterial);
        m_isSplattingDiffuseTexturePrerendered = true;
        for(ui32 i = 0; i < numChunksX; ++i)
        {
            for(ui32 j = 0; j < numChunksZ; ++j)
            {
                if(m_chunks[i + j * numChunksZ] != nullptr)
                {
                    m_chunks[i + j * numChunksZ]->setPrerenderedSplattingDiffuseTexture(m_prerenderedSplattingDiffuseTexture);
                }
            }
        }
    }
}

void CLandscape::prerenderSplattingNormalTexture(void)
{
    if(m_splattingNormalMaterial->isCommited() && !m_isSplattingNormalTexturePrerendered)
    {
        ui32 numChunksX = m_heightmapProcessor->getNumChunksX();
        ui32 numChunksZ = m_heightmapProcessor->getNumChunksZ();
        
        m_prerenderedSplattingNormalTexture = m_heightmapProcessor->PreprocessSplattingNormalTexture(m_splattingNormalMaterial);
        m_isSplattingNormalTexturePrerendered = true;
        for(ui32 i = 0; i < numChunksX; ++i)
        {
            for(ui32 j = 0; j < numChunksZ; ++j)
            {
                if(m_chunks[i + j * numChunksZ] != nullptr)
                {
                    m_chunks[i + j * numChunksZ]->setPrerenderedSplattingNormalTexture(m_prerenderedSplattingNormalTexture);
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

void CLandscape::setRenderTechniqueImporter(ISharedRenderTechniqueImporterRef techniqueImporter)
{
    assert(m_edges != nullptr);
    IGameObject::setRenderTechniqueImporter(techniqueImporter);
    m_edges->setRenderTechniqueImporter(techniqueImporter);
}

void CLandscape::setRenderTechniqueAccessor(ISharedRenderTechniqueAccessorRef techniqueAccessor)
{
    assert(m_edges != nullptr);
    IGameObject::setRenderTechniqueAccessor(techniqueAccessor);
    m_edges->setRenderTechniqueAccessor(techniqueAccessor);
}

void CLandscape::setSceneUpdateMgr(CSharedSceneUpdateMgrRef sceneUpdateMgr)
{
    assert(m_edges != nullptr);
    IGameObject::setSceneUpdateMgr(sceneUpdateMgr);
    m_edges->setSceneUpdateMgr(sceneUpdateMgr);
}

void CLandscape::enableRender(bool value)
{
    assert(m_edges != nullptr);
    m_isNeedToRender = value;
    m_edges->enableRender(value);
}

void CLandscape::enableUpdate(bool value)
{
    assert(m_edges != nullptr);
    IGameObject::enableUpdate(value);
    m_edges->enableUpdate(value);
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

f32 CLandscape::getHeight(const glm::vec3& position) const
{
    assert(m_heightmapProcessor != nullptr);
    return m_heightmapProcessor->getHeight(position);
}

