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
            
            uncompressedVertices[i + j * size.x].m_texcoord = glm::vec2(static_cast<f32>(i) / static_cast<f32>(m_size.x),
                                                                            static_cast<f32>(j) / static_cast<f32>(m_size.y));
        }
    }
    
    ui32 index = 0;
    for(ui32 i = 0; i < (m_size.x - 1); ++i)
    {
        for(ui32 j = 0; j < (m_size.y - 1); ++j)
        {
            m_faces[index].m_indexes[0] = i + j * m_size.x;
            m_uncompressedVertices[m_faces[index].m_indexes[0]].m_containsInFace[m_uncompressedVertices[m_faces[index].m_indexes[0]].m_containsInFaceSize++] = index;
            glm::vec3 point_01 = m_uncompressedVertices[m_faces[index].m_indexes[0]].m_position;
            m_faces[index].m_indexes[1] = i + (j + 1) * m_size.x;
            m_uncompressedVertices[m_faces[index].m_indexes[1]].m_containsInFace[m_uncompressedVertices[m_faces[index].m_indexes[1]].m_containsInFaceSize++] = index;
            glm::vec3 point_02 = m_uncompressedVertices[m_faces[index].m_indexes[1]].m_position;
            m_faces[index].m_indexes[2] = i + 1 + j * m_size.x;
            m_uncompressedVertices[m_faces[index].m_indexes[2]].m_containsInFace[m_uncompressedVertices[m_faces[index].m_indexes[2]].m_containsInFaceSize++] = index;
            glm::vec3 point_03 = m_uncompressedVertices[m_faces[index].m_indexes[2]].m_position;
            
            glm::vec3 edge_01 = point_02 - point_01;
            glm::vec3 edge_02 = point_03 - point_01;
            glm::vec3 normal = glm::cross(edge_01, edge_02);
            f32 sin = glm::length(normal) / (glm::length(edge_01) * glm::length(edge_02));
            m_faces[index].m_normal = glm::normalize(normal) * asinf(sin);
            index++;
            
            m_faces[index].m_indexes[0] = i + (j + 1) * m_size.x;
            m_uncompressedVertices[m_faces[index].m_indexes[0]].m_containsInFace[ m_uncompressedVertices[m_faces[index].m_indexes[0]].m_containsInFaceSize++] = index;
            point_01 = m_uncompressedVertices[m_faces[index].m_indexes[0]].m_position;
            m_faces[index].m_indexes[1] = i + 1 + (j + 1) * m_size.x;
            m_uncompressedVertices[m_faces[index].m_indexes[1]].m_containsInFace[m_uncompressedVertices[m_faces[index].m_indexes[1]].m_containsInFaceSize++] = index;
            point_02 = m_uncompressedVertices[m_faces[index].m_indexes[1]].m_position;
            m_faces[index].m_indexes[2] = i + 1 + j * m_size.x;
            m_uncompressedVertices[m_faces[index].m_indexes[2]].m_containsInFace[m_uncompressedVertices[m_faces[index].m_indexes[2]].m_containsInFaceSize++] = index;
            point_03 = m_uncompressedVertices[m_faces[index].m_indexes[2]].m_position;
            
            edge_01 = point_02 - point_01;
            edge_02 = point_03 - point_01;
            normal = glm::cross(edge_01, edge_02);
            sin = glm::length(normal) / (glm::length(edge_01) * glm::length(edge_02));
            m_faces[index].m_normal = glm::normalize(normal) * asinf(sin);
            index++;
        }
    }
    
    for(ui32 i = 0; i < m_size.x * m_size.y; ++i)
    {
        assert(m_uncompressedVertices[i].m_containsInFaceSize != 0 && m_uncompressedVertices[i].m_containsInFaceSize <= kMaxContainsInFace);
        glm::vec3 normal = m_faces[m_uncompressedVertices[i].m_containsInFace[0]].m_normal;
        for(ui32 j = 1; j < m_uncompressedVertices[i].m_containsInFaceSize; ++j)
        {
            normal += m_faces[m_uncompressedVertices[i].m_containsInFace[j]].m_normal;
        }
        normal = glm::normalize(normal);
        m_uncompressedVertices[i].m_normal = normal;
    }
}

void CHeightmapGeometryGenerator::writeVerticesMetadata(const std::shared_ptr<CHeightmapContainer>& container, const std::string& filename)
{
    
}