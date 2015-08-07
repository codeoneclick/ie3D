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
#include "CHeightmapAccessor.h"

CLandscape::CLandscape(CSharedResourceAccessorRef resourceAccessor,
                       ISharedRenderTechniqueAccessorRef renderTechniqueAccessor) :
IGameObject(resourceAccessor, renderTechniqueAccessor),
m_preprocessSplattingTextureMaterial(nullptr)
{
    m_heightmapAccessor = std::make_shared<CHeightmapAccessor>();
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
    if(distance < 64.0)
    {
        LOD = E_LANDSCAPE_CHUNK_LOD_01;
    }
    else if(distance < 192.0)
    {
        LOD = E_LANDSCAPE_CHUNK_LOD_02;
    }
    else if(distance < 256.0)
    {
        LOD = E_LANDSCAPE_CHUNK_LOD_03;
    }
    
    return LOD;
}

void CLandscape::onSceneUpdate(f32 deltatime)
{
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        glm::ivec2 numChunks = m_heightmapAccessor->getChunksNum();
        
        for(i32 i = 0; i < numChunks.x; ++i)
        {
            for(i32 j = 0; j < numChunks.y; ++j)
            {
                i32 index = i + j * numChunks.x;
                glm::vec3 minBound = std::get<0>(m_heightmapAccessor->getChunkBounds(i, j));
                glm::vec3 maxBound = std::get<1>(m_heightmapAccessor->getChunkBounds(i, j));
                
                i32 result = m_cameraFrustum->isBoundBoxInFrustum(maxBound, minBound);
                if((result == E_FRUSTUM_BOUND_RESULT_INSIDE ||
                    result == E_FRUSTUM_BOUND_RESULT_INTERSECT))
                {
                    E_LANDSCAPE_CHUNK_LOD LOD = CLandscape::getLOD(m_camera->getLookAt(), minBound, maxBound);
                    if(!m_chunks[index])
                    {
                        m_chunks[index] = std::make_shared<CLandscapeChunk>(m_resourceAccessor, m_renderTechniqueAccessor);
                        m_chunks[index]->setCamera(m_camera);
                        m_chunks[index]->setCameraFrustum(m_cameraFrustum);
                        m_chunks[index]->setInprogressLOD(LOD);
                        
                        m_chunks[index]->setPosition(IGameObject::getPosition());
                        m_chunks[index]->setRotation(IGameObject::getRotation());
                        m_chunks[index]->setScale(IGameObject::getScale());
                        
                        m_heightmapAccessor->runLoading(i, j, LOD, [this, index, LOD](CSharedMeshRef mesh) {
                            
                            m_chunks[index]->onAddedToScene(m_renderTechniqueImporter,
                                                            m_sceneUpdateMgr);
                            m_chunks[index]->setMesh(mesh);
                            m_chunks[index]->onConfigurationLoaded(m_configuration, true);
                            
                        }, [this, index, LOD](CSharedQuadTreeRef quadTree) {
                            m_chunks[index]->setQuadTree(quadTree, LOD);
                        }, [this, index](CSharedTextureRef DTexture, CSharedTextureRef NTexture) {
                            m_chunks[index]->setPreprocessedSplattingDTexture(DTexture);
                            m_chunks[index]->setPreprocessedSplattingNTexture(NTexture);
                        });
                    }
                    else if(m_chunks[index]->getInprogressLOD() == m_chunks[index]->getCurrentLOD() &&
                            m_chunks[index]->getCurrentLOD() != LOD)
                    {
                        m_chunks[index]->setInprogressLOD(LOD);
                        m_heightmapAccessor->runLoading(i, j, LOD, [this, index, LOD](CSharedMeshRef mesh) {
                            m_chunks[index]->setQuadTree(nullptr, m_chunks[index]->getCurrentLOD());
                            m_chunks[index]->setMesh(mesh);
                            m_chunks[index]->onSceneUpdate(0);
                        }, [this, index, LOD](CSharedQuadTreeRef quadTree) {
                            m_chunks[index]->setQuadTree(quadTree, LOD);
                            m_chunks[index]->onSceneUpdate(0);
                        }, [this, index](CSharedTextureRef DTexture, CSharedTextureRef NTexture) {
                            m_chunks[index]->setPreprocessedSplattingDTexture(DTexture);
                            m_chunks[index]->setPreprocessedSplattingNTexture(NTexture);
                        });
                    }
                }
                else if(m_chunks[index])
                {
                    m_chunks[index]->onRemovedFromScene();
                    m_heightmapAccessor->runUnLoading(i, j);
                    m_chunks[index] = nullptr;
                }
            }
        }
        m_heightmapAccessor->update();
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

    CSharedConfigurationMaterial configurationSplatting = std::make_shared<CConfigurationMaterial>();
    configurationSplatting->serialize(configurationLandscape->getPreprocessSplattingMaterialFilename());
    
    std::array<CSharedTexture, 3> splattingDTextures;
    std::array<CSharedTexture, 3> splattingNTextures;
    assert(configurationSplatting->getTexturesConfigurations().size() == 6);
    ui32 index = 0;
    for(const auto& iterator : configurationSplatting->getTexturesConfigurations())
    {
        CSharedConfigurationTexture textureConfiguration = std::static_pointer_cast<CConfigurationTexture>(iterator);
        assert(textureConfiguration != nullptr);
        assert(!textureConfiguration->getCubemap());
        assert(textureConfiguration->getTextureFilename().length() != 0);
        CSharedTexture texture = m_resourceAccessor->getTexture(textureConfiguration->getTextureFilename(), true);
        if(index >= 3)
        {
            splattingNTextures[index - 3] = texture;
        }
        else
        {
            splattingDTextures[index] = texture;
        }
        index++;
    }
    assert(configurationLandscape->getHeightmapDataFilename().length() != 0);
    
    std::shared_ptr<CHeightmapAccessor::SHeightmapCustomParameters> customParameters;
    if(configurationLandscape->getPerlin())
    {
        customParameters = std::make_shared<CHeightmapAccessor::SHeightmapCustomParameters>();
        customParameters->m_size = glm::ivec2(configurationLandscape->getSizeX(), configurationLandscape->getSizeY());
        customParameters->m_frequency = configurationLandscape->getFrequency();
        customParameters->m_octaves = configurationLandscape->getOctaves();
        customParameters->m_seed = configurationLandscape->getSeed();
    }
    
    m_heightmapAccessor->generate(configurationLandscape->getHeightmapDataFilename(), m_renderTechniqueAccessor, splattingDTextures, splattingNTextures, [this, configurationLandscape, success]() {
        
        m_chunks.resize(m_heightmapAccessor->getChunksNum().x * m_heightmapAccessor->getChunksNum().y);
        
        IGameObject::onConfigurationLoaded(configurationLandscape, success);
        m_resourceAccessor->addCustomTexture("deep.texture", m_heightmapAccessor->getDeepTexture());
        
        m_status |= E_LOADING_STATUS_TEMPLATE_LOADED;
    }, customParameters);
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
                if(chunk->getPreprocessedSplattingDTexture() && chunk->getPreprocessedSplattingNTexture())
                {
                    material->getShader()->setTexture(chunk->getPreprocessedSplattingDTexture(), E_SHADER_SAMPLER_01);
                    material->getShader()->setTexture(chunk->getPreprocessedSplattingNTexture(), E_SHADER_SAMPLER_02);
                    //material->getShader()->setTexture(chunk->getPreprocessedSplattingHTexture(), E_SHADER_SAMPLER_03);
                }
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
    return m_status & E_LOADING_STATUS_TEMPLATE_LOADED ? m_heightmapAccessor->getMainSize() : glm::ivec2(0);
}

f32 CLandscape::getHeight(const glm::vec3& position) const
{
    return m_status & E_LOADING_STATUS_TEMPLATE_LOADED ? m_heightmapAccessor->getHeight(position) : 0;
}

glm::vec2 CLandscape::getAngles(const glm::vec3& position) const
{
    return m_status & E_LOADING_STATUS_TEMPLATE_LOADED ? m_heightmapAccessor->getAngles(position) : glm::vec2(0.0f);
}

void CLandscape::setGeneratorStatisticCallback(CHeightmapGeneratorStatistic::T_STATISTIC_CALLBACK callback)
{
    assert(m_heightmapAccessor);
    m_heightmapAccessor->getGeneratorStatistic()->setCallback(callback);
}
