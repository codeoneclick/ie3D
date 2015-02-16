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
#include "CHeightmap.h"
#include "CFrustum.h"
#include "CQuadTree.h"
#include "CVertexBuffer.h"
#include "CConfigurationAccessor.h"

CLandscape::CLandscape(CSharedResourceAccessorRef resourceAccessor,
                       ISharedRenderTechniqueAccessorRef renderTechniqueAccessor) :
IGameObject(resourceAccessor, renderTechniqueAccessor)
{
    m_isNeedBoundingBox = false;
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
        m_heightmapGenerator->update();
        
        glm::ivec2 numChunks = m_heightmapGenerator->getNumChunks();
        
        for(i32 i = 0; i < numChunks.x; ++i)
        {
            for(i32 j = 0; j < numChunks.y; ++j)
            {
                i32 index = i + j * numChunks.x;
                glm::vec3 maxBound = std::get<0>(m_heightmapGenerator->getChunkBounds(i, j));
                glm::vec3 minBound = std::get<1>(m_heightmapGenerator->getChunkBounds(i, j));
                
                i32 result = m_cameraFrustum->isBoundBoxInFrustum(maxBound, minBound);
                if(result == E_FRUSTUM_BOUND_RESULT_INSIDE ||
                   result == E_FRUSTUM_BOUND_RESULT_INTERSECT)
                {
                    E_LANDSCAPE_CHUNK_LOD LOD = CLandscape::getLOD(m_camera->Get_LookAt(), minBound, maxBound);
                    if(m_chunks[index] == nullptr)
                    {
                        m_chunks[index] = std::make_shared<CLandscapeChunk>(m_resourceAccessor, m_renderTechniqueAccessor);
                        m_chunks[index]->setInprogressLOD(LOD);
                        m_heightmapGenerator->runChunkLoading(i, j, LOD, [this, index, i, j, LOD](CSharedMeshRef mesh) {
                            
                            //m_chunks[index]->setCamera(m_camera);
                            m_chunks[index]->setCameraFrustum(m_cameraFrustum);
                            //m_chunks[index]->setGlobalLightSource(m_globalLightSource);
                            
                            m_chunks[index]->setMesh(mesh);
                            //m_chunks[index]->onConfigurationLoaded(m_configuration, true);
                            
                            //m_chunks[index]->setRenderTechniqueImporter(m_renderTechniqueImporter);
                            //m_chunks[index]->setRenderTechniqueAccessor(m_renderTechniqueAccessor);
                            m_chunks[index]->setSceneUpdateMgr(m_sceneUpdateMgr);
                            
                            //m_chunks[index]->enableRender(m_isNeedToRender);
                            m_chunks[index]->enableUpdate(m_isNeedToUpdate);
                            
                        }, [this, index, LOD](CSharedQuadTreeRef quadTree) {
                            m_chunks[index]->setQuadTree(quadTree, LOD);
                        }, [this, i, j](void) {
                            CLandscape::resetSeams(i, j);
                        });
                    }
                    else if(m_chunks[index]->getInprogressLOD() == m_chunks[index]->getCurrentLOD() &&
                            m_chunks[index]->getCurrentLOD() != LOD)
                    {
                        m_chunks[index]->setInprogressLOD(LOD);
                        m_heightmapGenerator->runChunkLoading(i, j, LOD, [this, i, j, index, LOD](CSharedMeshRef mesh) {
                            m_chunks[index]->setQuadTree(nullptr, m_chunks[index]->getCurrentLOD());
                            m_chunks[index]->setMesh(mesh);
                            m_chunks[index]->onSceneUpdate(0);
                        }, [this, index, LOD](CSharedQuadTreeRef quadTree) {
                            m_chunks[index]->setQuadTree(quadTree, LOD);
                            m_chunks[index]->onSceneUpdate(0);
                        }, [this , i, j](void) {
                            CLandscape::resetSeams(i, j);
                        });
                    }
                    CLandscape::sewSeams(i, j);
                }
                else if(m_chunks[index] != nullptr)
                {
                    m_chunks[index]->enableRender(false);
                    m_chunks[index]->enableUpdate(false);
                    m_chunks[index]->removeLoadingDependencies();
                    m_heightmapGenerator->runChunkUnLoading(i, j);
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
    std::shared_ptr<CConfigurationLandscape> configurationLandscape = std::static_pointer_cast<CConfigurationLandscape>(configuration);
    assert(m_resourceAccessor != nullptr);
    assert(m_renderTechniqueAccessor != nullptr);
    
    m_heightmapGenerator = std::make_shared<CHeightmapGenerator>(m_renderTechniqueAccessor, configurationLandscape);
    m_chunks.resize(m_heightmapGenerator->getNumChunks().x * m_heightmapGenerator->getNumChunks().y);
    
    m_resourceAccessor->addCustomTexture("landscape.splatting.texture", m_heightmapGenerator->createSplattingTexture());
    m_resourceAccessor->addCustomTexture("landscape.heightmap.texture", m_heightmapGenerator->createHeightmapTexture());
    
    IGameObject::onConfigurationLoaded(configuration, success);
    
    m_tillingTexcoord[E_SHADER_SAMPLER_01] = 16;
    m_tillingTexcoord[E_SHADER_SAMPLER_02] = 16;
    m_tillingTexcoord[E_SHADER_SAMPLER_03] = 16;
    
    m_status |= E_LOADING_STATUS_TEMPLATE_LOADED;
}

std::vector<ISharedGameObject> CLandscape::getChunks(void) const
{
    std::vector<ISharedGameObject> chunks;
    std::for_each(m_chunks.cbegin(), m_chunks.cend(), [&chunks](CSharedLandscapeChunk chunk) {
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
    return false;
}

ui32 CLandscape::numTriangles(void)
{
    return std::accumulate(m_chunks.cbegin(), m_chunks.cend(), 0, [](ui32 count, CSharedLandscapeChunk chunk) {
        if(chunk)
        {
            count += chunk->m_numPassedIndexes / 3;
        }
        return count;
    });
}

void CLandscape::onBind(const std::string& techniqueName)
{
    assert(m_camera != nullptr);
    assert(m_globalLightSource != nullptr);
    assert(m_materials.find(techniqueName) != m_materials.end());
    
    const auto& material = m_materials.find(techniqueName)->second;
    if(material->getShader()->isLoaded())
    {
        material->bind();
        m_materialBindImposer(material);
    }
}

void CLandscape::onDraw(const std::string& techniqueName)
{
    assert(m_camera != nullptr);
    assert(m_globalLightSource != nullptr);
    assert(m_materials.find(techniqueName) != m_materials.end());
    
    const auto& material = m_materials.find(techniqueName)->second;
    if(material->getShader()->isLoaded())
    {
        std::for_each(m_chunks.cbegin(), m_chunks.cend(), [material, this](CSharedLandscapeChunk chunk) {
            if(chunk && chunk->m_mesh)
            {
                chunk->m_mesh->bind(material->getShader()->getAttributes());
                chunk->m_mesh->draw(chunk->m_numPassedIndexes);
                chunk->m_mesh->unbind(material->getShader()->getAttributes());
            }
        });
    }
}

void CLandscape::onUnbind(const std::string& techniqueName)
{
    assert(m_camera != nullptr);
    assert(m_globalLightSource != nullptr);
    assert(m_materials.find(techniqueName) != m_materials.end());
    
    const auto& material = m_materials.find(techniqueName)->second;
    if(material->getShader()->isLoaded())
    {
        material->unbind();
    }
}

void CLandscape::onBatch(const std::string& mode)
{
    
}

void CLandscape::bindCustomShaderUniforms(CSharedMaterialRef material)
{
    IGameObject::bindCustomShaderUniforms(material);
#if defined(__OSX__)
    material->getShader()->setFloatCustom(m_tillingTexcoord[E_SHADER_SAMPLER_01], "IN_TillingTexcoordLayer_01");
    material->getShader()->setFloatCustom(m_tillingTexcoord[E_SHADER_SAMPLER_02], "IN_TillingTexcoordLayer_02");
    material->getShader()->setFloatCustom(m_tillingTexcoord[E_SHADER_SAMPLER_03], "IN_TillingTexcoordLayer_03");
#elif defined(__IOS__)
    if(/*g_highPerformancePlatforms.count(getPlatform()) != 0*/ true)
    {
        material->getShader()->setFloatCustom(m_tillingTexcoord[E_SHADER_SAMPLER_01], "IN_TillingTexcoordLayer_01");
        material->getShader()->setFloatCustom(m_tillingTexcoord[E_SHADER_SAMPLER_02], "IN_TillingTexcoordLayer_02");
        material->getShader()->setFloatCustom(m_tillingTexcoord[E_SHADER_SAMPLER_03], "IN_TillingTexcoordLayer_03");
    }
#else
    material->getShader()->setFloatCustom(MAX_VALUE(m_heightmapSize.x, m_heightmapSize.y) / m_splattingTillingFactor,
                                          "IN_SplattingTillingFactor");
#endif
    material->getShader()->setFloatCustom(256.0, "IN_fogLinearStart");
    material->getShader()->setFloatCustom(512.0, "IN_fogLinearEnd");
}

void CLandscape::setTexture(CSharedTextureRef texture,
                            E_SHADER_SAMPLER samplerIndex,
                            const std::string& renderTechnique)
{
    IGameObject::setTexture(texture, samplerIndex, renderTechnique);
    
    std::shared_ptr<CConfigurationLandscape> configurationGameObject = std::static_pointer_cast<CConfigurationLandscape>(m_configuration);
    assert(configurationGameObject != nullptr);
    for(const auto& iterator_01 : configurationGameObject->getMaterialsConfigurations())
    {
        CSharedConfigurationMaterial configurationMaterial = std::static_pointer_cast<CConfigurationMaterial>(iterator_01);
        for(const auto& iterator_02 : configurationMaterial->getTexturesConfigurations())
        {
            CSharedConfigurationTexture configurationTexture = std::static_pointer_cast<CConfigurationTexture>(iterator_02);
            if(configurationTexture->getSamplerIndex() == samplerIndex)
            {
                std::string filename = texture->getGuid();
                configurationTexture->setAttribute("/texture/filename", std::make_shared<CConfigurationAttribute>(filename));
            }
        }
    }
    
    for(const auto& iterator : m_chunks)
    {
        if(iterator != nullptr)
        {
            iterator->setTexture(texture, samplerIndex);
        }
    }
    texture->addLoadingHandler(shared_from_this());
}

void CLandscape::setTillingTexcoord(f32 value, E_SHADER_SAMPLER sampler)
{
    m_tillingTexcoord[sampler] = value;
}

f32 CLandscape::getTillingTexcoord(E_SHADER_SAMPLER sampler) const
{
    return m_tillingTexcoord[sampler];
}

glm::ivec2 CLandscape::getHeightmapSize(void) const
{
    assert(m_heightmapGenerator != nullptr);
    return m_heightmapGenerator->getSize();
}

f32 CLandscape::getHeight(const glm::vec3& position) const
{
    assert(m_heightmapGenerator != nullptr);
    return m_heightmapGenerator->getHeight(position);
}

glm::vec2 CLandscape::getAngleOnHeightmapSurface(const glm::vec3& position) const
{
    assert(m_heightmapGenerator != nullptr);
    return m_heightmapGenerator->getAngleOnHeightmapSurface(position);
}

void CLandscape::sewSeams(i32 currentIndexX, i32 currentIndexZ)
{
    i32 index = currentIndexX + currentIndexZ * m_heightmapGenerator->getNumChunks().x;
    if((currentIndexZ - 1) >= 0)
    {
        CLandscape::sewSeams(m_chunks[index], currentIndexX + (currentIndexZ - 1) * m_heightmapGenerator->getNumChunks().x, E_LANDSCAPE_SEAM_Z_MINUS, E_LANDSCAPE_SEAM_Z_PLUS);
    }
    if((currentIndexZ + 1) < m_heightmapGenerator->getNumChunks().y)
    {
        CLandscape::sewSeams(m_chunks[index], currentIndexX + (currentIndexZ + 1) * m_heightmapGenerator->getNumChunks().x, E_LANDSCAPE_SEAM_Z_PLUS, E_LANDSCAPE_SEAM_Z_MINUS);
    }
    if((currentIndexX - 1) >= 0)
    {
        CLandscape::sewSeams(m_chunks[index], (currentIndexX - 1) + currentIndexZ * m_heightmapGenerator->getNumChunks().x, E_LANDSCAPE_SEAM_X_MINUS, E_LANDSCAPE_SEAM_X_PLUS);
    }
    if((currentIndexX + 1) < m_heightmapGenerator->getNumChunks().x)
    {
        CLandscape::sewSeams(m_chunks[index], (currentIndexX + 1) + currentIndexZ * m_heightmapGenerator->getNumChunks().x, E_LANDSCAPE_SEAM_X_PLUS, E_LANDSCAPE_SEAM_X_MINUS);
    }
}

void CLandscape::sewSeams(CSharedLandscapeChunkRef currentChunk, i32 neighborChunkIndex,
                          E_LANDSCAPE_SEAM currentChunkSeamType, E_LANDSCAPE_SEAM neighborChunkSeamType)
{
    if(currentChunk != nullptr &&
       currentChunk->getCurrentLOD() != E_LANDSCAPE_CHUNK_LOD_UNKNOWN)
    {
        CSharedLandscapeChunk neighborChunk = m_chunks[neighborChunkIndex];
        if(neighborChunk != nullptr &&
           neighborChunk->getCurrentLOD() != E_LANDSCAPE_CHUNK_LOD_UNKNOWN)
        {
            std::vector<SAttributeVertex> currentChunkVerteces = currentChunk->getSeamVerteces(currentChunkSeamType);
            std::vector<SAttributeVertex> neighborChunkVerteces = neighborChunk->getSeamVerteces(neighborChunkSeamType);
            
            if(currentChunkVerteces.size() > neighborChunkVerteces.size() && currentChunk->getCurrentLOD() != currentChunk->getSeamedLOD(currentChunkSeamType))
            {
                currentChunk->setSeamVerteces(neighborChunkVerteces, currentChunkSeamType);
            }
            else if(neighborChunkVerteces.size() >= currentChunkVerteces.size() && neighborChunk->getCurrentLOD() != neighborChunk->getSeamedLOD(neighborChunkSeamType))
            {
                neighborChunk->setSeamVerteces(currentChunkVerteces, neighborChunkSeamType);
            }
        }
    }
}

void CLandscape::resetSeams(i32 currentIndexX, i32 currentIndexZ)
{
    i32 index = currentIndexX + currentIndexZ * m_heightmapGenerator->getNumChunks().x;
    CSharedLandscapeChunk currentChunk = m_chunks[index];
    if(currentChunk != nullptr)
    {
        currentChunk->resetSeams();
    }
    
    if((currentIndexZ - 1) >= 0)
    {
        ui32 neighborChunkIndex = currentIndexX + (currentIndexZ - 1) * m_heightmapGenerator->getNumChunks().x;
        CSharedLandscapeChunk neighborChunk = m_chunks[neighborChunkIndex];
        if(neighborChunk != nullptr)
        {
            neighborChunk->resetSeams();
        }
    }
    if((currentIndexZ + 1) < m_heightmapGenerator->getNumChunks().y)
    {
        ui32 neighborChunkIndex = currentIndexX + (currentIndexZ + 1) * m_heightmapGenerator->getNumChunks().x;
        CSharedLandscapeChunk neighborChunk = m_chunks[neighborChunkIndex];
        if(neighborChunk != nullptr)
        {
            neighborChunk->resetSeams();
        }
    }
    if((currentIndexX - 1) >= 0)
    {
        ui32 neighborChunkIndex = (currentIndexX - 1) + currentIndexZ * m_heightmapGenerator->getNumChunks().x;
        CSharedLandscapeChunk neighborChunk = m_chunks[neighborChunkIndex];
        if(neighborChunk != nullptr)
        {
            neighborChunk->resetSeams();
        }
    }
    if((currentIndexX + 1) < m_heightmapGenerator->getNumChunks().x)
    {
        ui32 neighborChunkIndex = (currentIndexX + 1) + currentIndexZ * m_heightmapGenerator->getNumChunks().x;
        CSharedLandscapeChunk neighborChunk = m_chunks[neighborChunkIndex];
        if(neighborChunk != nullptr)
        {
            neighborChunk->resetSeams();
        }
    }
}
