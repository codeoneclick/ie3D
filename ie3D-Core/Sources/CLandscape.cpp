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
#include "CMesh.h"
#include "CLandscapeChunk.h"
#include "CHeightmap.h"
#include "CFrustum.h"
#include "CQuadTree.h"
#include "CVertexBuffer.h"
#include "CConfigurationAccessor.h"

CLandscape::CLandscape(CSharedResourceAccessorRef resourceAccessor,
                       ISharedRenderTechniqueAccessorRef renderTechniqueAccessor) :
IGameObject(resourceAccessor, renderTechniqueAccessor),
m_preprocessSplattingTextureMaterial(nullptr)
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
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED &&
       m_heightmapGenerator && m_heightmapGenerator->isGenerated())
    {
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
                    E_LANDSCAPE_CHUNK_LOD LOD = CLandscape::getLOD(m_camera->getLookAt(), minBound, maxBound);
                    if(m_chunks[index] == nullptr)
                    {
                        m_chunks[index] = std::make_shared<CLandscapeChunk>(m_resourceAccessor, m_renderTechniqueAccessor);
                        m_chunks[index]->setCamera(m_camera);
                        m_chunks[index]->setCameraFrustum(m_cameraFrustum);
                        m_chunks[index]->setInprogressLOD(LOD);
                        m_heightmapGenerator->runChunkLoading(i, j, LOD, m_preprocessSplattingTextureMaterial, [this, index, i, j, LOD](CSharedMeshRef mesh) {
                            
                            m_chunks[index]->onAddedToScene(m_renderTechniqueImporter,
                                                            m_sceneUpdateMgr);
                            m_chunks[index]->setMesh(mesh);
                            m_chunks[index]->onConfigurationLoaded(m_configuration, true);
                            
                        }, [this, index, LOD](CSharedQuadTreeRef quadTree) {
                            m_chunks[index]->setQuadTree(quadTree, LOD);
                        }, [this, index](CSharedTextureRef texture) {
                            m_chunks[index]->setPreprocessedSplattingTexture(texture);
                        });
                    }
                    else if(m_chunks[index]->getInprogressLOD() == m_chunks[index]->getCurrentLOD() &&
                            m_chunks[index]->getCurrentLOD() != LOD)
                    {
                        m_chunks[index]->setInprogressLOD(LOD);
                        m_heightmapGenerator->runChunkLoading(i, j, LOD, nullptr, [this, i, j, index, LOD](CSharedMeshRef mesh) {
                            m_chunks[index]->setQuadTree(nullptr, m_chunks[index]->getCurrentLOD());
                            m_chunks[index]->setMesh(mesh);
                            m_chunks[index]->onSceneUpdate(0);
                        }, [this, index, LOD](CSharedQuadTreeRef quadTree) {
                            m_chunks[index]->setQuadTree(quadTree, LOD);
                            m_chunks[index]->onSceneUpdate(0);
                        }, nullptr);
                    }
                }
                else if(m_chunks[index] != nullptr)
                {
                    m_chunks[index]->onRemovedFromScene();
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
    
    m_heightmapGenerator = std::make_shared<CHeightmapGenerator>(m_renderTechniqueAccessor);
    m_resourceAccessor->addCustomTexture("landscape.splatting.texture", m_heightmapGenerator->createSplattingTexture());
    m_resourceAccessor->addCustomTexture("landscape.heightmap.texture", m_heightmapGenerator->createHeightmapTexture());
    
    if(configurationLandscape->getHeightmapDataFilename().length() != 0)
    {
        m_heightmapGenerator->generate(configurationLandscape->getHeightmapDataFilename(), [this, configurationLandscape, success]() {
            
            if(configurationLandscape->getPreprocessSplattingMaterialFilename().length() != 0)
            {
                CSharedConfigurationMaterial configurationPreprocessSplatting = std::make_shared<CConfigurationMaterial>();
                configurationPreprocessSplatting->serialize(configurationLandscape->getPreprocessSplattingMaterialFilename());
                m_preprocessSplattingTextureMaterial = CMaterial::constructCustomMaterial(configurationPreprocessSplatting,
                                                                                          m_resourceAccessor,
                                                                                          m_renderTechniqueAccessor);
            }
            m_chunks.resize(m_heightmapGenerator->getNumChunks().x * m_heightmapGenerator->getNumChunks().y);
            
            IGameObject::onConfigurationLoaded(configurationLandscape, success);
            
            m_tillingTexcoord[E_SHADER_SAMPLER_01] = 16;
            m_tillingTexcoord[E_SHADER_SAMPLER_02] = 16;
            m_tillingTexcoord[E_SHADER_SAMPLER_03] = 16;
            
            m_status |= E_LOADING_STATUS_TEMPLATE_LOADED;
        });
    }
    else
    {
        m_heightmapGenerator->generate(glm::ivec2(configurationLandscape->getSizeX(), configurationLandscape->getSizeY()),
                                       configurationLandscape->getFrequency(), configurationLandscape->getOctaves(),
                                       configurationLandscape->getSeed(), nullptr);
    }
}

std::vector<ISharedGameObject> CLandscape::getChunks(void) const
{
    std::vector<ISharedGameObject> chunks;
    std::for_each(m_chunks.cbegin(), m_chunks.cend(), [&chunks](CSharedLandscapeChunk chunk) {
        chunks.push_back(chunk);
    });
    return chunks;
}

bool CLandscape::isInCameraFrustum(CSharedFrustumRef cameraFrustum)
{
    return true;
}

void CLandscape::onBind(CSharedMaterialRef material)
{
    material->bind();
    m_materialBindImposer(material);
}

void CLandscape::onUnbind(CSharedMaterialRef material)
{
    material->unbind();
}

void CLandscape::onDraw(CSharedMaterialRef material)
{
    assert(m_camera != nullptr);
    assert(m_globalLightSource != nullptr);
    CLandscape::onBind(material);
    if(material &&
       material->getShader()->isLoaded() &&
       material->getShader()->isCommited())
    {
        std::for_each(m_chunks.cbegin(), m_chunks.cend(), [material, this](CSharedLandscapeChunk chunk) {
            if(chunk && chunk->m_mesh && chunk->m_numPassedIndexes > 0)
            {
                //material->getShader()->setTexture(chunk->getPreprocessedSplattingTexture(), E_SHADER_SAMPLER_01);
                chunk->m_mesh->bind(material->getShader()->getGUID(), material->getShader()->getAttributes());
                chunk->m_mesh->draw(chunk->m_numPassedIndexes);
                chunk->m_mesh->unbind(material->getShader()->getGUID(), material->getShader()->getAttributes());
            }
        });
    }
    CLandscape::onUnbind(material);
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
    return m_heightmapGenerator->isGenerated() ? m_heightmapGenerator->getHeight(position) : 0.0f;
}

glm::vec2 CLandscape::getAngleOnHeightmapSurface(const glm::vec3& position) const
{
    assert(m_heightmapGenerator != nullptr);
    return m_heightmapGenerator->getAngleOnHeightmapSurface(position);
}
