//
//  CHeightmapTextureGenerator.cpp
//  ie3D-Core
//
//  Created by sergey.sergeev on 5/11/15.
//  Copyright (c) 2015 Sergey Sergeev. All rights reserved.
//

#include "CHeightmapTextureGenerator.h"
#include "CHeightmapContainer.h"
#include "CHeightmapLoader.h"
#include "CHeightmapAccessor.h"

glm::ivec2 CHeightmapTextureGenerator::kSplattingTextureMaskSize = glm::ivec2(64);

CHeightmapTextureGenerator::CHeightmapTextureGenerator(void)
{
    
}

CHeightmapTextureGenerator::~CHeightmapTextureGenerator(void)
{
    
}

void CHeightmapTextureGenerator::generate(const std::shared_ptr<CHeightmapContainer>& container, const std::string& filename)
{
    if(!CHeightmapLoader::isSplattingTextureMasksMMAPExist(filename))
    {
        CHeightmapTextureGenerator::createSplattingTextureMasks(container, filename);
    }
}

void CHeightmapTextureGenerator::createSplattingTextureMasks(const std::shared_ptr<CHeightmapContainer>& container, const std::string& filename)
{
    std::ofstream stream;
    stream.open(CHeightmapLoader::getSplattingTextureMasksMMAPFilename(filename), std::ios::binary | std::ios::out | std::ios::trunc);
    if(!stream.is_open())
    {
        assert(false);
    }
    
    glm::ivec2 verticesOffset(0);
    glm::vec2 step = glm::vec2(static_cast<f32>(container->getChunkSize().x) / static_cast<f32>(kSplattingTextureMaskSize.x) ,
                               static_cast<f32>(container->getChunkSize().y) / static_cast<f32>(kSplattingTextureMaskSize.y));
    
    glm::vec3 offset = glm::vec3(0.0f);
    
    ui16* pixels = new ui16[kSplattingTextureMaskSize.x * kSplattingTextureMaskSize.y];
    
    for(ui32 i = 0; i < container->getChunksNum().x; ++i)
    {
        verticesOffset.y = 0;
        for(ui32 j = 0; j < container->getChunksNum().y; ++j)
        {
            offset = glm::vec3(0.0f);
            for(ui32 x = 0; x < kSplattingTextureMaskSize.x; ++x)
            {
                offset.z = 0.0f;
                for(ui32 y = 0; y < kSplattingTextureMaskSize.y; ++y)
                {
                    ui32 index = x + y * kSplattingTextureMaskSize.x;
                    
                    pixels[index] = TO_RGB565(255, 0, 0);
                    f32 height = CHeightmapAccessor::getHeight(container, glm::vec3(offset.x + verticesOffset.x, 0.0f, offset.z + verticesOffset.y));
                    glm::vec3 normal = CHeightmapAccessor::getNormal(container, glm::vec3(offset.x + verticesOffset.x, 0.0f, offset.z + verticesOffset.y));
                    
                    f32 angle = glm::dot(glm::vec3(0.0f, 1.0f, 0.0f), normal);
                    angle = glm::degrees(acosf(angle));
                    assert(angle >= 0.0);
                    
                    if(height > 1.0)
                    {
                        pixels[index] = TO_RGB565(glm::mix(0, 255, 1.0f - MIN_VALUE(angle / 45.0f, 1.0f)), glm::mix(0, 255, MIN_VALUE(angle / 45.0f, 1.0f)), 0);
                    }
                    
                    if(height <= 1.0f && height > 0.0f)
                    {
                        pixels[index] = TO_RGB565(glm::mix(0, 255, height), 0, glm::mix(0, 255, 1.0f - height));
                    }
                    
                    if(height <= 0.0f)
                    {
                        pixels[index] = TO_RGB565(0, 0, 255);
                    }
                    
                    offset.z += step.y;
                }
                offset.x += step.x;
            }
            verticesOffset.y += container->getChunkSize().y - 1;
            
            i32 index = 0;
            for(ui32 x = 0; x < kSplattingTextureMaskSize.x; ++x)
            {
                for(ui32 y = 0; y < kSplattingTextureMaskSize.y; ++y)
                {
                    stream.write((char*)&pixels[index], sizeof(ui16));
                    index++;
                }
            }
        }
        verticesOffset.x += container->getChunkSize().x - 1;
    }
    stream.close();
    
    delete[] pixels;
    pixels = nullptr;
}
