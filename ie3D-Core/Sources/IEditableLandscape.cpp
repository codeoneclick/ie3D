//
//  IEditableLandscape.cpp
//  ie3D-Core
//
//  Created by Sergey Sergeev on 5/27/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#include "IEditableLandscape.h"
#include "CHeightmap.h"
#include "CLandscapeChunk.h"

IEditableLandscape::IEditableLandscape(void) :
m_editableSize(4),
m_editableStrength(1),
m_editableFalloffCoefficient(0),
m_editableSmoothCoefficient(0),
m_heightmapGenerator(nullptr)
{
    
}

IEditableLandscape::~IEditableLandscape(void)
{
    
}

void IEditableLandscape::setEditableSize(ui32 value)
{
    m_editableSize = value;
}

void IEditableLandscape::setEditableStrength(ui32 value)
{
    m_editableStrength = value;
}

void IEditableLandscape::setEditableFalloffCoefficient(ui32 value)
{
    m_editableFalloffCoefficient = value;
}

void IEditableLandscape::setEditableSmoothCoefficient(ui32 value)
{
    m_editableSmoothCoefficient = value;
}

void IEditableLandscape::pressureHeight(const glm::vec3& point, f32 pressureForce)
{
	i32 minIndX = static_cast<i32>(floor(point.x - m_editableSize));
	i32 minIndZ = static_cast<i32>(floor(point.z - m_editableSize));
	i32 maxIndX = static_cast<i32>(floor(point.x + m_editableSize));
	i32 maxIndZ = static_cast<i32>(floor(point.z + m_editableSize));
    std::vector<glm::vec3> vertices;
	for(i32 x = minIndX; x < maxIndX; x++)
	{
        for(i32 z = minIndZ; z < maxIndZ; z++)
        {
            if((x < 0) || (z < 0) ||
               x >= m_heightmapGenerator->getSize().x ||
               z >= m_heightmapGenerator->getSize().y)
                continue;
            
            f32 height = m_heightmapGenerator->getHeight(glm::vec3(x, 0.0, z));
            f32 distance = glm::length(glm::vec3(x - point.x, 0.0, z - point.z));
            
            if (distance > m_editableSize)
                continue;
            
            f32 riseCoefficient = distance / static_cast<f32>(m_editableSize);
            riseCoefficient = 1.0 - riseCoefficient * riseCoefficient;
            f32 deltaHeight = pressureForce * riseCoefficient * (m_editableStrength * 0.1);
            height += deltaHeight;
            
            i32 delimiter = 1;
            for(i32 i = x - m_editableSmoothCoefficient; i <= x + m_editableSmoothCoefficient; ++i)
            {
                for(i32 j = z - m_editableSmoothCoefficient; j <= z + m_editableSmoothCoefficient; ++j)
                {
                    if(i > 0 && j > 0 &&
                       i < m_heightmapGenerator->getSize().x && j < m_heightmapGenerator->getSize().y)
                    {
                        height += m_heightmapGenerator->getHeight(glm::vec3(i, 0.0, j));
                        delimiter++;
                    }
                }
            }
            height /= static_cast<f32>(delimiter);

            vertices.push_back(glm::vec3(x, height, z));
        }
    }
    
    glm::ivec2 minBound = glm::ivec2(MAX_VALUE(minIndX, 0),
                                     MAX_VALUE(minIndZ, 0));
    
    glm::ivec2 maxBound = glm::ivec2(MIN_VALUE(maxIndX, m_heightmapGenerator->getSize().x - 1),
                                     MIN_VALUE(maxIndZ, m_heightmapGenerator->getSize().y - 1));
    
    m_heightmapGenerator->updateHeightmap(minBound, maxBound, vertices);
}

void IEditableLandscape::generateVertecesData(const glm::ivec2& size, f32 frequency, i32 octaves, ui32 seed)
{
    assert(m_heightmapGenerator != nullptr);
    glm::ivec2 previousSize = m_heightmapGenerator->getSize();
    if(size.x != previousSize.x ||
       size.y != previousSize.y)
    {
        glm::ivec2 numChunks = m_heightmapGenerator->getNumChunks();
        for(i32 i = 0; i < numChunks.x; ++i)
        {
            for(i32 j = 0; j < numChunks.y; ++j)
            {
                i32 index = i + j * numChunks.x;
                if(m_chunks[index] != nullptr)
                {
                    m_heightmapGenerator->runChunkUnLoading(i, j);
                    m_chunks[index] = nullptr;
                }
            }
        }
    }
    
    m_heightmapGenerator->generateVertecesData(size, frequency, octaves, seed);
    
    if(size.x != previousSize.x ||
       size.y != previousSize.y)
    {
        std::vector<CSharedLandscapeChunk> chunks;
        m_chunks.swap(chunks);
        m_chunks.resize(m_heightmapGenerator->getNumChunks().x * m_heightmapGenerator->getNumChunks().y);
    }
}