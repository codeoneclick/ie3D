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
#include "CResourceAccessor.h"
#include "CConfigurationGameObjects.h"
#include "CBatchingMgr.h"
#include "CMesh.h"
#include "CLandscapeChunk.h"
#include "CLandscapeEdges.h"
#include "CHeightmapProcessor.h"
#include "CFrustum.h"
#include "CQuadTree.h"

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

bool CLandscape::isPointInBoundPlane(const glm::vec3& point,
                                   const glm::vec3& minBound,
                                   const glm::vec3& maxBound)
{
    if(point.x >= minBound.x &&
       point.x <= maxBound.x &&
       point.z >= minBound.z &&
       point.z <= maxBound.z)
    {
        return true;
    }
    return false;
}

E_LANDSCAPE_CHUNK_LOD CLandscape::getLOD(const glm::vec3& point,
                                         const glm::vec3& minBound,
                                         const glm::vec3& maxBound)
{
    glm::vec2 center = glm::vec2((maxBound.x - minBound.x) / 2.0 + minBound.x,
                                 (maxBound.z - minBound.z) / 2.0 + minBound.z);
    f32 distance = glm::distance(glm::vec2(point.x, point.z), center);
    E_LANDSCAPE_CHUNK_LOD LOD = E_LANDSCAPE_CHUNK_LOD_04;
    if(CLandscape::isPointInBoundPlane(point, minBound, maxBound))
    {
        LOD = E_LANDSCAPE_CHUNK_LOD_01;
    }
    else if(distance < 128.0)
    {
        LOD = E_LANDSCAPE_CHUNK_LOD_02;
    }
    else if(distance < 192.0)
    {
        LOD = E_LANDSCAPE_CHUNK_LOD_03;
    }
    
    return LOD;
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
                    E_LANDSCAPE_CHUNK_LOD LOD = CLandscape::getLOD(m_camera->Get_LookAt(), minBound, maxBound);
                    if(m_chunks[i + j * numChunksZ] == nullptr)
                    {
                        m_chunks[i + j * numChunksZ] = std::make_shared<CLandscapeChunk>(m_resourceAccessor, m_renderTechniqueAccessor);
                        m_chunks[i + j * numChunksZ]->setInprogressLOD(LOD);
                        m_heightmapProcessor->runChunkLoading(i, j, LOD, [this, i , j, numChunksZ, LOD](CSharedMeshRef mesh) {
                            
                            m_chunks[i + j * numChunksZ]->setCamera(m_camera);
                            m_chunks[i + j * numChunksZ]->setGlobalLightSource(m_globalLightSource);
                            
                            m_chunks[i + j * numChunksZ]->setMesh(mesh);
                            m_chunks[i + j * numChunksZ]->onConfigurationLoaded(m_configuration, true);
                            
                            if(m_prerenderedSplattingDiffuseTexture != nullptr)
                            {
                                m_chunks[i + j * numChunksZ]->setPrerenderedSplattingDiffuseTexture(m_prerenderedSplattingDiffuseTexture);
                            }
                            if(m_prerenderedSplattingNormalTexture != nullptr)
                            {
                                m_chunks[i + j * numChunksZ]->setPrerenderedSplattingNormalTexture(m_prerenderedSplattingNormalTexture);
                            }
                            m_chunks[i + j * numChunksZ]->setSplattinMaskTexture(m_heightmapProcessor->Get_SplattingTexture());
                            
                            m_chunks[i + j * numChunksZ]->setRenderTechniqueImporter(m_renderTechniqueImporter);
                            m_chunks[i + j * numChunksZ]->setRenderTechniqueAccessor(m_renderTechniqueAccessor);
                            m_chunks[i + j * numChunksZ]->setSceneUpdateMgr(m_sceneUpdateMgr);
                            
                            m_chunks[i + j * numChunksZ]->enableRender(m_isNeedToRender);
                            m_chunks[i + j * numChunksZ]->enableUpdate(m_isNeedToUpdate);
                            
                            m_chunks[i + j * numChunksZ]->setTillingTexcoord(m_tillingTexcoord[E_SHADER_SAMPLER_01], E_SHADER_SAMPLER_01);
                            m_chunks[i + j * numChunksZ]->setTillingTexcoord(m_tillingTexcoord[E_SHADER_SAMPLER_02], E_SHADER_SAMPLER_02);
                            m_chunks[i + j * numChunksZ]->setTillingTexcoord(m_tillingTexcoord[E_SHADER_SAMPLER_03], E_SHADER_SAMPLER_03);
                            
                        }, [this, i , j, numChunksZ, LOD](CSharedQuadTreeRef quadTree) {
                            m_chunks[i + j * numChunksZ]->setQuadTree(quadTree, LOD);
                        });
                    }
                    else if(m_chunks[i + j * numChunksZ]->getInprogressLOD() != LOD &&
                            m_chunks[i + j * numChunksZ]->getInprogressLOD() != E_LANDSCAPE_CHUNK_LOD_UNKNOWN)
                    {
                        m_heightmapProcessor->stopChunkLoading(i, j);
                        m_chunks[i + j * numChunksZ]->setInprogressLOD(LOD);
                        m_heightmapProcessor->runChunkLoading(i, j, LOD, [this, i , j, numChunksZ, LOD](CSharedMeshRef mesh) {
                            m_chunks[i + j * numChunksZ]->setQuadTree(nullptr, m_chunks[i + j * numChunksZ]->getCurrentLOD());
                            m_chunks[i + j * numChunksZ]->setMesh(mesh);
                            m_chunks[i + j * numChunksZ]->onSceneUpdate(0);
                        }, [this, i , j, numChunksZ, LOD](CSharedQuadTreeRef quadTree) {
                            m_chunks[i + j * numChunksZ]->setQuadTree(quadTree, LOD);
                            m_chunks[i + j * numChunksZ]->onSceneUpdate(0);
                        });
                    }
                }
                else if(m_chunks[i + j * numChunksZ] != nullptr)
                {
                    m_chunks[i + j * numChunksZ]->enableRender(false);
                    m_chunks[i + j * numChunksZ]->enableUpdate(false);
                    m_chunks[i + j * numChunksZ]->removeLoadingDependencies();
                    m_heightmapProcessor->runChunkUnLoading(i, j);
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
    IGameObject::onConfigurationLoaded(configuration, success);
    
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
    
    m_tillingTexcoord[E_SHADER_SAMPLER_01] = 16;
    m_tillingTexcoord[E_SHADER_SAMPLER_02] = 16;
    m_tillingTexcoord[E_SHADER_SAMPLER_03] = 16;
    
    m_status |= E_LOADING_STATUS_TEMPLATE_LOADED;
}

void CLandscape::prerenderSplattingDiffuseTexture(void)
{
    if(m_splattingDiffuseMaterial->isCommited() && !m_isSplattingDiffuseTexturePrerendered)
    {
        ui32 numChunksX = m_heightmapProcessor->getNumChunksX();
        ui32 numChunksZ = m_heightmapProcessor->getNumChunksZ();
        
        m_prerenderedSplattingDiffuseTexture = m_heightmapProcessor->createSplattingDiffuseTexture(m_splattingDiffuseMaterial);
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
        
        m_prerenderedSplattingNormalTexture = m_heightmapProcessor->createSplattingNormalTexture(m_splattingNormalMaterial);
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

void CLandscape::setGlobalLightSource(CSharedGlobalLightSourceRef lightSource)
{
    IGameObject::setGlobalLightSource(lightSource);
    assert(m_edges != nullptr);
    m_edges->setGlobalLightSource(lightSource);
}

void CLandscape::setTexture(CSharedTextureRef texture,
                            E_SHADER_SAMPLER sampler,
                            const std::string& renderTechnique)
{
    IGameObject::setTexture(texture, sampler, renderTechnique);
    std::shared_ptr<CConfigurationLandscape> gameObjectConfiguration = std::static_pointer_cast<CConfigurationLandscape>(m_configuration);
    assert(gameObjectConfiguration != nullptr);
    
    for(const auto& iterator_01 : gameObjectConfiguration->getMaterialsConfigurations())
    {
        CSharedConfigurationMaterial materialConfiguration = std::static_pointer_cast<CConfigurationMaterial>(iterator_01);
        for(const auto& iterator_02 : materialConfiguration->getTexturesConfigurations())
        {
            CSharedConfigurationTexture textureConfiguration = std::static_pointer_cast<CConfigurationTexture>(iterator_02);
            if(textureConfiguration->getSamplerIndex() == sampler)
            {
                std::string filename = texture->getGuid();
                textureConfiguration->setAttribute(getConfigurationAttributeKey(textureConfiguration->kTextureMainNode,
                                                                                textureConfiguration->kTextureFilenameAttribute),
                                                   std::make_shared<CConfigurationAttribute>(filename), true, 0);
            }
        }
    }
    
    for(const auto& iterator : m_chunks)
    {
        if(iterator != nullptr)
        {
            iterator->setTexture(texture, sampler);
        }
    }
    texture->addLoadingHandler(shared_from_this());
}

void CLandscape::setTillingTexcoord(f32 value, E_SHADER_SAMPLER sampler)
{
    m_tillingTexcoord[sampler] = value;
    ui32 numChunksX = m_heightmapProcessor->getNumChunksX();
    ui32 numChunksZ = m_heightmapProcessor->getNumChunksZ();
    
    for(ui32 i = 0; i < numChunksX; ++i)
    {
        for(ui32 j = 0; j < numChunksZ; ++j)
        {
            if(m_chunks[i + j * numChunksZ] != nullptr)
            {
                m_chunks[i + j * numChunksZ]->setTillingTexcoord(value, sampler);
            }
        }
    }
}

f32 CLandscape::getTillingTexcooed(E_SHADER_SAMPLER sampler) const
{
    return m_tillingTexcoord[sampler];
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

glm::vec2 CLandscape::getAngleOnHeightmapSurface(const glm::vec3& position) const
{
    assert(m_heightmapProcessor != nullptr);
    return m_heightmapProcessor->getAngleOnHeightmapSurface(position);
}
