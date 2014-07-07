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
            
            i32 delimiter = 1;
            for(i32 i = x - 1; i <= x + 1; ++i)
            {
                for(i32 j = z - 1; j <= z + 1; ++j)
                {
                    if(i > 0 && j > 0 &&
                       i < m_heightmapProcessor->getSizeX() && j < m_heightmapProcessor->getSizeZ())
                    {
                        height += m_heightmapProcessor->getHeight(glm::vec3(i, 0.0, j));
                        delimiter++;
                    }
                }
            }
            height /= static_cast<f32>(delimiter);

            modifiedHeights.push_back(std::make_tuple(x, z, height));
        }
	}
    m_heightmapProcessor->updateHeightmapData(modifiedHeights);
    
    ui32 offsetX = MAX_VALUE(minIndX, 0);
    ui32 offsetZ = MAX_VALUE(minIndZ, 0);
    ui32 subWidth = MIN_VALUE(maxIndX, m_heightmapProcessor->getSizeX() - 1) - offsetX;
    ui32 subHeight = MIN_VALUE(maxIndZ, m_heightmapProcessor->getSizeZ() - 1) - offsetZ;

    m_heightmapProcessor->updateHeightmap(offsetX, offsetZ,
                                          subWidth, subHeight);
}