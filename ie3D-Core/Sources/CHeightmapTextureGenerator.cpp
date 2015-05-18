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
#include "IRenderTechniqueAccessor.h"

glm::ivec2 CHeightmapTextureGenerator::kSplattingTextureMaskSize = glm::ivec2(64);

CHeightmapTextureGenerator::CHeightmapTextureGenerator(void)
{
    
}

CHeightmapTextureGenerator::~CHeightmapTextureGenerator(void)
{
    
}

void CHeightmapTextureGenerator::generateSplattingMasks(const std::shared_ptr<CHeightmapContainer>& container, const std::string& filename)
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
    
    f32 maxHeight = fabsf(container->getMaxHeight()) + fabsf(container->getMinHeight());
    
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
                    f32 height = CHeightmapAccessor::getHeight(container, glm::vec3(offset.x + verticesOffset.x, 0.0f, offset.z + verticesOffset.y)) + CHeightmapContainer::kDeep;
                    glm::vec3 normal = CHeightmapAccessor::getNormal(container, glm::vec3(offset.x + verticesOffset.x, 0.0f, offset.z + verticesOffset.y));
                    
                    f32 normalizedHeight = height / maxHeight;
                    ui8 redColor = normalizedHeight <= CHeightmapContainer::kLayerSection01 ? 255 : 0;
                    if(normalizedHeight > CHeightmapContainer::kLayerSection01 &&
                       normalizedHeight <= CHeightmapContainer::kLayerSection01 + CHeightmapContainer::kLayerSectionOffset)
                    {
                        f32 interpolation = (normalizedHeight - CHeightmapContainer::kLayerSection01) / CHeightmapContainer::kLayerSectionOffset;
                        redColor = glm::mix(255, 0, interpolation);
                    }
                    
                    ui8 greenColor = normalizedHeight > CHeightmapContainer::kLayerSection01 && normalizedHeight <= CHeightmapContainer::kLayerSection02 ? 255 : 0;
                    if(normalizedHeight < CHeightmapContainer::kLayerSection01 &&
                       normalizedHeight >= CHeightmapContainer::kLayerSection01 - CHeightmapContainer::kLayerSectionOffset)
                    {
                        f32 interpolation = (normalizedHeight - (CHeightmapContainer::kLayerSection01 - CHeightmapContainer::kLayerSectionOffset)) / CHeightmapContainer::kLayerSectionOffset;
                        greenColor = glm::mix(0, 255, interpolation);
                    }
                    else if(normalizedHeight > CHeightmapContainer::kLayerSection02 &&
                            normalizedHeight <= CHeightmapContainer::kLayerSection02 + CHeightmapContainer::kLayerSectionOffset)
                    {
                        f32 interpolation = (normalizedHeight - CHeightmapContainer::kLayerSection02) / CHeightmapContainer::kLayerSectionOffset;
                        greenColor = glm::mix(255, 0, interpolation);
                    }
                    
                    ui8 blueColor = normalizedHeight > CHeightmapContainer::kLayerSection02 ? 255 : 0;
                    if(normalizedHeight < CHeightmapContainer::kLayerSection02 &&
                       normalizedHeight >= CHeightmapContainer::kLayerSection02 - CHeightmapContainer::kLayerSectionOffset)
                    {
                        f32 interpolation = (normalizedHeight - (CHeightmapContainer::kLayerSection02 - CHeightmapContainer::kLayerSectionOffset)) / CHeightmapContainer::kLayerSectionOffset;
                        blueColor = glm::mix(0, 255, interpolation);
                    }
                    
                    f32 angle = glm::dot(glm::vec3(0.0f, 1.0f, 0.0f), normal);
                    angle = glm::degrees(acosf(angle));
                    assert(angle >= 0.0);
                    angle = MIN_VALUE(angle / 45.0f, 1.0f);
                    blueColor = MAX_VALUE(glm::mix(0, 255, angle), blueColor);
                    
                    pixels[index] = TO_RGB565(redColor, greenColor, blueColor);
                    
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

void CHeightmapTextureGenerator::generateSplattingTextures(ISharedRenderTechniqueAccessorRef renderTechniqueAccessor,
                                                           const std::shared_ptr<CHeightmapContainer>& container, const std::string& filename)
{
    
}
