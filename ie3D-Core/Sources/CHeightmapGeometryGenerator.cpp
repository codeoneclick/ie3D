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

CHeightmapGeometryGenerator::CHeightmapGeometryGenerator(void)
{
    
}

CHeightmapGeometryGenerator::~CHeightmapGeometryGenerator(void)
{
    
}

void CHeightmapGeometryGenerator::generate(const std::shared_ptr<CHeightmapContainer>& container, const std::string& filename,
                                           const glm::ivec2& size, const std::vector<f32>& heights, const std::function<void(void)>& callback)
{
    container->create(size);
}

void CHeightmapGeometryGenerator::createVerticesMetadata(const std::shared_ptr<CHeightmapContainer>& container, const glm::ivec2& size, const std::vector<f32>& heights)
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
}

void CHeightmapGeometryGenerator::writeVerticesMetadata(const std::shared_ptr<CHeightmapContainer>& container, const std::string& filename)
{
    CHeightmapContainer::SUncomressedVertex *uncompressedVertices = container->getUncopressedVertices();
    CHeightmapContainer::SCompressedVertex *compressedVertices = container->getCompressedVertices();
    CHeightmapContainer::SFace *faces = container->getFaces();
    glm::ivec2 size = container->getSize();
    
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