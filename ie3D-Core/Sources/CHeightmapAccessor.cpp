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
#include "CThreadOperation.h"
#include "CMesh.h"
#include "CQuadTree.h"

extern ui32 g_heightmapGUID;

CHeightmapAccessor::CHeightmapAccessor(void) :
m_container(std::make_shared<CHeightmapContainer>()),
m_isGenerated(false)
{
    
}

CHeightmapAccessor::~CHeightmapAccessor(void)
{
    
}

void CHeightmapAccessor::createLoadingOperations(void)
{
    m_callbacks.clear();
    m_callbacks.resize(m_container->getChunksNum().x * m_container->getChunksNum().y,
                       std::make_tuple(nullptr, nullptr));
    
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
                            std::make_tuple(nullptr, nullptr, E_LANDSCAPE_CHUNK_LOD_UNKNOWN));
}

void CHeightmapAccessor::eraseMetadataContainers(void)
{
    m_chunksMetadata.clear();
}

void CHeightmapAccessor::eraseChunkMetadata(i32 index)
{
    std::get<0>(m_chunksMetadata[index]) = nullptr;
    std::get<1>(m_chunksMetadata[index]) = nullptr;
    std::get<2>(m_chunksMetadata[index]) = E_LANDSCAPE_CHUNK_LOD_UNKNOWN;
    
    std::get<0>(m_callbacks[index]) = nullptr;
    std::get<1>(m_callbacks[index]) = nullptr;
    
    m_executedOperations[index] = nullptr;
}

void CHeightmapAccessor::generate(const std::string& filename, const std::function<void(void)>& callback)
{
    m_isGenerated = false;
    
    CSharedThreadOperation completionOperation = std::make_shared<CThreadOperation>(E_THREAD_OPERATION_QUEUE_MAIN);
    completionOperation->setExecutionBlock([this, callback](void) {
        m_isGenerated = true;
        callback();
    });
    
    CSharedThreadOperation generateOperation = std::make_shared<CThreadOperation>(E_THREAD_OPERATION_QUEUE_BACKGROUND);
    generateOperation->setExecutionBlock([this, filename](void) {
        std::tuple<glm::ivec2, std::vector<f32>> heights = CHeightmapLoader::getHeights(filename);
        CHeightmapGeometryGenerator::generate(m_container, filename, std::get<0>(heights), std::get<1>(heights));
        
        CHeightmapAccessor::createLoadingOperations();
        CHeightmapAccessor::createMetadataContainers();
        CHeightmapAccessor::createBoundingBoxes();
    });
    completionOperation->addDependency(generateOperation);
    
    CSharedThreadOperation mmapOperation = std::make_shared<CThreadOperation>(E_THREAD_OPERATION_QUEUE_BACKGROUND);
    mmapOperation->setExecutionBlock([this, filename](void) {
        m_container->mmap(filename);
    });
    completionOperation->addDependency(mmapOperation);
    
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
    stringstream<<"chunk_"<<index<<"_"<<LOD<<"_"<<g_heightmapGUID<<std::endl;
    std::shared_ptr<CMesh> mesh = CMesh::construct(stringstream.str(), vbo, ibo,
                                                   std::get<0>(m_chunksBounds[index]), std::get<1>(m_chunksBounds[index]));
    std::cout<<"used indices: "<<ibo->getUsedSize()<<std::endl;
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

void CHeightmapAccessor::runLoading(i32 i, i32 j, E_LANDSCAPE_CHUNK_LOD LOD,
                                    const std::function<void(CSharedMeshRef)>& meshLoadedCallback,
                                    const std::function<void(CSharedQuadTreeRef)>& quadTreeLoadedCallback)
{
    ui32 index = i + j * m_container->getChunksNum().x;
    if(m_executedOperations[index] != nullptr || !m_isGenerated)
    {
        return;
    }
    
    std::get<0>(m_callbacks[index]) = meshLoadedCallback;
    std::get<1>(m_callbacks[index]) = quadTreeLoadedCallback;
    
    if(std::get<0>(m_chunksMetadata[index]) != nullptr)
    {
        std::get<0>(m_chunksMetadata[index]) = nullptr;
    }
    if(std::get<1>(m_chunksMetadata[index]) != nullptr)
    {
        std::get<1>(m_chunksMetadata[index]) = nullptr;
    }
    std::get<2>(m_chunksMetadata[index]) = LOD;
    
    assert(std::get<0>(m_callbacks[index]) != nullptr);
    assert(std::get<1>(m_callbacks[index]) != nullptr);
    
    assert(std::get<0>(m_chunksMetadata[index]) == nullptr);
    assert(std::get<1>(m_chunksMetadata[index]) == nullptr);
    assert(std::get<2>(m_chunksMetadata[index]) != E_LANDSCAPE_CHUNK_LOD_UNKNOWN);
    
    CSharedThreadOperation completionOperation = std::make_shared<CThreadOperation>(E_THREAD_OPERATION_QUEUE_MAIN);
    completionOperation->setExecutionBlock([this, index](void) {
        assert(std::get<1>(m_callbacks[index]) != nullptr);
        std::get<1>(m_callbacks[index])(std::get<1>(m_chunksMetadata[index]));
        m_executedOperations[index] = nullptr;
    });
    
    CSharedThreadOperation createMeshOperation = std::make_shared<CThreadOperation>(E_THREAD_OPERATION_QUEUE_MAIN);
    createMeshOperation->setExecutionBlock([this, index, LOD](void) {
        CHeightmapAccessor::generateMesh(index, LOD);
        assert(std::get<0>(m_callbacks[index]) != nullptr);
        std::get<0>(m_callbacks[index])(std::get<0>(m_chunksMetadata[index]));
    });
    completionOperation->addDependency(createMeshOperation);
    
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