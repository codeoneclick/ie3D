//
//  CHeightmapGeometryGenerator.cpp
//  ie3D-Core
//
//  Created by sergey.sergeev on 5/11/15.
//  Copyright (c) 2015 Sergey Sergeev. All rights reserved.
//

#include "CHeightmapGeometryGenerator.h"
#include "CHeightmapContainer.h"
#include "CHeightmapLoader.h"
#include "HDeclaration.h"
#include "CThreadOperation.h"
#include "CVertexBuffer.h"
#include "CIndexBuffer.h"

CHeightmapGeometryGenerator::CHeightmapGeometryGenerator(void)
{
   
}

CHeightmapGeometryGenerator::~CHeightmapGeometryGenerator(void)
{
    
}

void CHeightmapGeometryGenerator::generate(const std::shared_ptr<CHeightmapContainer>& container, const std::string& filename,
                                           const glm::ivec2& size, const std::vector<f32>& heights)
{
    container->create(size);
    
    /*if(!CHeightmapLoader::isUncompressedVerticesMMAPExist(filename) ||
       !CHeightmapLoader::isCompressedVerticesMMAPExist(filename) ||
       !CHeightmapLoader::isFacesMMAPExist(filename))*/
    {
        CHeightmapGeometryGenerator::createVerticesMetadata(container, size, heights, filename);
        CHeightmapGeometryGenerator::createVBOsMetadata(container, filename);
        CHeightmapGeometryGenerator::createIBOsMetadata(container, filename);
    }
    /*else
    {
        if(!CHeightmapLoader::isVBOsMMAPExist(filename))
        {
            CHeightmapGeometryGenerator::createVBOsMetadata(container, filename);
        }
        
        if(!CHeightmapLoader::isIBOsMMAPExist(filename))
        {
            CHeightmapGeometryGenerator::createIBOsMetadata(container, filename);
        }
    }*/
}

void CHeightmapGeometryGenerator::createVerticesMetadata(const std::shared_ptr<CHeightmapContainer>& container, const glm::ivec2& size, const std::vector<f32>& heights,
                                                         const std::string& filename)
{
    CHeightmapContainer::SUncomressedVertex *uncompressedVertices = container->getUncopressedVertices();
    CHeightmapContainer::SCompressedVertex *compressedVertices = container->getCompressedVertices();
    CHeightmapContainer::SFace *faces = container->getFaces();
    
    for(ui32 i = 0; i < size.x; ++i)
    {
        for(ui32 j = 0; j < size.y; ++j)
        {
            uncompressedVertices[i + j * size.x].m_position = glm::vec3(i, heights[i + j * size.x], j);
            uncompressedVertices[i + j * size.x].m_texcoord = glm::vec2(static_cast<f32>(i) / static_cast<f32>(size.x),
                                                                        static_cast<f32>(j) / static_cast<f32>(size.y));
        }
    }
    
    ui32 index = 0;
    for(ui32 i = 0; i < (size.x - 1); ++i)
    {
        for(ui32 j = 0; j < (size.y - 1); ++j)
        {
            faces[index].m_indexes[0] = i + j * size.x;
            uncompressedVertices[faces[index].m_indexes[0]].m_containsInFace.push_back(index);
            glm::vec3 point_01 = uncompressedVertices[faces[index].m_indexes[0]].m_position;
            faces[index].m_indexes[1] = i + (j + 1) * size.x;
            uncompressedVertices[faces[index].m_indexes[1]].m_containsInFace.push_back(index);
            glm::vec3 point_02 = uncompressedVertices[faces[index].m_indexes[1]].m_position;
            faces[index].m_indexes[2] = i + 1 + j * size.x;
            uncompressedVertices[faces[index].m_indexes[2]].m_containsInFace.push_back(index);
            glm::vec3 point_03 = uncompressedVertices[faces[index].m_indexes[2]].m_position;
            
            glm::vec3 edge_01 = point_02 - point_01;
            glm::vec3 edge_02 = point_03 - point_01;
            glm::vec3 normal = glm::cross(edge_01, edge_02);
            f32 sin = glm::length(normal) / (glm::length(edge_01) * glm::length(edge_02));
            faces[index].m_normal = glm::normalize(normal) * asinf(sin);
            index++;
            
            faces[index].m_indexes[0] = i + (j + 1) * size.x;
            uncompressedVertices[faces[index].m_indexes[0]].m_containsInFace.push_back(index);
            point_01 = uncompressedVertices[faces[index].m_indexes[0]].m_position;
            faces[index].m_indexes[1] = i + 1 + (j + 1) * size.x;
            uncompressedVertices[faces[index].m_indexes[1]].m_containsInFace.push_back(index);
            point_02 = uncompressedVertices[faces[index].m_indexes[1]].m_position;
            faces[index].m_indexes[2] = i + 1 + j * size.x;
            uncompressedVertices[faces[index].m_indexes[2]].m_containsInFace.push_back(index);
            point_03 = uncompressedVertices[faces[index].m_indexes[2]].m_position;
            
            edge_01 = point_02 - point_01;
            edge_02 = point_03 - point_01;
            normal = glm::cross(edge_01, edge_02);
            sin = glm::length(normal) / (glm::length(edge_01) * glm::length(edge_02));
            faces[index].m_normal = glm::normalize(normal) * asinf(sin);
            index++;
        }
    }
    
    for(ui32 i = 0; i < size.x * size.y; ++i)
    {
        assert(uncompressedVertices[i].m_containsInFace.size() != 0 && uncompressedVertices[i].m_containsInFace.size() <= CHeightmapContainer::kMaxContainsInFace);
        glm::vec3 normal = faces[uncompressedVertices[i].m_containsInFace[0]].m_normal;
        for(ui32 j = 1; j < uncompressedVertices[i].m_containsInFace.size(); ++j)
        {
            normal += faces[uncompressedVertices[i].m_containsInFace[j]].m_normal;
        }
        normal = glm::normalize(normal);
        uncompressedVertices[i].m_normal = normal;
    }
    
    for(ui32 i = 0; i < size.x * size.y; ++i)
    {
        compressedVertices[i].m_position = uncompressedVertices[i].m_position;
        compressedVertices[i].m_texcoord = glm::packUnorm2x16(uncompressedVertices[i].m_texcoord);
        compressedVertices[i].m_normal = glm::packSnorm4x8(glm::vec4(uncompressedVertices[i].m_normal, 0.0f));
    }
    
    { // writing compressed vertices metadata
        std::ofstream stream;
        stream.open(CHeightmapLoader::getCompressedVerticesMMAPFilename(filename), std::ios::binary | std::ios::out | std::ios::trunc);
        if(!stream.is_open())
        {
            assert(false);
        }
        
        for(ui32 i = 0; i < size.x * size.y; ++i)
        {
            stream.write((char*)&compressedVertices[i], sizeof(CHeightmapContainer::SCompressedVertex));
        }
        stream.close();
    }
    
    { // writing uncompressed vertices metadata
        std::ofstream stream;
        stream.open(CHeightmapLoader::getUncompressedVerticesMMAPFilename(filename), std::ios::binary | std::ios::out | std::ios::trunc);
        if(!stream.is_open())
        {
            assert(false);
        }
        
        for(ui32 i = 0; i < size.x * size.y; ++i)
        {
            stream.write((char*)&uncompressedVertices[i], sizeof(CHeightmapContainer::SUncomressedVertex));
        }
        stream.close();
    }
    
    { // writing faces metadata
        std::ofstream stream;
        stream.open(CHeightmapLoader::getFacesMMAPFilename(filename), std::ios::binary | std::ios::out | std::ios::trunc);
        if(!stream.is_open())
        {
            assert(false);
        }
        
        for(ui32 i = 0; i < (size.x - 1) * (size.y - 1) * 2; ++i)
        {
            stream.write((char*)&faces[i], sizeof(CHeightmapContainer::SFace));
        }
        stream.close();
    }
}

void CHeightmapGeometryGenerator::createVBOsMetadata(const std::shared_ptr<CHeightmapContainer>& container, const std::string& filename)
{
    std::ofstream stream;
    stream.open(CHeightmapLoader::getVBOsMMAPFilename(filename), std::ios::binary | std::ios::out | std::ios::trunc);
    if(!stream.is_open())
    {
        assert(false);
    }
    
    glm::ivec2 verticesOffset(0);
    SAttributeVertex vertex;
    for(ui32 i = 0; i < container->getChunksNum().x; ++i)
    {
        verticesOffset.y = 0;
        for(ui32 j = 0; j < container->getChunksNum().y; ++j)
        {
            for(ui32 x = 0; x < container->getChunkSize().x; ++x)
            {
                for(ui32 y = 0; y < container->getChunkSize().y; ++y)
                {
                    vertex.m_position = container->getVertexPosition(x + verticesOffset.x, y + verticesOffset.y);
                    vertex.m_normal = container->getCompressedVertexNormal(x + verticesOffset.x, y + verticesOffset.y);
                    vertex.m_texcoord = glm::packUnorm2x16(glm::vec2(static_cast<f32>(x) / static_cast<f32>(container->getChunkSize().x),
                                                                     static_cast<f32>(y) / static_cast<f32>(container->getChunkSize().y)));
                    
                    ui32 index = y + x * container->getChunkSize().y;
                    container->attachUncompressedVertexToVBO(x + verticesOffset.x, y + verticesOffset.y,
                                                             i + j * container->getChunksNum().x, index);
                    
                    stream.write((char*)&vertex, sizeof(SAttributeVertex));
                }
            }
            verticesOffset.y += container->getChunkSize().y - 1;
        }
        verticesOffset.x += container->getChunkSize().x - 1;
    }
    stream.close();
}

void CHeightmapGeometryGenerator::createIBOsMetadata(const std::shared_ptr<CHeightmapContainer>& container, const std::string& filename)
{
    std::ofstream stream;
    stream.open(CHeightmapLoader::getIBOsMMAPFilename(filename), std::ios::binary | std::ios::out | std::ios::trunc);
    if(!stream.is_open())
    {
        assert(false);
    }
    
    for(ui32 i = 0; i < container->getChunksNum().x; ++i)
    {
        for(ui32 j = 0; j < container->getChunksNum().y; ++j)
        {
            for(ui32 k = 0; k < E_LANDSCAPE_CHUNK_LOD_MAX; ++k)
            {
                glm::ivec2 currentChunkSize = glm::ivec2(container->getChunkLODSize(static_cast<E_LANDSCAPE_CHUNK_LOD>(k)).x % 2 == 0 ?
                                                         container->getChunkLODSize(static_cast<E_LANDSCAPE_CHUNK_LOD>(k)).x :
                                                         container->getChunkLODSize(static_cast<E_LANDSCAPE_CHUNK_LOD>(k)).x - 1,
                                                         
                                                         container->getChunkLODSize(static_cast<E_LANDSCAPE_CHUNK_LOD>(k)).y % 2 == 0 ?
                                                         container->getChunkLODSize(static_cast<E_LANDSCAPE_CHUNK_LOD>(k)).y :
                                                         container->getChunkLODSize(static_cast<E_LANDSCAPE_CHUNK_LOD>(k)).y - 1);
                
                glm::ivec2 verticesLODOffset = glm::ivec2(MAX_VALUE((container->getChunkSize().x - 1) / currentChunkSize.x, 1),
                                                          MAX_VALUE((container->getChunkSize().y - 1) / currentChunkSize.y, 1));
                
                
                i32 verticesLineOffset = container->getChunkSize().x;
                glm::ivec2 currentChunkLODStartIndex = glm::ivec2(0);
                currentChunkLODStartIndex.x += k != E_LANDSCAPE_CHUNK_LOD_01 ? 1 : 0;
                currentChunkLODStartIndex.y += k != E_LANDSCAPE_CHUNK_LOD_01 ? 1 : 0;
                
                std::vector<ui16> additionIndices;
                if(k != E_LANDSCAPE_CHUNK_LOD_01)
                {
                    std::vector<ui16> currentLODEdgeIndices;
                    for(ui32 x = 0; x <= currentChunkSize.x; ++x)
                    {
                        ui32 index = x * verticesLODOffset.x + verticesLineOffset * verticesLODOffset.y;
                        currentLODEdgeIndices.push_back(index);
                    }
                    
                    ui32 currentLODIndex = 0;
                    for(ui32 x = 0; x < container->getChunkSize().x; ++x)
                    {
                        ui32 mainLODIndex = x;
                        if(currentLODEdgeIndices.size() > currentLODIndex + 1)
                        {
                            if(x != 0 && x % verticesLODOffset.x == 0)
                            {
                                additionIndices.push_back(currentLODEdgeIndices[currentLODIndex]);
                                additionIndices.push_back(currentLODEdgeIndices[currentLODIndex + 1]);
                                additionIndices.push_back(mainLODIndex);
                                currentLODIndex++;
                            }
                            
                            if(currentLODEdgeIndices.size() - 1 == currentLODIndex)
                            {
                                break;
                            }
                            else
                            {
                                additionIndices.push_back(mainLODIndex);
                                additionIndices.push_back(currentLODEdgeIndices[currentLODIndex]);
                                additionIndices.push_back(mainLODIndex + 1);
                            }
                        }
                    }
                    currentLODEdgeIndices.clear();
                    
                    for(ui32 x = 0; x <= currentChunkSize.x; ++x)
                    {
                        ui32 index = x * verticesLODOffset.x + verticesLineOffset * verticesLODOffset.y * (currentChunkSize.y - 1);
                        currentLODEdgeIndices.push_back(index);
                    }
                    
                    currentLODIndex = 0;
                    for(ui32 x = 0; x < container->getChunkSize().x; ++x)
                    {
                        ui32 mainLODIndex = x + verticesLineOffset * (container->getChunkSize().x - 1);
                        if(currentLODEdgeIndices.size() > currentLODIndex + 1)
                        {
                            if(x != 0 && x % verticesLODOffset.x == 0)
                            {
                                additionIndices.push_back(currentLODEdgeIndices[currentLODIndex]);
                                additionIndices.push_back(mainLODIndex);
                                additionIndices.push_back(currentLODEdgeIndices[currentLODIndex + 1]);
                                currentLODIndex++;
                            }
                            
                            if(currentLODEdgeIndices.size() - 1 == currentLODIndex)
                            {
                                break;
                            }
                            else
                            {
                                additionIndices.push_back(currentLODEdgeIndices[currentLODIndex]);
                                additionIndices.push_back(mainLODIndex);
                                additionIndices.push_back(mainLODIndex + 1);
                            }
                        }
                    }
                    currentLODEdgeIndices.clear();
                    
                    for(ui32 y = 0; y <= currentChunkSize.y; ++y)
                    {
                        ui32 index = verticesLODOffset.x + y * verticesLineOffset * verticesLODOffset.y;
                        currentLODEdgeIndices.push_back(index);
                    }
                    
                    currentLODIndex = 0;
                    for(ui32 y = 0; y < container->getChunkSize().y; ++y)
                    {
                        ui32 mainLODIndex = y * container->getChunkSize().y;
                        if(currentLODEdgeIndices.size() > currentLODIndex + 1)
                        {
                            if(y != 0 && y % verticesLODOffset.y == 0)
                            {
                                additionIndices.push_back(currentLODEdgeIndices[currentLODIndex]);
                                additionIndices.push_back(mainLODIndex);
                                additionIndices.push_back(currentLODEdgeIndices[currentLODIndex + 1]);
                                currentLODIndex++;
                            }
                            
                            if(currentLODEdgeIndices.size() - 1 == currentLODIndex)
                            {
                                break;
                            }
                            else
                            {
                                additionIndices.push_back(currentLODEdgeIndices[currentLODIndex]);
                                additionIndices.push_back(mainLODIndex);
                                additionIndices.push_back(mainLODIndex + container->getChunkSize().y);
                            }
                        }
                    }
                    currentLODEdgeIndices.clear();
                    
                    for(ui32 y = 0; y <= currentChunkSize.y; ++y)
                    {
                        ui32 index = verticesLODOffset.x * (currentChunkSize.x - 1) + y * verticesLineOffset * verticesLODOffset.y;
                        currentLODEdgeIndices.push_back(index);
                    }
                    
                    currentLODIndex = 0;
                    for(ui32 y = 0; y < container->getChunkSize().y; ++y)
                    {
                        ui32 mainLODIndex = (container->getChunkSize().x - 1) + y * container->getChunkSize().y;
                        if(currentLODEdgeIndices.size() > currentLODIndex + 1)
                        {
                            if(y != 0 && y % verticesLODOffset.y == 0)
                            {
                                additionIndices.push_back(currentLODEdgeIndices[currentLODIndex]);
                                additionIndices.push_back(currentLODEdgeIndices[currentLODIndex + 1]);
                                additionIndices.push_back(mainLODIndex);
                                currentLODIndex++;
                            }
                            
                            if(currentLODEdgeIndices.size() - 1 == currentLODIndex)
                            {
                                break;
                            }
                            else
                            {
                                additionIndices.push_back(mainLODIndex);
                                additionIndices.push_back(currentLODEdgeIndices[currentLODIndex]);
                                additionIndices.push_back(mainLODIndex + container->getChunkSize().y);
                            }
                        }
                    }
                    currentLODEdgeIndices.clear();
                }
                
                currentChunkSize.x -= k != E_LANDSCAPE_CHUNK_LOD_01 ? 2 : 0;
                currentChunkSize.y -= k != E_LANDSCAPE_CHUNK_LOD_01 ? 2 : 0;
                
                ui32 indicesCount = currentChunkSize.x * currentChunkSize.y * 6 + static_cast<ui32>(additionIndices.size());
                
                currentChunkSize.x += k != E_LANDSCAPE_CHUNK_LOD_01 ? 1 : 0;
                currentChunkSize.y += k != E_LANDSCAPE_CHUNK_LOD_01 ? 1 : 0;
                
                ui16* indices = new ui16[indicesCount];
                
                ui32 index = 0;
                
                for(ui32 x = 0; x < additionIndices.size(); ++x)
                {
                    indices[index] = additionIndices[x];
                    index++;
                }
                
                for(ui32 x = currentChunkLODStartIndex.x; x < currentChunkSize.x; ++x)
                {
                    for(ui32 y = currentChunkLODStartIndex.y; y < currentChunkSize.y; ++y)
                    {
                        indices[index] = x * verticesLODOffset.x + y * verticesLODOffset.y * verticesLineOffset;
                        index++;
                        indices[index] = x * verticesLODOffset.x + (y * verticesLODOffset.y + verticesLODOffset.y) * verticesLineOffset;
                        index++;
                        indices[index] = x * verticesLODOffset.x + verticesLODOffset.x + y * verticesLODOffset.y * verticesLineOffset;
                        index++;
                        
                        indices[index] = x * verticesLODOffset.x + (y * verticesLODOffset.y + verticesLODOffset.y) * verticesLineOffset;
                        index++;
                        indices[index] = x * verticesLODOffset.x + verticesLODOffset.x + (y * verticesLODOffset.y + verticesLODOffset.y) * verticesLineOffset;
                        index++;
                        indices[index] = x * verticesLODOffset.x + verticesLODOffset.x + y * verticesLODOffset.y * verticesLineOffset;
                        index++;
                    }
                }
                
                for(ui32 index_01 = 0; index_01 < 2; ++index_01)
                {
                    for(ui32 index_02 = 0; index_02 < indicesCount; ++index_02)
                    {
                        stream.write((char*)&indices[index_02], sizeof(ui16));
                    }
                }
                delete [] indices;
            }
        }
    }
    stream.close();
}

