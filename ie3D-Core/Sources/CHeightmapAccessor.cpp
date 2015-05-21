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

CHeightmapAccessor::CHeightmapAccessor(void) :
m_container(std::make_shared<CHeightmapContainer>()),
m_isGenerated(false)
{
    CHeightmapLoader::g_heightmapGUID++;
}

CHeightmapAccessor::~CHeightmapAccessor(void)
{
    
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
    }
}

void CHeightmapAccessor::eraseBoundingBoxes(void)
{
    m_chunksBounds.clear();
}

void CHeightmapAccessor::createMetadataContainers(void)
{
    m_chunksMetadata.clear();
    m_chunksMetadata.resize(m_container->getChunksNum().x * m_container->getChunksNum().y,
                            std::make_tuple(nullptr, nullptr, nullptr, E_LANDSCAPE_CHUNK_LOD_UNKNOWN));
}

void CHeightmapAccessor::eraseMetadataContainers(void)
{
    m_chunksMetadata.clear();
}

void CHeightmapAccessor::eraseChunkMetadata(i32 index)
{
    std::get<0>(m_chunksMetadata[index]) = nullptr;
    std::get<1>(m_chunksMetadata[index]) = nullptr;
    std::get<2>(m_chunksMetadata[index]) = nullptr;
    std::get<3>(m_chunksMetadata[index]) = E_LANDSCAPE_CHUNK_LOD_UNKNOWN;
    
    std::get<0>(m_callbacks[index]) = nullptr;
    std::get<1>(m_callbacks[index]) = nullptr;
    
    m_executedOperations[index] = nullptr;
}

void CHeightmapAccessor::generate(const std::string& filename, ISharedRenderTechniqueAccessorRef renderTechniqueAccessor,
                                  const std::array<CSharedTexture, 3>& splattingTextures, const std::function<void(void)>& callback)
{
    m_isGenerated = false;
    
    CSharedThreadOperation completionOperation = std::make_shared<CThreadOperation>(E_THREAD_OPERATION_QUEUE_MAIN);
    completionOperation->setExecutionBlock([this, callback](void) {
        m_isGenerated = true;
        callback();
    });
    
    CSharedThreadOperation generateGeometryOperation = std::make_shared<CThreadOperation>(E_THREAD_OPERATION_QUEUE_BACKGROUND);
    generateGeometryOperation->setExecutionBlock([this, filename](void) {
        
        std::tuple<glm::ivec2, std::vector<f32>> heights = CHeightmapLoader::getHeights(filename);
        
        m_container->init(std::get<0>(heights));
        
        CHeightmapGeometryGenerator::generate(m_container, filename, std::get<0>(heights), std::get<1>(heights));
        CHeightmapAccessor::createLoadingOperations();
        CHeightmapAccessor::createMetadataContainers();
    });
    completionOperation->addDependency(generateGeometryOperation);
    
    CSharedThreadOperation mmapGeometryOperation = std::make_shared<CThreadOperation>(E_THREAD_OPERATION_QUEUE_BACKGROUND);
    mmapGeometryOperation->setExecutionBlock([this, filename](void) {
       
        m_container->mmapGeometry(filename);
        CHeightmapGeometryGenerator::generateTangentSpace(m_container, filename);
        CHeightmapAccessor::createBoundingBoxes();
        
    });
    completionOperation->addDependency(mmapGeometryOperation);
    
    CSharedThreadOperation generateSplattingMasksOperation = std::make_shared<CThreadOperation>(E_THREAD_OPERATION_QUEUE_BACKGROUND);
    generateSplattingMasksOperation->setExecutionBlock([this, filename](void) {
        
        CHeightmapTextureGenerator::generateSplattingMasks(m_container, filename);
    });
    completionOperation->addDependency(generateSplattingMasksOperation);
    
    CSharedThreadOperation mmapMasksOperation = std::make_shared<CThreadOperation>(E_THREAD_OPERATION_QUEUE_BACKGROUND);
    mmapMasksOperation->setExecutionBlock([this, filename](void) {
        
        m_container->mmapMasks(filename);
    });
    completionOperation->addDependency(mmapMasksOperation);
    
    CSharedThreadOperation generateSplattingTexturesOperation = std::make_shared<CThreadOperation>(E_THREAD_OPERATION_QUEUE_MAIN);
    generateSplattingTexturesOperation->setExecutionBlock([this, filename, renderTechniqueAccessor, splattingTextures](void) {
        
        CHeightmapTextureGenerator::generateSplattingTextures(renderTechniqueAccessor, m_container, filename, splattingTextures);
    });
    completionOperation->addDependency(generateSplattingTexturesOperation);
    
    CSharedThreadOperation mmapTexturesOperation = std::make_shared<CThreadOperation>(E_THREAD_OPERATION_QUEUE_MAIN);
    mmapTexturesOperation->setExecutionBlock([this, filename](void) {
        
        m_container->mmapTextures(filename);
    });
    completionOperation->addDependency(mmapTexturesOperation);
    
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
    stringstream<<"chunk_"<<index<<"_"<<LOD<<"_"<<CHeightmapLoader::g_heightmapGUID<<std::endl;
    std::shared_ptr<CMesh> mesh = CMesh::construct(stringstream.str(), vbo, ibo,
                                                   std::get<0>(m_chunksBounds[index]), std::get<1>(m_chunksBounds[index]));
    std::get<0>(m_chunksMetadata[index]) = mesh;
}

void CHeightmapAccessor::generateQuadTree(i32 index)
{
    assert(std::get<0>(m_chunksMetadata[index]) != nullptr);
    
    CSharedQuadTree quadTree = std::make_shared<CQuadTree>();
    
    quadTree->generate(std::get<0>(m_chunksMetadata[index])->getVertexBuffer(),
                       std::get<0>(m_chunksMetadata[index])->getIndexBuffer(),
                       std::get<0>(m_chunksMetadata[index])->getMaxBound(),
                       std::get<0>(m_chunksMetadata[index])->getMinBound(),
                       4, m_container->getChunkLODSize(E_LANDSCAPE_CHUNK_LOD_01).x);
    
    std::get<1>(m_chunksMetadata[index]) = quadTree;
}

void CHeightmapAccessor::generateSplattingTexture(i32 index, E_LANDSCAPE_CHUNK_LOD LOD)
{
    std::ostringstream stringstream;
    stringstream<<"texture_"<<index<<"_"<<CHeightmapLoader::g_heightmapGUID<<std::endl;
    
    ui32 textureId;
    ieGenTextures(1, &textureId);
    
    CSharedTexture texture = CTexture::constructCustomTexture(stringstream.str(), textureId,
                                                              m_container->getTexturesLODSize(LOD).x,
                                                              m_container->getTexturesLODSize(LOD).y);
    texture->setWrapMode(GL_CLAMP_TO_EDGE);
    texture->setMagFilter(GL_LINEAR);
    texture->setMinFilter(GL_LINEAR_MIPMAP_NEAREST);
    
    texture->bind();
    
    ieTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 m_container->getTexturesLODSize(LOD).x, m_container->getTexturesLODSize(LOD).y,
                 0, GL_RGBA, GL_UNSIGNED_BYTE, m_container->getSplattingTexturesMmap(index, LOD)->getPointer());
    ieGenerateMipmap(GL_TEXTURE_2D);
    
    std::get<2>(m_chunksMetadata[index]) = texture;
}

void CHeightmapAccessor::runLoading(i32 i, i32 j, E_LANDSCAPE_CHUNK_LOD LOD,
                                    const std::function<void(CSharedMeshRef)>& meshLoadedCallback,
                                    const std::function<void(CSharedQuadTreeRef)>& quadTreeLoadedCallback,
                                    const std::function<void(CSharedTextureRef)>& textureLoadingCallback)
{
    ui32 index = i + j * m_container->getChunksNum().x;
    if(m_executedOperations[index] != nullptr || !m_isGenerated)
    {
        return;
    }
    
    std::get<0>(m_callbacks[index]) = meshLoadedCallback;
    std::get<1>(m_callbacks[index]) = quadTreeLoadedCallback;
    std::get<2>(m_callbacks[index]) = textureLoadingCallback;
    
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
    std::get<3>(m_chunksMetadata[index]) = LOD;
    
    assert(std::get<0>(m_callbacks[index]) != nullptr);
    assert(std::get<1>(m_callbacks[index]) != nullptr);
    
    assert(std::get<0>(m_chunksMetadata[index]) == nullptr);
    assert(std::get<1>(m_chunksMetadata[index]) == nullptr);
    assert(std::get<2>(m_chunksMetadata[index]) == nullptr);
    assert(std::get<3>(m_chunksMetadata[index]) != E_LANDSCAPE_CHUNK_LOD_UNKNOWN);
    
    CSharedThreadOperation completionOperation = std::make_shared<CThreadOperation>(E_THREAD_OPERATION_QUEUE_MAIN);
    completionOperation->setExecutionBlock([this, index](void) {
        assert(std::get<1>(m_callbacks[index]) != nullptr);
        std::get<1>(m_callbacks[index])(std::get<1>(m_chunksMetadata[index]));
        std::get<2>(m_callbacks[index])(std::get<2>(m_chunksMetadata[index]));
        m_executedOperations[index] = nullptr;
    });
    
    CSharedThreadOperation createMeshOperation = std::make_shared<CThreadOperation>(E_THREAD_OPERATION_QUEUE_MAIN);
    createMeshOperation->setExecutionBlock([this, index, LOD](void) {
        CHeightmapAccessor::generateMesh(index, LOD);
        assert(std::get<0>(m_callbacks[index]) != nullptr);
        std::get<0>(m_callbacks[index])(std::get<0>(m_chunksMetadata[index]));
    });
    completionOperation->addDependency(createMeshOperation);
    
    CSharedThreadOperation generateSplattingTextureOperation = std::make_shared<CThreadOperation>(E_THREAD_OPERATION_QUEUE_MAIN);
    generateSplattingTextureOperation->setExecutionBlock([this, index, LOD](void) {
        CHeightmapAccessor::generateSplattingTexture(index, LOD);
    });
    completionOperation->addDependency(generateSplattingTextureOperation);
    
    CSharedThreadOperation generateQuadTreeOperation = std::make_shared<CThreadOperation>(E_THREAD_OPERATION_QUEUE_BACKGROUND);
    generateQuadTreeOperation->setExecutionBlock([this, index](void) {
        CHeightmapAccessor::generateQuadTree(index);
    });
    completionOperation->addDependency(generateQuadTreeOperation);
    
    assert(m_executedOperations[index] == nullptr);
    m_executedOperations[index] = completionOperation;
    
    std::thread::id runningThreadId = std::this_thread::get_id();
    
    completionOperation->setCancelBlock([this, index, runningThreadId](void) {
        assert(m_executedOperations[index] != nullptr);
        
        if(runningThreadId == std::this_thread::get_id())
        {
            CHeightmapAccessor::eraseChunkMetadata(index);
        }
        else
        {
            CSharedThreadOperation eraseOperation = std::make_shared<CThreadOperation>(E_THREAD_OPERATION_QUEUE_MAIN);
            eraseOperation->setExecutionBlock([this, index](void) {
                CHeightmapAccessor::eraseChunkMetadata(index);
            });
            eraseOperation->addToExecutionQueue();
        }
    });
    completionOperation->addToExecutionQueue();
}

void CHeightmapAccessor::runUnLoading(i32 i, i32 j)
{
    ui32 index = i + j * m_container->getChunksNum().x;
    
    if(m_executedOperations[index] != nullptr)
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