//
//  CHeightmapAccessor.cpp
//  ie3D-Core
//
//  Created by sergey.sergeev on 5/11/15.
//  Copyright (c) 2015 Sergey Sergeev. All rights reserved.
//

#include "CHeightmapAccessor.h"
#include "CHeightmapContainer.h"
#include "CHeightmapLoader.h"
#include "CHeightmapGeometryGenerator.h"
#include "CHeightmapTextureGenerator.h"
#include "CThreadOperation.h"
#include "CMesh.h"
#include "CQuadTree.h"
#include "CTexture.h"
#include "CBoundingBox.h"
#include "CHeightmapGeneratorStatistic.h"
#include "HEnums.h"
#include "CPerlinNoise.h"

CHeightmapAccessor::CHeightmapAccessor(void) :
m_container(std::make_shared<CHeightmapContainer>()),
m_isGenerated(false),
m_renderTechniqueAccessor(nullptr),
m_generatorStatistic(std::make_shared<CHeightmapGeneratorStatistic>())
{
    for(ui32 i = 0; i < E_SPLATTING_TEXTURE_MAX; ++i)
    {
        m_splattingDTextures[i] = nullptr;
        m_splattingNTextures[i] = nullptr;
    }
}

CHeightmapAccessor::~CHeightmapAccessor(void)
{
    for(ui32 i = 0; i < E_SPLATTING_TEXTURE_MAX; ++i)
    {
        m_splattingDTextures[i] = nullptr;
        m_splattingNTextures[i] = nullptr;
    }
}

void CHeightmapAccessor::createLoadingOperations(void)
{
    m_callbacks.clear();
    m_callbacks.resize(m_container->getChunksNum().x * m_container->getChunksNum().y,
                       std::make_tuple(nullptr, nullptr, nullptr));
    
    m_executedOperations.clear();
    m_executedOperations.resize(m_container->getChunksNum().x * m_container->getChunksNum().y, nullptr);
}

void CHeightmapAccessor::eraseLoadingOperations(void)
{
    m_callbacks.clear();
    m_executedOperations.clear();
}

void CHeightmapAccessor::createBoundingBoxes(void)
{
    m_chunksBounds.clear();
    m_chunksBounds.resize(m_container->getChunksNum().x * m_container->getChunksNum().y,
                          std::make_tuple(glm::vec3(INT16_MAX),
                                          glm::vec3(INT16_MIN)));
    
    for(ui32 i = 0; i < m_container->getChunksNum().x; ++i)
    {
        for(ui32 j = 0; j < m_container->getChunksNum().y; ++j)
        {
            CHeightmapAccessor::createBoundingBox(i, j);
        }
    }
}

void CHeightmapAccessor::createBoundingBox(ui32 i, ui32 j)
{
    ui32 index = i + j * m_container->getChunksNum().x;
    glm::vec3 minBound = glm::vec3(INT16_MAX);
    glm::vec3 maxBound = glm::vec3(INT16_MIN);
    
    for(ui32 x = 0; x < m_container->getChunkSize().x; ++x)
    {
        for(ui32 y = 0; y < m_container->getChunkSize().y; ++y)
        {
            glm::ivec2 position = glm::ivec2(x + i * m_container->getChunkSize().x - i,
                                             y + j * m_container->getChunkSize().y - j);
            
            position.x = position.x < m_container->getMainSize().x ? position.x : m_container->getMainSize().x - 1;
            position.y = position.y < m_container->getMainSize().y ? position.y : m_container->getMainSize().y - 1;
            
            glm::vec3 point = m_container->getVertexPosition(position.x, position.y);
            minBound = glm::min(point, minBound);
            maxBound = glm::max(point, maxBound);
        }
    }
    m_chunksBounds[index] = std::make_tuple(minBound, maxBound);
}

void CHeightmapAccessor::eraseBoundingBoxes(void)
{
    m_chunksBounds.clear();
}

void CHeightmapAccessor::createMetadataContainers(void)
{
    m_chunksMetadata.clear();
    m_chunksMetadata.resize(m_container->getChunksNum().x * m_container->getChunksNum().y,
                            std::make_tuple(nullptr, nullptr, nullptr, nullptr, E_LANDSCAPE_CHUNK_LOD_UNKNOWN, std::make_shared<std::mutex>()));
}

void CHeightmapAccessor::eraseMetadataContainers(void)
{
    m_chunksMetadata.clear();
}

void CHeightmapAccessor::eraseChunkMetadata(i32 index)
{
    assert(std::get<5>(m_chunksMetadata[index]));
    std::lock_guard<std::mutex> guard(*std::get<5>(m_chunksMetadata[index]).get());
    
    std::get<0>(m_chunksMetadata[index]) = nullptr;
    std::get<1>(m_chunksMetadata[index]) = nullptr;
    std::get<2>(m_chunksMetadata[index]) = nullptr;
    std::get<3>(m_chunksMetadata[index]) = nullptr;
    std::get<4>(m_chunksMetadata[index]) = E_LANDSCAPE_CHUNK_LOD_UNKNOWN;
    
    std::get<0>(m_callbacks[index]) = nullptr;
    std::get<1>(m_callbacks[index]) = nullptr;
    std::get<2>(m_callbacks[index]) = nullptr;
    
    m_executedOperations[index] = nullptr;
}

void CHeightmapAccessor::generate(const std::string& filename, ISharedRenderTechniqueAccessorRef renderTechniqueAccessor,
                                  const std::array<CSharedTexture, 3>& splattingDTextures,
                                  const std::array<CSharedTexture, 3>& splattingNTextures,
                                  const std::function<void(void)>& callback,
                                  const std::shared_ptr<SHeightmapCustomParameters>& customParameters)
{
    m_isGenerated = false;
    m_generatorStatistic->update("Heightmap Generation...", E_HEIGHTMAP_GENERATION_STATUS_STARTED);
    
    for(ui32 i = 0; i < E_SPLATTING_TEXTURE_MAX; ++i)
    {
        m_splattingDTextures[i] = splattingDTextures[i];
        m_splattingNTextures[i] = splattingNTextures[i];
    }
    m_renderTechniqueAccessor = renderTechniqueAccessor;
    
    CSharedThreadOperation completionOperation = std::make_shared<CThreadOperation>(E_THREAD_OPERATION_QUEUE_MAIN);
    completionOperation->setExecutionBlock([this, callback](void) {
        m_isGenerated = true;
        callback();
        m_generatorStatistic->update("Heightmap Generation...", E_HEIGHTMAP_GENERATION_STATUS_ENDED);
    });
    
    CSharedThreadOperation generateGeometryOperation = std::make_shared<CThreadOperation>(E_THREAD_OPERATION_QUEUE_BACKGROUND);
    generateGeometryOperation->setExecutionBlock([this, filename, customParameters](void) {
        
        std::tuple<glm::ivec2, std::vector<f32>> metadata;
        if(customParameters)
        {
            const CPerlinNoise perlin(customParameters->m_seed);
            const f32 fx = customParameters->m_size.x / customParameters->m_frequency;
            const f32 fy = customParameters->m_size.y / customParameters->m_frequency;
            
            std::vector<f32> heights;
            for(ui32 i = 0; i < customParameters->m_size.x; ++i)
            {
                for(ui32 j = 0; j < customParameters->m_size.y; ++j)
                {
                    f32 n = perlin.octaveNoise(i / fx, j / fy, customParameters->m_octaves);
                    n = glm::clamp(n * 0.5f + 0.5f, 0.0f, 1.0f);
                    heights.push_back(n * 64.0f - 32.0f);
                }
            }
            metadata = std::make_tuple(customParameters->m_size, heights);
        }
        else
        {
            metadata = CHeightmapLoader::getHeights(filename);
        }

        m_container->init(std::get<0>(metadata));
        
        m_generatorStatistic->update("Geometry Generation...", E_HEIGHTMAP_GENERATION_STATUS_STARTED);
        CHeightmapGeometryGenerator::generate(m_container, filename, std::get<0>(metadata), std::get<1>(metadata));
        m_generatorStatistic->update("Geometry Generation...", E_HEIGHTMAP_GENERATION_STATUS_ENDED);
        CHeightmapAccessor::createLoadingOperations();
        CHeightmapAccessor::createMetadataContainers();
    });
    completionOperation->addDependency(generateGeometryOperation);
    
    CSharedThreadOperation mmapGeometryOperation = std::make_shared<CThreadOperation>(E_THREAD_OPERATION_QUEUE_BACKGROUND);
    mmapGeometryOperation->setExecutionBlock([this, filename](void) {
       
        m_generatorStatistic->update("MMAP Geometry...", E_HEIGHTMAP_GENERATION_STATUS_STARTED);
        m_container->mmapGeometry(filename);
        m_generatorStatistic->update("MMAP Geometry...", E_HEIGHTMAP_GENERATION_STATUS_ENDED);
        
        /*m_generatorStatistic->update("Smooth Textcoord Generation...", E_HEIGHTMAP_GENERATION_STATUS_STARTED);
        CHeightmapGeometryGenerator::generateSmoothTexcoord(m_container, filename);
        m_generatorStatistic->update("Smooth Textcoord Generation...", E_HEIGHTMAP_GENERATION_STATUS_ENDED);*/
        
        m_generatorStatistic->update("Tangent Space Generation...", E_HEIGHTMAP_GENERATION_STATUS_STARTED);
        CHeightmapGeometryGenerator::generateTangentSpace(m_container, filename);
        m_generatorStatistic->update("Tangent Space Generation...", E_HEIGHTMAP_GENERATION_STATUS_ENDED);
        
        m_generatorStatistic->update("VBO Attaches Generation...", E_HEIGHTMAP_GENERATION_STATUS_STARTED);
        CHeightmapGeometryGenerator::generateAttachesToVBO(m_container, filename);
        m_generatorStatistic->update("VBO Attaches Generation...", E_HEIGHTMAP_GENERATION_STATUS_ENDED);
        
        m_generatorStatistic->update("Bounding Boxes Creation...", E_HEIGHTMAP_GENERATION_STATUS_STARTED);
        CHeightmapAccessor::createBoundingBoxes();
        m_generatorStatistic->update("Bounding Boxes Creation...", E_HEIGHTMAP_GENERATION_STATUS_ENDED);
        
    });
    completionOperation->addDependency(mmapGeometryOperation);
    
    CSharedThreadOperation generateHeightmapTextureOperation = std::make_shared<CThreadOperation>(E_THREAD_OPERATION_QUEUE_MAIN);
    generateHeightmapTextureOperation->setExecutionBlock([this, filename](void) {
        
        m_generatorStatistic->update("Deep Texture Generation...", E_HEIGHTMAP_GENERATION_STATUS_STARTED);
        m_container->createDeepTexture();
        CHeightmapTextureGenerator::generateDeepTexture(m_container, true, 0, 0, 0, 0);
        m_generatorStatistic->update("Deep Texture Generation...", E_HEIGHTMAP_GENERATION_STATUS_ENDED);
    });
    completionOperation->addDependency(generateHeightmapTextureOperation);
    
    CSharedThreadOperation generateSplattingMasksOperation = std::make_shared<CThreadOperation>(E_THREAD_OPERATION_QUEUE_BACKGROUND);
    generateSplattingMasksOperation->setExecutionBlock([this, filename](void) {
        
        m_generatorStatistic->update("Splatting Masks Generation...", E_HEIGHTMAP_GENERATION_STATUS_STARTED);
        CHeightmapTextureGenerator::generateSplattingMTextures(m_container, filename);
        m_generatorStatistic->update("Splatting Masks Generation...", E_HEIGHTMAP_GENERATION_STATUS_ENDED);
    });
    completionOperation->addDependency(generateSplattingMasksOperation);
    
    CSharedThreadOperation mmapMasksOperation = std::make_shared<CThreadOperation>(E_THREAD_OPERATION_QUEUE_BACKGROUND);
    mmapMasksOperation->setExecutionBlock([this, filename](void) {
        
        m_generatorStatistic->update("MMAP Splatting Masks...", E_HEIGHTMAP_GENERATION_STATUS_STARTED);
        m_container->mmapMTextures(filename);
        m_generatorStatistic->update("MMAP Splatting Masks...", E_HEIGHTMAP_GENERATION_STATUS_ENDED);
    });
    completionOperation->addDependency(mmapMasksOperation);
    
    CSharedThreadOperation generateSplattingDTexturesOperation = std::make_shared<CThreadOperation>(E_THREAD_OPERATION_QUEUE_BACKGROUND);
    generateSplattingDTexturesOperation->setExecutionBlock([this, filename, renderTechniqueAccessor, splattingDTextures](void) {
        
        m_generatorStatistic->update("Splatting Textures Generation...", E_HEIGHTMAP_GENERATION_STATUS_STARTED);
        CHeightmapTextureGenerator::generateSplattingDTextures(renderTechniqueAccessor, m_container, filename, splattingDTextures);
        m_generatorStatistic->update("Splatting Textures Generation...", E_HEIGHTMAP_GENERATION_STATUS_ENDED);
    });
    completionOperation->addDependency(generateSplattingDTexturesOperation);
    
    CSharedThreadOperation mmapDTexturesOperation = std::make_shared<CThreadOperation>(E_THREAD_OPERATION_QUEUE_BACKGROUND);
    mmapDTexturesOperation->setExecutionBlock([this, filename](void) {
        
        m_generatorStatistic->update("MMAP Splatting Textures...", E_HEIGHTMAP_GENERATION_STATUS_STARTED);
        m_container->mmapDTextures(filename);
        m_generatorStatistic->update("MMAP Splatting Textures...", E_HEIGHTMAP_GENERATION_STATUS_ENDED);
    });
    completionOperation->addDependency(mmapDTexturesOperation);
    
    CSharedThreadOperation generateSplattingNTexturesOperation = std::make_shared<CThreadOperation>(E_THREAD_OPERATION_QUEUE_BACKGROUND);
    generateSplattingNTexturesOperation->setExecutionBlock([this, filename, renderTechniqueAccessor, splattingNTextures](void) {
        
        m_generatorStatistic->update("Splatting Textures Generation...", E_HEIGHTMAP_GENERATION_STATUS_STARTED);
        CHeightmapTextureGenerator::generateSplattingNTextures(renderTechniqueAccessor, m_container, filename, splattingNTextures);
        m_generatorStatistic->update("Splatting Textures Generation...", E_HEIGHTMAP_GENERATION_STATUS_ENDED);
    });
    completionOperation->addDependency(generateSplattingNTexturesOperation);
    
    CSharedThreadOperation mmapNTexturesOperation = std::make_shared<CThreadOperation>(E_THREAD_OPERATION_QUEUE_BACKGROUND);
    mmapNTexturesOperation->setExecutionBlock([this, filename](void) {
        
        m_generatorStatistic->update("MMAP Splatting Textures...", E_HEIGHTMAP_GENERATION_STATUS_STARTED);
        m_container->mmapNTextures(filename);
        m_generatorStatistic->update("MMAP Splatting Textures...", E_HEIGHTMAP_GENERATION_STATUS_ENDED);
    });
    completionOperation->addDependency(mmapNTexturesOperation);

    completionOperation->addToExecutionQueue();
}

void CHeightmapAccessor::generateMesh(i32 index, E_LANDSCAPE_CHUNK_LOD LOD)
{
    std::shared_ptr<CVertexBuffer> vbo = std::make_shared<CVertexBuffer>(m_container->getVBOMmap(index)->getSize(),
                                                                         GL_STATIC_DRAW,
                                                                         m_container->getVBOMmap(index)->getPointer());
    vbo->unlock();
    
    m_container->getIBOMmap(index, LOD)->updateSourcePointer();
    
    std::shared_ptr<CIndexBuffer> ibo = std::make_shared<CIndexBuffer>(m_container->getIBOMmap(index, LOD)->getSize(),
                                                                       GL_DYNAMIC_DRAW,
                                                                       m_container->getIBOMmap(index, LOD)->getSourcePointer());
    ibo->unlock();
    
    std::ostringstream stringstream;
    stringstream<<"chunk_"<<index<<"_"<<LOD<<std::endl;
    std::shared_ptr<CMesh> mesh = CMesh::construct(stringstream.str(), vbo, ibo,
                                                   std::get<0>(m_chunksBounds[index]), std::get<1>(m_chunksBounds[index]));
    std::get<0>(m_chunksMetadata[index]) = mesh;
}

void CHeightmapAccessor::generateQuadTree(i32 index)
{
    assert(std::get<5>(m_chunksMetadata[index]));
    std::lock_guard<std::mutex> guard(*std::get<5>(m_chunksMetadata[index]).get());
    
    assert(std::get<0>(m_chunksMetadata[index]) != nullptr);
    CSharedQuadTree quadTree = std::make_shared<CQuadTree>();
    quadTree->generate(std::get<0>(m_chunksMetadata[index])->getVertexBuffer(),
                       std::get<0>(m_chunksMetadata[index])->getIndexBuffer(),
                       std::get<0>(m_chunksMetadata[index])->getMaxBound(),
                       std::get<0>(m_chunksMetadata[index])->getMinBound(),
                       4, m_container->getChunkLODSize(E_LANDSCAPE_CHUNK_LOD_01).x);
    
    std::get<1>(m_chunksMetadata[index]) = quadTree;
}

void CHeightmapAccessor::generateSplattingTextures(i32 index, E_LANDSCAPE_CHUNK_LOD LOD)
{
    {
        std::ostringstream stringstream;
        stringstream<<"DTexture_"<<index<<std::endl;
        
        ui32 DTextureId;
        ieGenTextures(1, &DTextureId);
        
        CSharedTexture DTexture = CTexture::constructCustomTexture(stringstream.str(), DTextureId,
                                                                   m_container->getTexturesLODSize(LOD).x,
                                                                   m_container->getTexturesLODSize(LOD).y);
        DTexture->setWrapMode(GL_CLAMP_TO_EDGE);
        DTexture->setMagFilter(GL_LINEAR);
        DTexture->setMinFilter(GL_LINEAR_MIPMAP_NEAREST);
        
        DTexture->bind();
        
        ieTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                     m_container->getTexturesLODSize(LOD).x, m_container->getTexturesLODSize(LOD).y,
                     0, GL_RGBA, GL_UNSIGNED_BYTE, m_container->getSplattingDTexturesMmap(index, LOD)->getPointer());
        ieGenerateMipmap(GL_TEXTURE_2D);
        
        std::get<2>(m_chunksMetadata[index]) = DTexture;
    }
    {
        std::ostringstream stringstream;
        stringstream<<"NTexture_"<<index<<std::endl;
        
        ui32 NTextureId;
        ieGenTextures(1, &NTextureId);
        
        CSharedTexture NTexture = CTexture::constructCustomTexture(stringstream.str(), NTextureId,
                                                                   m_container->getTexturesLODSize(LOD).x,
                                                                   m_container->getTexturesLODSize(LOD).y);
        NTexture->setWrapMode(GL_CLAMP_TO_EDGE);
        NTexture->setMagFilter(GL_LINEAR);
        NTexture->setMinFilter(GL_LINEAR_MIPMAP_NEAREST);
        
        NTexture->bind();
        
        ieTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                     m_container->getTexturesLODSize(LOD).x, m_container->getTexturesLODSize(LOD).y,
                     0, GL_RGBA, GL_UNSIGNED_BYTE, m_container->getSplattingNTexturesMmap(index, LOD)->getPointer());
        ieGenerateMipmap(GL_TEXTURE_2D);
        
        std::get<3>(m_chunksMetadata[index]) = NTexture;
    }
}

void CHeightmapAccessor::updateSplattingTextures(i32 index)
{
    {
        CSharedTexture DTexture = std::get<2>(m_chunksMetadata[index]);
        if(DTexture)
        {
            E_LANDSCAPE_CHUNK_LOD LOD = std::get<4>(m_chunksMetadata[index]);
            DTexture->bind();
            ieTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                         m_container->getTexturesLODSize(LOD).x, m_container->getTexturesLODSize(LOD).y,
                         0, GL_RGBA, GL_UNSIGNED_BYTE, m_container->getSplattingDTexturesMmap(index, LOD)->getPointer());
            ieGenerateMipmap(GL_TEXTURE_2D);
        }
    }
    {
        CSharedTexture NTexture = std::get<3>(m_chunksMetadata[index]);
        if(NTexture)
        {
            E_LANDSCAPE_CHUNK_LOD LOD = std::get<4>(m_chunksMetadata[index]);
            NTexture->bind();
            ieTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                         m_container->getTexturesLODSize(LOD).x, m_container->getTexturesLODSize(LOD).y,
                         0, GL_RGBA, GL_UNSIGNED_BYTE, m_container->getSplattingNTexturesMmap(index, LOD)->getPointer());
            ieGenerateMipmap(GL_TEXTURE_2D);
        }
    }
}

void CHeightmapAccessor::runLoading(i32 i, i32 j, E_LANDSCAPE_CHUNK_LOD LOD,
                                    const std::function<void(CSharedMeshRef)>& callbackMeshLoaded,
                                    const std::function<void(CSharedQuadTreeRef)>& callbackQuadTreeLoaded,
                                    const std::function<void(CSharedTextureRef, CSharedTextureRef)>& callbackTexturesLoaded)
{
    ui32 index = i + j * m_container->getChunksNum().x;
    if(m_executedOperations[index] != nullptr || !m_isGenerated)
    {
        return;
    }
    
    std::get<0>(m_callbacks[index]) = callbackMeshLoaded;
    std::get<1>(m_callbacks[index]) = callbackQuadTreeLoaded;
    std::get<2>(m_callbacks[index]) = callbackTexturesLoaded;
    
    if(std::get<0>(m_chunksMetadata[index]) != nullptr)
    {
        std::get<0>(m_chunksMetadata[index]) = nullptr;
    }
    if(std::get<1>(m_chunksMetadata[index]) != nullptr)
    {
        std::get<1>(m_chunksMetadata[index]) = nullptr;
    }
    if(std::get<2>(m_chunksMetadata[index]) != nullptr)
    {
        std::get<2>(m_chunksMetadata[index]) = nullptr;
    }
    if(std::get<3>(m_chunksMetadata[index]) != nullptr)
    {
        std::get<3>(m_chunksMetadata[index]) = nullptr;
    }
    std::get<4>(m_chunksMetadata[index]) = LOD;
    
    assert(std::get<0>(m_callbacks[index]) != nullptr);
    assert(std::get<1>(m_callbacks[index]) != nullptr);
    
    assert(std::get<0>(m_chunksMetadata[index]) == nullptr);
    assert(std::get<1>(m_chunksMetadata[index]) == nullptr);
    assert(std::get<2>(m_chunksMetadata[index]) == nullptr);
    assert(std::get<3>(m_chunksMetadata[index]) == nullptr);
    assert(std::get<4>(m_chunksMetadata[index]) != E_LANDSCAPE_CHUNK_LOD_UNKNOWN);
    
    CSharedThreadOperation completionOperation = std::make_shared<CThreadOperation>(E_THREAD_OPERATION_QUEUE_MAIN);
    completionOperation->setExecutionBlock([this, index](void) {
        assert(std::get<1>(m_callbacks[index]) != nullptr);
        std::get<1>(m_callbacks[index])(std::get<1>(m_chunksMetadata[index]));
        m_executedOperations[index] = nullptr;
    });
    
    CSharedThreadOperation createMeshOperation = std::make_shared<CThreadOperation>(E_THREAD_OPERATION_QUEUE_MAIN);
    createMeshOperation->setExecutionBlock([this, index, LOD](void) {
        CHeightmapAccessor::generateMesh(index, LOD);
        assert(std::get<0>(m_callbacks[index]));
        std::get<0>(m_callbacks[index])(std::get<0>(m_chunksMetadata[index]));
    });
    completionOperation->addDependency(createMeshOperation);
    
    CSharedThreadOperation generateSplattingTexturesOperation = std::make_shared<CThreadOperation>(E_THREAD_OPERATION_QUEUE_MAIN);
    generateSplattingTexturesOperation->setExecutionBlock([this, index, LOD](void) {
        CHeightmapAccessor::generateSplattingTextures(index, LOD);
        assert(std::get<2>(m_callbacks[index]));
        std::get<2>(m_callbacks[index])(std::get<2>(m_chunksMetadata[index]), std::get<3>(m_chunksMetadata[index]));
    });
    completionOperation->addDependency(generateSplattingTexturesOperation);
    
    CSharedThreadOperation generateQuadTreeOperation = std::make_shared<CThreadOperation>(E_THREAD_OPERATION_QUEUE_BACKGROUND);
    generateQuadTreeOperation->setExecutionBlock([this, index](void) {
        CHeightmapAccessor::generateQuadTree(index);
    });
    completionOperation->addDependency(generateQuadTreeOperation);
    
    assert(m_executedOperations[index] == nullptr);
    m_executedOperations[index] = completionOperation;
    
    completionOperation->setCancelBlock([this, index](void) {
        assert(m_executedOperations[index] != nullptr);
        CHeightmapAccessor::eraseChunkMetadata(index);
    });
    completionOperation->addToExecutionQueue();
}

void CHeightmapAccessor::runUnLoading(i32 i, i32 j)
{
    ui32 index = i + j * m_container->getChunksNum().x;
    
    if(m_executedOperations[index])
    {
        m_executedOperations[index]->cancel();
    }
    else
    {
        CHeightmapAccessor::eraseChunkMetadata(index);
    }
}


f32 CHeightmapAccessor::getAngle(const glm::vec3& point_01,
                                 const glm::vec3& point_02,
                                 const glm::vec3& point_03)
{
    f32 vectorLength_01 = sqrt(pow(point_02.x - point_01.x, 2) + pow(point_02.y - point_01.y, 2) + pow(point_02.z - point_01.z, 2));
    f32 vectorLength_02 = sqrt(pow(point_03.x - point_01.x, 2) + pow(point_03.y - point_01.y, 2) + pow(point_03.z - point_01.z, 2));
    
    f32 scalar = (point_02.x - point_01.x) * (point_03.x - point_01.x) +
    (point_02.y - point_01.y) * (point_03.y - point_01.y) +
    (point_02.z - point_01.z) * (point_03.z - point_01.z);
    return scalar / (vectorLength_01 * vectorLength_02);
}

f32 CHeightmapAccessor::getHeight(const glm::vec3& position) const
{
    return CHeightmapAccessor::getHeight(m_container, position);
}

glm::vec3 CHeightmapAccessor::getNormal(const glm::vec3& position) const
{
    return CHeightmapAccessor::getNormal(m_container, position);
}

glm::vec2 CHeightmapAccessor::getAngles(const glm::vec3& position) const
{
    return CHeightmapAccessor::getAngles(m_container, position);
}

f32 CHeightmapAccessor::getHeight(std::shared_ptr<CHeightmapContainer> container, const glm::vec3& position)
{
    f32 _x = position.x / 1.0;
    f32 _z = position.z / 1.0;
    i32 x = static_cast<i32>(floor(_x));
    i32 z = static_cast<i32>(floor(_z));
    f32 dx = _x - x;
    f32 dy = _z - z;
    
    if((x < 0) || (z < 0) || (x > (container->getMainSize().x - 1)) || (z > (container->getMainSize().y - 1)))
    {
        return 0.0;
    }
    
    f32 height_00 = container->getVertexPosition(x, z).y;
    f32 height_01 = container->getVertexPosition(x, z).y;
    if(z < (container->getMainSize().y - 1) && z >= 0)
    {
        height_01 = container->getVertexPosition(x, z + 1).y;
    }
    
    f32 height_10 = container->getVertexPosition(x, z).y;
    if(x < (container->getMainSize().x - 1) && x >= 0)
    {
        height_10 = container->getVertexPosition(x + 1, z).y;
    }
    
    f32 height_11 = container->getVertexPosition(x, z).y;
    if(z < (container->getMainSize().y - 1) && z >= 0 && x < (container->getMainSize().x - 1) && x >= 0)
    {
        height_11 = container->getVertexPosition(x + 1, z + 1).y;
    }
    
    f32 height_0 = height_00 * (1.0f - dy) + height_01 * dy;
    f32 height_1 = height_10 * (1.0f - dy) + height_11 * dy;
    return height_0 * (1.0f - dx) + height_1 * dx;
}

glm::vec3 CHeightmapAccessor::getNormal(std::shared_ptr<CHeightmapContainer> container, const glm::vec3& position)
{
    f32 _x = position.x / 1.0;
    f32 _z = position.z / 1.0;
    i32 x = static_cast<i32>(floor(_x));
    i32 z = static_cast<i32>(floor(_z));
    
    if((x < 0) || (z < 0) || (x > (container->getMainSize().x - 1)) || (z > (container->getMainSize().y - 1)))
    {
        return glm::vec3(0.0f, 1.0f, 0.0f);
    }
    
    glm::vec3 normal_00 = container->getUncompressedVertexNormal(x, z);
    
    glm::vec3 normal_01 = container->getUncompressedVertexNormal(x, z);
    if(z < (container->getMainSize().y - 1) && z >= 0)
    {
        normal_01 = container->getUncompressedVertexNormal(x, z + 1);
    }
    
    glm::vec3 normal_10 = container->getUncompressedVertexNormal(x, z);
    if(x < (container->getMainSize().x - 1) && x >= 0)
    {
        normal_10 = container->getUncompressedVertexNormal(x + 1, z);
    }
    
    glm::vec3 normal_11 = container->getUncompressedVertexNormal(x, z);
    if(z < (container->getMainSize().y - 1) && z >= 0 && x < (container->getMainSize().x - 1) && x >= 0)
    {
        normal_11 = container->getUncompressedVertexNormal(x + 1, z + 1);
    }
    
    glm::vec3 normal = normal_00 + normal_01 + normal_10 + normal_11 / 4.0f;
    return glm::normalize(glm::vec3(normal.x, normal.y, normal.z));
}

glm::vec2 CHeightmapAccessor::getAngles(std::shared_ptr<CHeightmapContainer> container, const glm::vec3& position)
{
    f32 offset = 0.25;
    glm::vec3 point_01 = position;
    glm::vec3 point_02 = glm::vec3(position.x, position.y + offset, position.z);
    f32 height =  CHeightmapAccessor::getHeight(container, glm::vec3(position.x + offset, 0.0f, position.z));
    glm::vec3 point_03 = glm::vec3(position.x + offset, height, position.z);
    height = CHeightmapAccessor::getHeight(container, glm::vec3(position.x, 0.0f, position.z + offset));
    glm::vec3 point_04 = glm::vec3(position.x, height, position.z + offset);
    
    f32 angle_01 = CHeightmapAccessor::getAngle(point_01, point_02, point_03);
    f32 angle_02 = CHeightmapAccessor::getAngle(point_01, point_02, point_04);
    
    return glm::vec2(glm::degrees(acos(angle_02) - M_PI_2), glm::degrees(asin(angle_01)));
}

void CHeightmapAccessor::update(void)
{
    if(!m_updateHeightmapOperations.empty())
    {
        CSharedThreadOperation operation = m_updateHeightmapOperations.front();
        if(!operation->isExecuted() && !operation->isCompleted() && !operation->isCanceled())
        {
            operation->addToExecutionQueue();
        }
        else if(operation->isCompleted() || operation->isCanceled())
        {
            m_updateHeightmapOperations.pop();
        }
    }
}

void CHeightmapAccessor::updateVertices(const std::vector<glm::vec3>& vertices,
                                        const glm::ivec2& minBound, const glm::ivec2& maxBound)
{
    CHeightmapGeometryGenerator::updateVertices(m_container, vertices);
    
    std::set<std::shared_ptr<CVertexBuffer>> updatedVBOs;
    for(ui32 i = 0; i < vertices.size(); ++i)
    {
        i32 x = static_cast<i32>(vertices.at(i).x);
        i32 z = static_cast<i32>(vertices.at(i).z);
        
        ui8 containsInVBOSize = 0;
        glm::ivec2 *containsInVBO = m_container->attachedVerticesToVBO(x, z, &containsInVBOSize);
        assert(containsInVBO != nullptr);
        
        for(ui32 j = 0; j < containsInVBOSize; ++j)
        {
            if(std::get<0>(m_chunksMetadata[containsInVBO[j].x]) != nullptr)
            {
                std::get<0>(m_chunksMetadata[containsInVBO[j].x])->getVertexBuffer()->lock()[containsInVBO[j].y].m_position =
                m_container->getVertexPosition(x, z);
                std::get<0>(m_chunksMetadata[containsInVBO[j].x])->getVertexBuffer()->lock()[containsInVBO[j].y].m_normal =
                m_container->getCompressedVertexNormal(x, z);
                
                updatedVBOs.insert(std::get<0>(m_chunksMetadata[containsInVBO[j].x])->getVertexBuffer());
            }
        }
    }
    
    for(auto vbo : updatedVBOs)
    {
        vbo->unlock();
    }
    
    CSharedThreadOperation executedOperation = nullptr;
    if(!m_updateHeightmapOperations.empty() && m_updateHeightmapOperations.front()->isExecuted())
    {
        executedOperation = m_updateHeightmapOperations.front();
    }
    while (!m_updateHeightmapOperations.empty())
    {
        m_updateHeightmapOperations.pop();
    }
    if(executedOperation)
    {
        m_updateHeightmapOperations.push(executedOperation);
    }
    
    for(ui32 i = 0; i < m_container->getChunksNum().x; ++i)
    {
        for(ui32 j = 0; j < m_container->getChunksNum().y; ++j)
        {
            ui32 index = i + j * m_container->getChunksNum().x;
            if(CBoundingBox::isPointInXZ(glm::vec2(minBound.x, minBound.y), std::get<0>(m_chunksBounds[index]), std::get<1>(m_chunksBounds[index])) ||
               CBoundingBox::isPointInXZ(glm::vec2(maxBound.x, minBound.y), std::get<0>(m_chunksBounds[index]), std::get<1>(m_chunksBounds[index])) ||
               CBoundingBox::isPointInXZ(glm::vec2(minBound.x, maxBound.y), std::get<0>(m_chunksBounds[index]), std::get<1>(m_chunksBounds[index])) ||
               CBoundingBox::isPointInXZ(glm::vec2(maxBound.x, maxBound.y), std::get<0>(m_chunksBounds[index]), std::get<1>(m_chunksBounds[index])))
            {
                CHeightmapAccessor::createBoundingBox(i, j);
                
                CSharedThreadOperation updateGeometryOperation = std::make_shared<CThreadOperation>(E_THREAD_OPERATION_QUEUE_BACKGROUND);
                updateGeometryOperation->setExecutionBlock([this, i , j, index](void) {
                    /*CHeightmapGeometryGenerator::generateSmoothTexcoord(m_container, index);*/
                    CHeightmapGeometryGenerator::generateTangentSpace(m_container, index);
                });
                m_updateHeightmapOperations.push(updateGeometryOperation);
                
                CSharedThreadOperation generateHeightmapTextureOperation = std::make_shared<CThreadOperation>(E_THREAD_OPERATION_QUEUE_MAIN);
                generateHeightmapTextureOperation->setExecutionBlock([this, minBound, maxBound](void) {
                    CHeightmapTextureGenerator::generateDeepTexture(m_container, false, minBound.x, minBound.y,
                                                                    maxBound.x - minBound.x, maxBound.y - minBound.y);
                });
                m_updateHeightmapOperations.push(generateHeightmapTextureOperation);
                
                CSharedThreadOperation updateSplattingMTextureOperation = std::make_shared<CThreadOperation>(E_THREAD_OPERATION_QUEUE_BACKGROUND);
                updateSplattingMTextureOperation->setExecutionBlock([this, i , j](void) {
                    CHeightmapTextureGenerator::generateSplattingMTexture(m_container, i, j);
                });
                m_updateHeightmapOperations.push(updateSplattingMTextureOperation);
                
                CSharedThreadOperation updateSplattingDTextureOperation = std::make_shared<CThreadOperation>(E_THREAD_OPERATION_QUEUE_MAIN);
                updateSplattingDTextureOperation->setExecutionBlock([this, i , j](void) {
                    CHeightmapTextureGenerator::generateSplattingDTexture(m_renderTechniqueAccessor, m_container, m_splattingDTextures, i, j);
                });
                m_updateHeightmapOperations.push(updateSplattingDTextureOperation);
                
                CSharedThreadOperation updateSplattingNTextureOperation = std::make_shared<CThreadOperation>(E_THREAD_OPERATION_QUEUE_MAIN);
                updateSplattingNTextureOperation->setExecutionBlock([this, i , j](void) {
                    CHeightmapTextureGenerator::generateSplattingNTexture(m_renderTechniqueAccessor, m_container, m_splattingNTextures, i, j);
                });
                m_updateHeightmapOperations.push(updateSplattingNTextureOperation);
                
                CSharedThreadOperation updateTexturesOperation = std::make_shared<CThreadOperation>(E_THREAD_OPERATION_QUEUE_MAIN);
                updateTexturesOperation->setExecutionBlock([this, index](void) {
                    CHeightmapAccessor::updateSplattingTextures(index);
                });
                m_updateHeightmapOperations.push(updateTexturesOperation);
            }
        }
    }
}

