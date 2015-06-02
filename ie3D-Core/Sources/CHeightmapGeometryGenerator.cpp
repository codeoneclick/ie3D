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
    if(!CHeightmapLoader::isUncompressedVerticesMMAPExist(filename) ||
       !CHeightmapLoader::isCompressedVerticesMMAPExist(filename) ||
       !CHeightmapLoader::isFacesMMAPExist(filename))
    {
        CHeightmapGeometryGenerator::createVerticesMetadata(container, size, heights, filename);
        CHeightmapGeometryGenerator::createVBOsMetadata(container, filename);
        CHeightmapGeometryGenerator::createIBOsMetadata(container, filename);
    }
    else
    {
        if(!CHeightmapLoader::isVBOsMMAPExist(filename))
        {
            CHeightmapGeometryGenerator::createVBOsMetadata(container, filename);
        }
        
        if(!CHeightmapLoader::isIBOsMMAPExist(filename))
        {
            CHeightmapGeometryGenerator::createIBOsMetadata(container, filename);
        }
    }
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
            uncompressedVertices[faces[index].m_indexes[0]].m_containsInFace[uncompressedVertices[faces[index].m_indexes[0]].m_containsInFaceSize++] = index;
            glm::vec3 point_01 = uncompressedVertices[faces[index].m_indexes[0]].m_position;
            faces[index].m_indexes[1] = i + (j + 1) * size.x;
            uncompressedVertices[faces[index].m_indexes[1]].m_containsInFace[uncompressedVertices[faces[index].m_indexes[1]].m_containsInFaceSize++] = index;
            glm::vec3 point_02 = uncompressedVertices[faces[index].m_indexes[1]].m_position;
            faces[index].m_indexes[2] = i + 1 + j * size.x;
            uncompressedVertices[faces[index].m_indexes[2]].m_containsInFace[uncompressedVertices[faces[index].m_indexes[2]].m_containsInFaceSize++] = index;
            glm::vec3 point_03 = uncompressedVertices[faces[index].m_indexes[2]].m_position;
            
            glm::vec3 edge_01 = point_02 - point_01;
            glm::vec3 edge_02 = point_03 - point_01;
            glm::vec3 normal = glm::cross(edge_01, edge_02);
            f32 sin = glm::length(normal) / (glm::length(edge_01) * glm::length(edge_02));
            faces[index].m_normal = glm::normalize(normal) * asinf(sin);
            index++;
            
            faces[index].m_indexes[0] = i + (j + 1) * size.x;
            uncompressedVertices[faces[index].m_indexes[0]].m_containsInFace[uncompressedVertices[faces[index].m_indexes[0]].m_containsInFaceSize++] = index;
            point_01 = uncompressedVertices[faces[index].m_indexes[0]].m_position;
            faces[index].m_indexes[1] = i + 1 + (j + 1) * size.x;
            uncompressedVertices[faces[index].m_indexes[1]].m_containsInFace[uncompressedVertices[faces[index].m_indexes[1]].m_containsInFaceSize++] = index;
            point_02 = uncompressedVertices[faces[index].m_indexes[1]].m_position;
            faces[index].m_indexes[2] = i + 1 + j * size.x;
            uncompressedVertices[faces[index].m_indexes[2]].m_containsInFace[uncompressedVertices[faces[index].m_indexes[2]].m_containsInFaceSize++] = index;
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
        assert(uncompressedVertices[i].m_containsInFaceSize != 0 && uncompressedVertices[i].m_containsInFaceSize <= CHeightmapContainer::kMaxContainsInFace);
        glm::vec3 normal = faces[uncompressedVertices[i].m_containsInFace[0]].m_normal;
        for(ui32 j = 1; j < uncompressedVertices[i].m_containsInFaceSize; ++j)
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

void CHeightmapGeometryGenerator::generateTangentSpace(const std::shared_ptr<CHeightmapContainer>& container, const std::string& filename)
{
    if(!CHeightmapLoader::isTangentSpace_MMapExist(filename))
    {
        CHeightmapGeometryGenerator::createTangentSpace(container, filename);
    }
}

void CHeightmapGeometryGenerator::createTangentSpace(const std::shared_ptr<CHeightmapContainer>& container, const std::string& filename)
{
    for(ui32 i = 0; i < container->getChunksNum().x; ++i)
    {
        for(ui32 j = 0; j < container->getChunksNum().y; ++j)
        {
            ui32 index = i + j * container->getChunksNum().x;
            CHeightmapGeometryGenerator::generateTangentSpace(container, index);
        }
    }
    
    std::ofstream stream;
    stream.open(CHeightmapLoader::getTangentSpace_MMapFilename(filename), std::ios::binary | std::ios::out | std::ios::trunc);
    if(!stream.is_open())
    {
        assert(false);
    }
    
    ui8 value = 1;
    stream.write((char *)&value, sizeof(ui8));
    stream.close();
}

void CHeightmapGeometryGenerator::generateTangentSpace(const std::shared_ptr<CHeightmapContainer>& container, ui32 index)
{
    std::vector<std::pair<glm::vec3, ui32>> tangents;
    SAttributeVertex* vertices = container->getVBOMmap(index)->getPointer();
    ui32 numVertices = container->getVBOMmap(index)->getSize();
    
    tangents.resize(numVertices, std::make_pair(glm::vec3(0.0f), 0));
    
    ui16* indices = container->getIBOMmap(index, E_LANDSCAPE_CHUNK_LOD_01)->getSourcePointer();
    ui32 numIndices = container->getIBOMmap(index, E_LANDSCAPE_CHUNK_LOD_01)->getSize();
    
    std::pair<i32, i32> minmax = std::make_pair(INT16_MAX, INT16_MIN);
    for (ui32 i = 0; i < numIndices; i += 3)
    {
        glm::vec3 point_01 = vertices[indices[i + 0]].m_position;
        glm::vec3 point_02 = vertices[indices[i + 1]].m_position;
        glm::vec3 point_03 = vertices[indices[i + 2]].m_position;
        
        glm::vec2 texcoord_01 = glm::unpackUnorm2x16(vertices[indices[i + 0]].m_texcoord);
        glm::vec2 texcoord_02 = glm::unpackUnorm2x16(vertices[indices[i + 1]].m_texcoord);
        glm::vec2 texcoord_03 = glm::unpackUnorm2x16(vertices[indices[i + 2]].m_texcoord);
        
        glm::vec3 tangent = CHeightmapGeometryGenerator::generateTangent(point_01, point_02, point_03,
                                                                         texcoord_01, texcoord_02, texcoord_03);
        
        assert(indices[i + 0] < numVertices);
        assert(indices[i + 1] < numVertices);
        assert(indices[i + 2] < numVertices);
        
        tangents[indices[i + 0]].first += tangent;
        tangents[indices[i + 0]].second++;
        
        tangents[indices[i + 1]].first += tangent;
        tangents[indices[i + 1]].second++;
        
        tangents[indices[i + 2]].first += tangent;
        tangents[indices[i + 2]].second++;
        
        minmax.first = minmax.first < indices[i + 0] ? minmax.first : indices[i + 0];
        minmax.second = minmax.second > indices[i + 0] ? minmax.second : indices[i + 0];
        
        minmax.first = minmax.first < indices[i + 1] ? minmax.first : indices[i + 1];
        minmax.second = minmax.second > indices[i + 1] ? minmax.second : indices[i + 1];
        
        minmax.first = minmax.first < indices[i + 2] ? minmax.first : indices[i + 2];
        minmax.second = minmax.second > indices[i + 2] ? minmax.second : indices[i + 2];
    }
    
    for(i32 i = minmax.first; i <= minmax.second; i++)
    {
        glm::vec3 tangent = tangents[i].first / static_cast<f32>(tangents[i].second);
        glm::vec4 normal = glm::unpackSnorm4x8(vertices[i].m_normal);
        tangent = CHeightmapGeometryGenerator::ortogonalize(glm::vec3(normal.x, normal.y, normal.z), tangent);
        vertices[i].m_tangent = glm::packSnorm4x8(glm::vec4(tangent.x, tangent.y, tangent.z, 0.0));
    }
}

glm::vec3 CHeightmapGeometryGenerator::generateTangent(const glm::vec3& point_01, const glm::vec3& point_02, const glm::vec3& point_03,
                                                       const glm::vec2& texcoord_01, const glm::vec2& texcoord_02, const glm::vec2& texcoord_03)
{
    glm::vec3 P = point_02 - point_01;
    glm::vec3 Q = point_03 - point_01;
    f32 s1 = texcoord_02.x - texcoord_01.x;
    f32 t1 = texcoord_02.y - texcoord_01.y;
    f32 s2 = texcoord_03.x - texcoord_01.x;
    f32 t2 = texcoord_03.y - texcoord_01.y;
    f32 pqMatrix[2][3];
    pqMatrix[0][0] = P[0];
    pqMatrix[0][1] = P[1];
    pqMatrix[0][2] = P[2];
    pqMatrix[1][0] = Q[0];
    pqMatrix[1][1] = Q[1];
    pqMatrix[1][2] = Q[2];
    f32 temp = 1.0f / ( s1 * t2 - s2 * t1);
    f32 stMatrix[2][2];
    stMatrix[0][0] = t2 * temp;
    stMatrix[0][1] = -t1 * temp;
    stMatrix[1][0] = -s2 * temp;
    stMatrix[1][1] = s1 * temp;
    f32 tbMatrix[2][3];
    tbMatrix[0][0] = stMatrix[0][0] * pqMatrix[0][0] + stMatrix[0][1] * pqMatrix[1][0];
    tbMatrix[0][1] = stMatrix[0][0] * pqMatrix[0][1] + stMatrix[0][1] * pqMatrix[1][1];
    tbMatrix[0][2] = stMatrix[0][0] * pqMatrix[0][2] + stMatrix[0][1] * pqMatrix[1][2];
    tbMatrix[1][0] = stMatrix[1][0] * pqMatrix[0][0] + stMatrix[1][1] * pqMatrix[1][0];
    tbMatrix[1][1] = stMatrix[1][0] * pqMatrix[0][1] + stMatrix[1][1] * pqMatrix[1][1];
    tbMatrix[1][2] = stMatrix[1][0] * pqMatrix[0][2] + stMatrix[1][1] * pqMatrix[1][2];
    return glm::normalize(glm::vec3(tbMatrix[0][0], tbMatrix[0][1], tbMatrix[0][2]));
}

glm::vec3 CHeightmapGeometryGenerator::getClosestPointOnLine(const glm::vec3& a, const glm::vec3& b, const glm::vec3& p)
{
    glm::vec3 c = p - a;
    glm::vec3 v = b - a;
    f32 d = v.length();
    v = glm::normalize(v);
    f32 t = glm::dot( v, c );
    
    if ( t < 0.0f )
        return a;
    if ( t > d )
        return b;
    v *= t;
    return ( a + v );
}

glm::vec3 CHeightmapGeometryGenerator::ortogonalize(const glm::vec3& v1, const glm::vec3& v2)
{
    glm::vec3 v2ProjV1 = CHeightmapGeometryGenerator::getClosestPointOnLine( v1, -v1, v2 );
    glm::vec3 res = v2 - v2ProjV1;
    res = glm::normalize(res);
    return res;
}

void CHeightmapGeometryGenerator::generateSmoothTexcoord(const std::shared_ptr<CHeightmapContainer> &container, const std::string &filename)
{
    CHeightmapGeometryGenerator::createSmoothTexcoord(container, filename);
}

void CHeightmapGeometryGenerator::createSmoothTexcoord(const std::shared_ptr<CHeightmapContainer> &container, const std::string &filename)
{
    for(ui32 i = 0; i < container->getChunksNum().x; ++i)
    {
        for(ui32 j = 0; j < container->getChunksNum().y; ++j)
        {
            ui32 index = i + j * container->getChunksNum().x;
            CHeightmapGeometryGenerator::generateSmoothTexcoord(container, index);
        }
    }
}

void CHeightmapGeometryGenerator::generateSmoothTexcoord(const std::shared_ptr<CHeightmapContainer>& container, ui32 index)
{
    SAttributeVertex* vertices = container->getVBOMmap(index)->getPointer();
    ui32 numVertices = container->getVBOMmap(index)->getSize();
    
    const glm::ivec2 size = glm::ivec2(sqrt(numVertices));
    
    std::vector<f32> widths_x;
    std::vector<f32> widths_y;
    
    for(i32 x = 0; x < size.x; ++x)
    {
        f32 width_x = 0;
        f32 width_y = 0;
        for(i32 y = 0; y < size.y; ++y)
        {
            if(y + 1 < size.y)
            {
                width_x += glm::distance(vertices[x + y * size.y].m_position,
                                         vertices[x + (y + 1) * size.y].m_position);
                
                width_y += glm::distance(vertices[y + x * size.x].m_position,
                                         vertices[y + 1 + x * size.x].m_position);
            }
        }
        widths_y.push_back(width_y);
        widths_x.push_back(width_x);
    }
    
    for(i32 x = 0; x < size.x; ++x)
    {
        f32 width_x = widths_x[x];
        f32 distance_summ_x = 0.0f;
        
        for(i32 y = 0; y < size.y; ++y)
        {
            if(y + 1 < size.y)
            {
                f32 distance_x = glm::distance(vertices[x + y * size.y].m_position,
                                               vertices[x + (y + 1) * size.y].m_position);
                glm::vec2 texcoord = glm::unpackUnorm2x16(vertices[x + (y + 1) * size.y].m_texcoord);
                texcoord.x = (distance_summ_x + distance_x) / width_x;
                distance_summ_x += distance_x;
                texcoord = glm::clamp(texcoord, 0.0f, 1.0f);
                vertices[x + (y + 1) * size.y].m_texcoord = glm::packUnorm2x16(texcoord);
            }
            else
            {
                glm::vec2 texcoord = glm::unpackUnorm2x16(vertices[x + y * size.y].m_texcoord);
                texcoord.x = 1.0f;
                texcoord = glm::clamp(texcoord, 0.0f, 1.0f);
                vertices[x + y * size.y].m_texcoord = glm::packUnorm2x16(texcoord);
            }
        }
    }
    
    for(i32 y = 0; y < size.y; ++y)
    {
        f32 width_y = widths_y[y];
        f32 distance_summ_y = 0.0f;
        
        for(i32 x = 0; x < size.x; ++x)
        {
            if(x + 1 < size.x)
            {
                f32 distance_y = glm::distance(vertices[x + y * size.y].m_position,
                                               vertices[x + 1 + y * size.y].m_position);
                glm::vec2 texcoord = glm::unpackUnorm2x16(vertices[x + 1 + y * size.y].m_texcoord);
                texcoord.y = (distance_summ_y + distance_y) / width_y;
                distance_summ_y += distance_y;
                texcoord = glm::clamp(texcoord, 0.0f, 1.0f);
                vertices[x + 1 + y * size.y].m_texcoord = glm::packUnorm2x16(texcoord);
            }
            else
            {
                glm::vec2 texcoord = glm::unpackUnorm2x16(vertices[x + y * size.y].m_texcoord);
                texcoord.y = 1.0f;
                texcoord = glm::clamp(texcoord, 0.0f, 1.0f);
                vertices[x + y * size.y].m_texcoord = glm::packUnorm2x16(texcoord);
            }
        }
    }
}

void CHeightmapGeometryGenerator::generateAttachesToVBO(const std::shared_ptr<CHeightmapContainer> &container, const std::string &filename)
{
    if(!CHeightmapLoader::isAttachesToVBO_MMapExist(filename))
    {
        CHeightmapGeometryGenerator::createAttachesToVBO(container, filename);
    }
}

void CHeightmapGeometryGenerator::createAttachesToVBO(const std::shared_ptr<CHeightmapContainer> &container, const std::string &filename)
{
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
                    ui32 index = y + x * container->getChunkSize().y;
                    container->attachUncompressedVertexToVBO(x + verticesOffset.x, y + verticesOffset.y,
                                                             i + j * container->getChunksNum().x, index);
                }
            }
            verticesOffset.y += container->getChunkSize().y - 1;
        }
        verticesOffset.x += container->getChunkSize().x - 1;
    }
    
    std::ofstream stream;
    stream.open(CHeightmapLoader::getAttachesToVBO_MMapFilename(filename), std::ios::binary | std::ios::out | std::ios::trunc);
    if(!stream.is_open())
    {
        assert(false);
    }
    
    ui8 value = 1;
    stream.write((char *)&value, sizeof(ui8));
    stream.close();
}

void CHeightmapGeometryGenerator::updateVertices(const std::shared_ptr<CHeightmapContainer>& container, const std::vector<glm::vec3>& vertices)
{
    for(ui32 i = 0; i < vertices.size(); ++i)
    {
        i32 x = static_cast<i32>(vertices.at(i).x);
        i32 z = static_cast<i32>(vertices.at(i).z);
        i32 index = x + z * container->getMainSize().x;
        container->getUncopressedVertices()[index].m_position = vertices.at(i);
    }
    
    for(ui32 i = 0; i < vertices.size(); ++i)
    {
        i32 x = static_cast<i32>(vertices.at(i).x);
        i32 z = static_cast<i32>(vertices.at(i).z);
        i32 index = x + z * container->getMainSize().x;
        
        ui8 containsInVBOSize = 0;
        glm::ivec2 *containsInVBO = container->attachedVerticesToVBO(x, z, &containsInVBOSize);
        assert(containsInVBO != nullptr);
        
        for(ui32 j = 0; j < containsInVBOSize; ++j)
        {
            ui32 face = container->getUncopressedVertices()[index].m_containsInFace[j];
            
            glm::vec3 point_01 = container->getUncopressedVertices()[container->getFaces()[face].m_indexes[0]].m_position;
            glm::vec3 point_02 = container->getUncopressedVertices()[container->getFaces()[face].m_indexes[1]].m_position;
            glm::vec3 point_03 = container->getUncopressedVertices()[container->getFaces()[face].m_indexes[2]].m_position;
            
            glm::vec3 edge_01 = point_02 - point_01;
            glm::vec3 edge_02 = point_03 - point_01;
            glm::vec3 normal = glm::cross(edge_01, edge_02);
            f32 sin = glm::length(normal) / (glm::length(edge_01) * glm::length(edge_02));
            container->getFaces()[face].m_normal = glm::normalize(normal) * asinf(sin);
        }
    }
    
    for(ui32 i = 0; i < vertices.size(); ++i)
    {
        i32 x = static_cast<i32>(vertices.at(i).x);
        i32 z = static_cast<i32>(vertices.at(i).z);
        i32 index = x + z * container->getMainSize().x;
        
        ui8 containsInVBOSize = 0;
        glm::ivec2 *containsInVBO = container->attachedVerticesToVBO(x, z, &containsInVBOSize);
        assert(containsInVBO != nullptr);
        
        glm::vec3 normal = container->getFaces()[container->getUncopressedVertices()[index].m_containsInFace[0]].m_normal;
        for(ui32 j = 1; j < containsInVBOSize; ++j)
        {
            normal += container->getFaces()[container->getUncopressedVertices()[index].m_containsInFace[j]].m_normal;
        }
        container->getUncopressedVertices()[index].m_normal = glm::normalize(normal);
        container->getCompressedVertices()[index].m_position = container->getUncopressedVertices()[index].m_position;
        container->getCompressedVertices()[index].m_normal = glm::packSnorm4x8(glm::vec4(container->getUncopressedVertices()[index].m_normal, 0.0));
    }
}

