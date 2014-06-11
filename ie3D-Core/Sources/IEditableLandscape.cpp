//
//  IEditableLandscape.cpp
//  ie3D-Core
//
//  Created by Sergey Sergeev on 5/27/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#include "IEditableLandscape.h"
#include "CHeightmapProcessor.h"

IEditableLandscape::IEditableLandscape(void)
{
    
}

IEditableLandscape::~IEditableLandscape(void)
{
    
}

void IEditableLandscape::setHeightmapProcessor(CSharedHeightmapProcessorRef heightmapProcessor)
{
    assert(heightmapProcessor != nullptr);
    m_heightmapProcessor = heightmapProcessor;
}

void IEditableLandscape::pressureHeight(const glm::vec3& point, f32 radius,
                                        bool isSmooth, f32 pressureForce)
{
	i32 minIndX = static_cast<i32>(floor(point.x - radius));
	i32 minIndZ = static_cast<i32>(floor(point.z - radius));
	i32 maxIndX = static_cast<i32>(floor(point.x + radius));
	i32 maxIndZ = static_cast<i32>(floor(point.z + radius));
    std::vector<std::tuple<ui32, ui32, f32>> modifiedHeights;
	for(i32 x = minIndX; x < maxIndX; x++)
	{
        for(i32 z = minIndZ; z < maxIndZ; z++)
        {
            if((x < 0) || (z < 0) ||
               x >= m_heightmapProcessor->getSizeX() ||
               z >= m_heightmapProcessor->getSizeZ())
                continue;
            
            f32 distance = glm::length(glm::vec3(x - point.x, 0.0, z - point.z));
            
            if (distance > radius)
                continue;
            
            f32 riseCoefficient = distance / radius;
            riseCoefficient = 1.0 - riseCoefficient * riseCoefficient;
            f32 deltaHeight = pressureForce * riseCoefficient;
            f32 height = m_heightmapProcessor->getHeight(glm::vec3(x, 0.0, z)) + deltaHeight;
            modifiedHeights.push_back(std::make_tuple(x, z, height));
        }
	}
    m_heightmapProcessor->updateHeightmapData(modifiedHeights);

    /*f32 middleHeight = 0;
    for (i32 x = minIndX; x < maxIndX; x++)
    {
        for (i32 z = minIndZ; z < maxIndZ; z++)
        {
            if((x < 0) || (z < 0) ||
               x >= m_heightmapProcessor->getSizeX() ||
               z >= m_heightmapProcessor->getSizeZ())
                continue;
            
            f32 distance = glm::length(glm::vec3(x - point.x, 0.0, z - point.z));
            f32 coefficient = radius - distance;
            
            float height = m_heightmapProcessor->getHeight(glm::vec3(x, 0.0, z));
            middleHeight = 0;
            if (coefficient > 0)
            {
                middleHeight = height;
                middleHeight += m_heightmapProcessor->getHeight(glm::vec3(x - 1, 0.0, z));
                middleHeight += m_heightmapProcessor->getHeight(glm::vec3(x + 1, 0.0, z));
                middleHeight += m_heightmapProcessor->getHeight(glm::vec3(x - 1, 0.0, z - 1));
                middleHeight += m_heightmapProcessor->getHeight(glm::vec3(x, 0.0, z - 1));
                middleHeight += m_heightmapProcessor->getHeight(glm::vec3(x + 1, 0.0, z - 1));
                middleHeight += m_heightmapProcessor->getHeight(glm::vec3(x - 1, 0.0, z + 1));
                middleHeight += m_heightmapProcessor->getHeight(glm::vec3(x, 0.0, z + 1));
                middleHeight += m_heightmapProcessor->getHeight(glm::vec3(x + 1, 0.0, z + 1));
                height = middleHeight / 9.0f;
                height = floor(height + 0.5f);
            }
            modifiedHeights.push_back(std::make_tuple(x, z, height));
        }
    }
    m_heightmapProcessor->updateHeightmapData(modifiedHeights);*/
    
    ui32 offsetX = MAX_VALUE(minIndX, 0);
    ui32 offsetZ = MAX_VALUE(minIndZ, 0);
    ui32 subWidth = MIN_VALUE(maxIndX, m_heightmapProcessor->getSizeX() - 1) - offsetX;
    ui32 subHeight = MIN_VALUE(maxIndZ, m_heightmapProcessor->getSizeZ() - 1) - offsetZ;

    m_heightmapProcessor->updateHeightmap(offsetX, offsetZ,
                                          subWidth, subHeight);
}