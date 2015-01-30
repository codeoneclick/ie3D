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
#include "CBatchingMgr.h"
#include "CMesh.h"
#include "CLandscapeChunk.h"
#include "CHeightmapProcessor.h"
#include "CFrustum.h"
#include "CQuadTree.h"
#include "CVertexBuffer.h"
#include "CConfigurationAccessor.h"

CLandscape::CLandscape(CSharedResourceAccessorRef resourceAccessor,
                       ISharedRenderTechniqueAccessorRef renderTechniqueAccessor) :
IGameObject(resourceAccessor, renderTechniqueAccessor)
{
    m_isNeedBoundingBox = false;
    m_materialBindImposer = nullptr;
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
        
        ui32 numChunksX = m_heightmapProcessor->getNumChunksX();
        ui32 numChunksZ = m_heightmapProcessor->getNumChunksZ();
        
        for(i32 i = 0; i < numChunksX; ++i)
        {
            for(i32 j = 0; j < numChunksZ; ++j)
            {
                i32 index = i + j * numChunksZ;
                glm::vec3 maxBound = std::get<0>(m_heightmapProcessor->getChunkBounds(i, j));
                glm::vec3 minBound = std::get<1>(m_heightmapProcessor->getChunkBounds(i, j));
                
                i32 result = m_cameraFrustum->isBoundBoxInFrustum(maxBound, minBound);
                if(result == E_FRUSTUM_BOUND_RESULT_INSIDE ||
                   result == E_FRUSTUM_BOUND_RESULT_INTERSECT)
                {
                    E_LANDSCAPE_CHUNK_LOD LOD = CLandscape::getLOD(m_camera->Get_LookAt(), minBound, maxBound);
                    if(m_chunks[index] == nullptr)
                    {
                        m_chunks[index] = std::make_shared<CLandscapeChunk>(m_resourceAccessor, m_renderTechniqueAccessor);
                        m_chunks[index]->setInprogressLOD(LOD);
                        m_heightmapProcessor->runChunkLoading(i, j, LOD, [this, index, i, j, numChunksX, numChunksZ, LOD](CSharedMeshRef mesh) {
                            
                            m_chunks[index]->setCamera(m_camera);
                            m_chunks[index]->setCameraFrustum(m_cameraFrustum);
                            m_chunks[index]->setGlobalLightSource(m_globalLightSource);
                            
                            m_chunks[index]->setMesh(mesh);
                            m_chunks[index]->onConfigurationLoaded(m_configuration, true);
                            
                            m_chunks[index]->setRenderTechniqueImporter(m_renderTechniqueImporter);
                            m_chunks[index]->setRenderTechniqueAccessor(m_renderTechniqueAccessor);
                            m_chunks[index]->setSceneUpdateMgr(m_sceneUpdateMgr);
                            
                            m_chunks[index]->enableRender(m_isNeedToRender);
                            m_chunks[index]->enableUpdate(m_isNeedToUpdate);
                            
                            m_chunks[index]->setTillingTexcoord(m_tillingTexcoord[E_SHADER_SAMPLER_01], E_SHADER_SAMPLER_01);
                            m_chunks[index]->setTillingTexcoord(m_tillingTexcoord[E_SHADER_SAMPLER_02], E_SHADER_SAMPLER_02);
                            m_chunks[index]->setTillingTexcoord(m_tillingTexcoord[E_SHADER_SAMPLER_03], E_SHADER_SAMPLER_03);
                            //CLandscape::sewSeams(i, j);

                        }, [this, index, LOD](CSharedQuadTreeRef quadTree) {
                            m_chunks[index]->setQuadTree(quadTree, LOD);
                        });
                    }
                    else if(m_chunks[index]->getInprogressLOD() != LOD &&
                            m_chunks[index]->getCurrentLOD() != E_LANDSCAPE_CHUNK_LOD_UNKNOWN)
                    {
                        m_chunks[index]->setInprogressLOD(LOD);
                        m_heightmapProcessor->stopChunkLoading(i, j, [this, i, j, index, numChunksZ, LOD](void){
                            m_heightmapProcessor->runChunkLoading(i, j, LOD, [this, i, j, index, numChunksZ, LOD](CSharedMeshRef mesh) {
                                m_chunks[index]->setQuadTree(nullptr, m_chunks[index]->getCurrentLOD());
                                m_chunks[index]->setMesh(mesh);
                                m_chunks[index]->onSceneUpdate(0);
                                //CLandscape::sewSeams(i, j);

                            }, [this, index, LOD](CSharedQuadTreeRef quadTree) {
                                m_chunks[index]->setQuadTree(quadTree, LOD);
                                m_chunks[index]->onSceneUpdate(0);
                            });
                        });
                    }
                }
                else if(m_chunks[index] != nullptr)
                {
                    m_chunks[index]->enableRender(false);
                    m_chunks[index]->enableUpdate(false);
                    m_chunks[index]->removeLoadingDependencies();
                    m_heightmapProcessor->runChunkUnLoading(i, j);
                    m_chunks[index] = nullptr;
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
    
    m_heightmapProcessor = std::make_shared<CHeightmapProcessor>(m_renderTechniqueAccessor, landscapeConfiguration);
    IEditableLandscape::setHeightmapProcessor(m_heightmapProcessor);
    
    m_chunks.resize(m_heightmapProcessor->getNumChunksX() * m_heightmapProcessor->getNumChunksZ());
    
    m_tillingTexcoord[E_SHADER_SAMPLER_01] = 16;
    m_tillingTexcoord[E_SHADER_SAMPLER_02] = 16;
    m_tillingTexcoord[E_SHADER_SAMPLER_03] = 16;
    
    m_status |= E_LOADING_STATUS_TEMPLATE_LOADED;
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
    return 0;
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

void CLandscape::setTexture(CSharedTextureRef texture,
                            E_SHADER_SAMPLER sampler,
                            const std::string& renderTechnique)
{
    IGameObject::setTexture(texture, sampler, renderTechnique);
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

f32 CLandscape::getTillingTexcoord(E_SHADER_SAMPLER sampler) const
{
    return m_tillingTexcoord[sampler];
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

void CLandscape::sewSeams(i32 currentIndexX, i32 currentIndexZ)
{
    ui32 chunksCountPerRow = m_heightmapProcessor->getNumChunksX();
    i32 index = currentIndexX + currentIndexZ * chunksCountPerRow;
    if((currentIndexZ - 1) >= 0)
    {
        CLandscape::sewSeams(m_chunks[index], currentIndexX + (currentIndexZ - 1) * chunksCountPerRow, E_LANDSCAPE_SEAM_Z_MINUS, E_LANDSCAPE_SEAM_Z_PLUS);
    }
    if((currentIndexZ + 1) < chunksCountPerRow)
    {
        CLandscape::sewSeams(m_chunks[index], currentIndexX + (currentIndexZ + 1) * chunksCountPerRow, E_LANDSCAPE_SEAM_Z_PLUS, E_LANDSCAPE_SEAM_Z_MINUS);
    }
    if((currentIndexX - 1) >= 0)
    {
        CLandscape::sewSeams(m_chunks[index], (currentIndexX - 1) + currentIndexZ * chunksCountPerRow, E_LANDSCAPE_SEAM_X_MINUS, E_LANDSCAPE_SEAM_X_PLUS);
    }
    if((currentIndexX + 1) < chunksCountPerRow)
    {
        CLandscape::sewSeams(m_chunks[index], (currentIndexX + 1) + currentIndexZ * chunksCountPerRow, E_LANDSCAPE_SEAM_X_PLUS, E_LANDSCAPE_SEAM_X_MINUS);
    }
}

void CLandscape::sewSeams(CSharedLandscapeChunkRef currentChunk, i32 neighborChunkIndex,
                          E_LANDSCAPE_SEAM currentChunkSeamType, E_LANDSCAPE_SEAM neighborChunkSeamType)
{
    if(neighborChunkIndex >= 0 &&
       neighborChunkIndex < m_chunks.size())
    {
        CSharedLandscapeChunk neighborChunk = m_chunks[neighborChunkIndex];
        if(neighborChunk != nullptr &&
           neighborChunk->getCurrentLOD() == neighborChunk->getInprogressLOD())
        {
            std::vector<SAttributeVertex> currentChunkVerteces = currentChunk->getSeamVerteces(currentChunkSeamType);
            std::vector<SAttributeVertex> neighborChunkVerteces = neighborChunk->getSeamVerteces(neighborChunkSeamType);
            
            if(currentChunkVerteces.size() >= neighborChunkVerteces.size())
            {
                currentChunk->setSeamVerteces(neighborChunkVerteces, currentChunkSeamType);
            }
            else if(currentChunkVerteces.size() < neighborChunkVerteces.size())
            {
                neighborChunk->setSeamVerteces(currentChunkVerteces, neighborChunkSeamType);
            }
        }
    }
}
