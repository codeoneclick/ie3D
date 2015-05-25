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
#include "CTexture.h"
#include "CMaterial.h"
#include "CShader.h"
#include "CRenderTarget.h"
#include "CQuad.h"
#include "IGraphicsContext.h"

const glm::ivec2 CHeightmapTextureGenerator::kSplattingTextureMaskSize = glm::ivec2(64);
const glm::ivec2 CHeightmapTextureGenerator::kSplattingTextureSize_LOD1 = glm::ivec2(1024);
const glm::ivec2 CHeightmapTextureGenerator::kSplattingTextureSize_LOD2 = glm::ivec2(512);
const glm::ivec2 CHeightmapTextureGenerator::kSplattingTextureSize_LOD3 = glm::ivec2(128);
const glm::ivec2 CHeightmapTextureGenerator::kSplattingTextureSize_LOD4 = glm::ivec2(64);

CHeightmapTextureGenerator::CHeightmapTextureGenerator(void)
{
    
}

CHeightmapTextureGenerator::~CHeightmapTextureGenerator(void)
{
    
}

void CHeightmapTextureGenerator::generateSplattingMasks(const std::shared_ptr<CHeightmapContainer>& container, const std::string& filename)
{
    if(!CHeightmapLoader::isSplattingTextureM_MMapExist(filename))
    {
        CHeightmapTextureGenerator::createSplattingTextureMasks(container, filename);
    }
}

void CHeightmapTextureGenerator::createSplattingTextureMasks(const std::shared_ptr<CHeightmapContainer>& container, const std::string& filename)
{
    std::ofstream stream;
    stream.open(CHeightmapLoader::getSplattingTextureM_MMapFilename(filename), std::ios::binary | std::ios::out | std::ios::trunc);
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
                                                           const std::shared_ptr<CHeightmapContainer>& container, const std::string& filename,
                                                           const std::array<CSharedTexture, 3>& splattingTextures)
{
    if(!CHeightmapLoader::isSplattingTextures_MMapExist(filename))
    {
        CHeightmapTextureGenerator::createSplattingTextures(renderTechniqueAccessor, container, filename, splattingTextures);
    }
}

void CHeightmapTextureGenerator::createSplattingTextures(ISharedRenderTechniqueAccessorRef renderTechniqueAccessor,
                                                         const std::shared_ptr<CHeightmapContainer>& container, const std::string& filename,
                                                         const std::array<CSharedTexture, 3>& splattingTextures)
{
    renderTechniqueAccessor->getGraphicsContext()->beginBackgroundContext();
    
    CSharedMaterial material = std::make_shared<CMaterial>();
    CSharedShader shader = CShader::constructCustomShader("splattingTexture", ShaderSplattingTexture_vert, ShaderSplattingTexture_frag);
    assert(shader != nullptr);
    CSharedQuad quad = std::make_shared<CQuad>();
    material->setShader(shader);
    assert(splattingTextures[0]->isLoaded() && splattingTextures[0]->isCommited());
    material->setTexture(splattingTextures[0], E_SHADER_SAMPLER_01);
    assert(splattingTextures[1]->isLoaded() && splattingTextures[1]->isCommited());
    material->setTexture(splattingTextures[1], E_SHADER_SAMPLER_02);
    assert(splattingTextures[2]->isLoaded() && splattingTextures[2]->isCommited());
    material->setTexture(splattingTextures[2], E_SHADER_SAMPLER_03);
    
    material->setCulling(false);
    material->setCullingMode(GL_BACK);
    
    material->setBlending(false);
    material->setBlendingFunctionSource(GL_SRC_ALPHA);
    material->setBlendingFunctionDestination(GL_ONE);
    
    material->setDepthTest(false);
    material->setDepthMask(true);
    
    material->setClipping(false);
    material->setClippingPlane(glm::vec4(0.0, 0.0, 0.0, 0.0));
    
    material->setReflecting(false);
    material->setShadowing(false);
    material->setDebugging(false);
    
    std::ofstream stream;
    stream.open(CHeightmapLoader::getSplattingTextures_MMapFilename(filename), std::ios::binary | std::ios::out | std::ios::trunc);
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
                glm::ivec2 sizeTexture = container->getTexturesLODSize(static_cast<E_LANDSCAPE_CHUNK_LOD>(k));
                ui32 rawdataSize = static_cast<ui32>(sizeTexture.x) * static_cast<ui32>(sizeTexture.y) * 4;
                ui8 *rawdata = new ui8[rawdataSize];
                
                std::ostringstream stringstream;
                stringstream<<"texture_"<<i<<"_"<<j<<"_"<<k<<"_"<<CHeightmapLoader::g_heightmapGUID<<std::endl;
                
                ui32 textureId;
                ieGenTextures(1, &textureId);
                
                ui32 index = i + j * container->getChunksNum().x;
                
                glm::ivec2 size = glm::ivec2(sqrt(container->getSplattingTextureMaskMmap(index)->getSize()));
                CSharedTexture texture = CTexture::constructCustomTexture(stringstream.str(), textureId,
                                                                          size.x, size.y);
                texture->setWrapMode(GL_CLAMP_TO_EDGE);
                texture->setMagFilter(GL_LINEAR);
                texture->setMinFilter(GL_LINEAR);
                
                texture->bind();
                
                ieTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                             size.x, size.y,
                             0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, container->getSplattingTextureMaskMmap(index)->getPointer());
                
                material->setTexture(texture, E_SHADER_SAMPLER_04);
                
                CSharedRenderTarget renderTarget = std::make_shared<CRenderTarget>(renderTechniqueAccessor->getGraphicsContext(), GL_RGBA,
                                                                                   sizeTexture.x, sizeTexture.y);
                
                renderTarget->begin();
                renderTarget->clear();
                
                material->bind();
                assert(material->getShader()->getAttributes().at(E_SHADER_ATTRIBUTE_POSITION) >= 0);
                assert(material->getShader()->getAttributes().at(E_SHADER_ATTRIBUTE_TEXCOORD) >= 0);
                quad->bind(material->getShader()->getGUID(), material->getShader()->getAttributes());
                
                quad->draw();
                
                quad->unbind(material->getShader()->getGUID(), material->getShader()->getAttributes());
                material->unbind();
                
                glReadPixels(0, 0, sizeTexture.x, sizeTexture.y,
                             GL_RGBA, GL_UNSIGNED_BYTE, rawdata);
                stream.write((char *)rawdata, rawdataSize * sizeof(ui8));
                
                renderTarget->end();
                
                delete [] rawdata;
            }
        }
    }
    
    renderTechniqueAccessor->getGraphicsContext()->endBackgroundContext();
    stream.close();
}

void CHeightmapTextureGenerator::createSplattingNTextures(ISharedRenderTechniqueAccessorRef renderTechniqueAccessor,
                                                          const std::shared_ptr<CHeightmapContainer>& container, const std::string& filename,
                                                          const std::array<CSharedTexture, 3>& splattingTextures)
{
    
}

void CHeightmapTextureGenerator::generateSplattingNTextures(ISharedRenderTechniqueAccessorRef renderTechniqueAccessor,
                                                            const std::shared_ptr<CHeightmapContainer>& container, const std::string& filename,
                                                            const std::array<CSharedTexture, 3>& splattingNormalTextures)
{
    
}
