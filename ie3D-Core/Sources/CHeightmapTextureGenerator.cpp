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

const f32 CHeightmapTextureGenerator::kMinSplattingTextureHeight = 32.0f;
const f32 CHeightmapTextureGenerator::kMaxSplattingTextureHeight = 32.0f;
const ui8 CHeightmapTextureGenerator::kSplattingTextureChannels = 4;
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

std::once_flag g_createSplattingTexturesMaterialOnce;
CSharedMaterial CHeightmapTextureGenerator::getSplattingTexturesMaterial(const std::array<CSharedTexture,
                                                                         E_SPLATTING_TEXTURE_MAX>& splattingTextures)
{
    static CSharedMaterial material = std::make_shared<CMaterial>();
    std::call_once(g_createSplattingTexturesMaterialOnce, []() {
        
        CSharedShader shader = CShader::constructCustomShader("splattingTexture", ShaderSplattingTexture_vert, ShaderSplattingTexture_frag);
        assert(shader != nullptr);
        material->setShader(shader);
        
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
    });
    
    assert(splattingTextures[0]->isLoaded() && splattingTextures[0]->isCommited());
    material->setTexture(splattingTextures[0], E_SHADER_SAMPLER_01);
    assert(splattingTextures[1]->isLoaded() && splattingTextures[1]->isCommited());
    material->setTexture(splattingTextures[1], E_SHADER_SAMPLER_02);
    assert(splattingTextures[2]->isLoaded() && splattingTextures[2]->isCommited());
    material->setTexture(splattingTextures[2], E_SHADER_SAMPLER_03);
    
    return material;
}

std::once_flag g_createRenderTargetsOnce;
std::once_flag g_createSplattingMTexture;
void CHeightmapTextureGenerator::generateSplattingTexture(ISharedRenderTechniqueAccessorRef renderTechniqueAccessor,
                                                          const std::shared_ptr<CHeightmapContainer> &container,
                                                          const std::array<CSharedTexture, E_SPLATTING_TEXTURE_MAX> &splattingDTextures, ui32 i, ui32 j,
                                                          const std::function<void(ui8 *, ui32, E_LANDSCAPE_CHUNK_LOD LOD)>& callback)
{
    CSharedMaterial material = CHeightmapTextureGenerator::getSplattingTexturesMaterial(splattingDTextures);
    CSharedQuad quad = std::make_shared<CQuad>();
    
    static std::array<CSharedRenderTarget, E_LANDSCAPE_CHUNK_LOD_MAX> renderTargets;
    std::call_once(g_createRenderTargetsOnce, [container, renderTechniqueAccessor]() {
        for(ui32 i = 0; i < E_LANDSCAPE_CHUNK_LOD_MAX; ++i)
        {
            glm::ivec2 sizeTexture = container->getTexturesLODSize(static_cast<E_LANDSCAPE_CHUNK_LOD>(i));
            renderTargets[i] = std::make_shared<CRenderTarget>(renderTechniqueAccessor->getGraphicsContext(), GL_RGBA,
                                                               sizeTexture.x, sizeTexture.y);
        }
    });
    
    glm::ivec2 sizeMTexture = glm::ivec2(sqrt(container->getSplattingMTexturesMmap(0)->getSize()));
    static CSharedTexture splattingMTexture = nullptr;
    std::call_once(g_createSplattingMTexture, [container, material, sizeMTexture]() {
        
        ui32 textureId;
        ieGenTextures(1, &textureId);
        
        splattingMTexture = CTexture::constructCustomTexture("splatting.MTexture", textureId,
                                                             sizeMTexture.x, sizeMTexture.y);
        
        splattingMTexture->setWrapMode(GL_CLAMP_TO_EDGE);
        splattingMTexture->setMagFilter(GL_LINEAR);
        splattingMTexture->setMinFilter(GL_LINEAR);
        material->setTexture(splattingMTexture, E_SHADER_SAMPLER_04);
    });
    
    for(ui32 LOD = 0; LOD < E_LANDSCAPE_CHUNK_LOD_MAX; ++LOD)
    {
        glm::ivec2 sizeDTexture = container->getTexturesLODSize(static_cast<E_LANDSCAPE_CHUNK_LOD>(LOD));
        ui32 size = static_cast<ui32>(sizeDTexture.x) * static_cast<ui32>(sizeDTexture.y) * kSplattingTextureChannels;
        ui8 *data = new ui8[size];
        
        ui32 index = i + j * container->getChunksNum().x;
        splattingMTexture->bind();
        
        ieTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                     sizeMTexture.x, sizeMTexture.y,
                     0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5,
                     container->getSplattingMTexturesMmap(index)->getPointer());
        
        renderTargets[LOD]->begin();
        renderTargets[LOD]->clear();
        
        material->bind();
        assert(material->getShader()->getAttributes().at(E_SHADER_ATTRIBUTE_POSITION) >= 0);
        assert(material->getShader()->getAttributes().at(E_SHADER_ATTRIBUTE_TEXCOORD) >= 0);
        quad->bind(material->getShader()->getGUID(), material->getShader()->getAttributes());
        
        quad->draw();
        
        quad->unbind(material->getShader()->getGUID(), material->getShader()->getAttributes());
        material->unbind();
        
        renderTargets[LOD]->end(data);
        
        assert(callback);
        callback(data, size, static_cast<E_LANDSCAPE_CHUNK_LOD>(LOD));
        delete [] data;
    }
}

void CHeightmapTextureGenerator::generateSplattingMTextures(const std::shared_ptr<CHeightmapContainer>& container, const std::string& filename)
{
    if(!CHeightmapLoader::isSplattingTexturesM_MMapExist(filename))
    {
        CHeightmapTextureGenerator::createSplattingMTextures(container, filename);
    }
}

void CHeightmapTextureGenerator::createSplattingMTextures(const std::shared_ptr<CHeightmapContainer>& container, const std::string& filename)
{
    std::shared_ptr<std::ofstream> stream = std::make_shared<std::ofstream>();
    stream->open(CHeightmapLoader::getSplattingTexturesM_MMapFilename(filename), std::ios::binary | std::ios::out | std::ios::trunc);
    if(!stream->is_open())
    {
        assert(false);
    }
    
    for(ui32 i = 0; i < container->getChunksNum().x; ++i)
    {
        for(ui32 j = 0; j < container->getChunksNum().y; ++j)
        {
            CHeightmapTextureGenerator::generateSplattingMTexture(container, i, j, stream);
        }
    }
    stream->close();
}

void CHeightmapTextureGenerator::generateSplattingMTexture(const std::shared_ptr<CHeightmapContainer> &container,
                                                           ui32 i, ui32 j, const std::shared_ptr<std::ofstream> stream)
{
    glm::ivec2 verticesOffset( i * (container->getChunkSize().x - 1), j * (container->getChunkSize().y - 1));
    glm::vec2 step = glm::vec2(static_cast<f32>(container->getChunkSize().x) / static_cast<f32>(kSplattingTextureMaskSize.x) ,
                               static_cast<f32>(container->getChunkSize().y) / static_cast<f32>(kSplattingTextureMaskSize.y));
    
    glm::vec3 offset = glm::vec3(0.0f);
    f32 maxHeight = kMaxSplattingTextureHeight;
    
    ui16* pixels = new ui16[kSplattingTextureMaskSize.x * kSplattingTextureMaskSize.y];
    
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
            ui8 red = normalizedHeight <= CHeightmapContainer::kLayerSection01 ? 255 : 0;
            if(normalizedHeight > CHeightmapContainer::kLayerSection01 &&
               normalizedHeight <= CHeightmapContainer::kLayerSection01 + CHeightmapContainer::kLayerSectionOffset)
            {
                f32 interpolation = (normalizedHeight - CHeightmapContainer::kLayerSection01) / CHeightmapContainer::kLayerSectionOffset;
                red = glm::mix(255, 0, interpolation);
            }
            
            ui8 green = normalizedHeight > CHeightmapContainer::kLayerSection01 && normalizedHeight <= CHeightmapContainer::kLayerSection02 ? 255 : 0;
            if(normalizedHeight < CHeightmapContainer::kLayerSection01 &&
               normalizedHeight >= CHeightmapContainer::kLayerSection01 - CHeightmapContainer::kLayerSectionOffset)
            {
                f32 interpolation = (normalizedHeight - (CHeightmapContainer::kLayerSection01 - CHeightmapContainer::kLayerSectionOffset)) /
                CHeightmapContainer::kLayerSectionOffset;
                green = glm::mix(0, 255, interpolation);
            }
            else if(normalizedHeight > CHeightmapContainer::kLayerSection02 &&
                    normalizedHeight <= CHeightmapContainer::kLayerSection02 + CHeightmapContainer::kLayerSectionOffset)
            {
                f32 interpolation = (normalizedHeight - CHeightmapContainer::kLayerSection02) / CHeightmapContainer::kLayerSectionOffset;
                green = glm::mix(255, 0, interpolation);
            }
            
            ui8 blue = normalizedHeight > CHeightmapContainer::kLayerSection02 ? 255 : 0;
            if(normalizedHeight < CHeightmapContainer::kLayerSection02 &&
               normalizedHeight >= CHeightmapContainer::kLayerSection02 - CHeightmapContainer::kLayerSectionOffset)
            {
                f32 interpolation = (normalizedHeight - (CHeightmapContainer::kLayerSection02 - CHeightmapContainer::kLayerSectionOffset)) /
                CHeightmapContainer::kLayerSectionOffset;
                blue = glm::mix(0, 255, interpolation);
            }
            
            f32 angle = glm::dot(glm::vec3(0.0f, 1.0f, 0.0f), normal);
            angle = glm::degrees(acosf(angle));
            assert(angle >= 0.0);
            angle = MIN_VALUE(angle / 45.0f, 1.0f);
            blue = MAX_VALUE(glm::mix(0, 255, angle), blue);
            
            pixels[index] = TO_RGB565(red, green, blue);
            
            offset.z += step.y;
        }
        offset.x += step.x;
    }
    
    if(stream && stream->is_open())
    {
        stream->write((char*)&pixels[0], sizeof(ui16) * kSplattingTextureMaskSize.x * kSplattingTextureMaskSize.y);
    }
    else
    {
        ui32 index = i + j * container->getChunksNum().x;
        assert(container->getSplattingMTexturesMmap(index)->getPointer());
        memcpy(container->getSplattingMTexturesMmap(index)->getPointer(), pixels, sizeof(ui16) * kSplattingTextureMaskSize.x * kSplattingTextureMaskSize.y);
    }
    
    delete[] pixels;
    pixels = nullptr;
}

void CHeightmapTextureGenerator::generateSplattingDTextures(ISharedRenderTechniqueAccessorRef renderTechniqueAccessor,
                                                            const std::shared_ptr<CHeightmapContainer>& container, const std::string& filename,
                                                            const std::array<CSharedTexture, E_SPLATTING_TEXTURE_MAX>& splattingDTextures)
{
    if(!CHeightmapLoader::isSplattingTexturesD_MMapExist(filename))
    {
        CHeightmapTextureGenerator::createSplattingDTextures(renderTechniqueAccessor, container, filename, splattingDTextures);
    }
}

void CHeightmapTextureGenerator::createSplattingDTextures(ISharedRenderTechniqueAccessorRef renderTechniqueAccessor,
                                                          const std::shared_ptr<CHeightmapContainer>& container, const std::string& filename,
                                                          const std::array<CSharedTexture, E_SPLATTING_TEXTURE_MAX>& splattingDTextures)
{
    renderTechniqueAccessor->getGraphicsContext()->beginBackgroundContext();
    
    CSharedMaterial material = CHeightmapTextureGenerator::getSplattingTexturesMaterial(splattingDTextures);
    CSharedQuad quad = std::make_shared<CQuad>();
    
    std::shared_ptr<std::ofstream> stream = std::make_shared<std::ofstream>();
    stream->open(CHeightmapLoader::getSplattingTexturesD_MMapFilename(filename), std::ios::binary | std::ios::out | std::ios::trunc);
    if(!stream->is_open())
    {
        assert(false);
    }

    for(ui32 i = 0; i < container->getChunksNum().x; ++i)
    {
        for(ui32 j = 0; j < container->getChunksNum().y; ++j)
        {
            CHeightmapTextureGenerator::generateSplattingDTexture(renderTechniqueAccessor, container, splattingDTextures, i, j, stream);
        }
    }

    renderTechniqueAccessor->getGraphicsContext()->endBackgroundContext();
    stream->close();
}

void CHeightmapTextureGenerator::generateSplattingDTexture(ISharedRenderTechniqueAccessorRef renderTechniqueAccessor,
                                                           const std::shared_ptr<CHeightmapContainer>& container,
                                                           const std::array<CSharedTexture, E_SPLATTING_TEXTURE_MAX>& splattingDTextures,
                                                           ui32 i, ui32 j, const std::shared_ptr<std::ofstream> stream)
{
    ui32 index = i + j * container->getChunksNum().x;
    CHeightmapTextureGenerator::generateSplattingTexture(renderTechniqueAccessor, container, splattingDTextures, i, j, [stream, container, index](ui8 *data, ui32 size, E_LANDSCAPE_CHUNK_LOD LOD) {
        if(stream)
        {
            stream->write((char *)data, size * sizeof(ui8));
        }
        else
        {
            assert(container->getSplattingDTexturesMmap(index, LOD)->getPointer());
            memcpy(container->getSplattingDTexturesMmap(index, LOD)->getPointer(), data, size * sizeof(ui8));
        }
    });
}

void CHeightmapTextureGenerator::generateSplattingNTextures(ISharedRenderTechniqueAccessorRef renderTechniqueAccessor,
                                                            const std::shared_ptr<CHeightmapContainer>& container, const std::string& filename,
                                                            const std::array<CSharedTexture, E_SPLATTING_TEXTURE_MAX>& splattingNTextures)
{
    if(!CHeightmapLoader::isSplattingTexturesN_MMapExist(filename))
    {
        CHeightmapTextureGenerator::createSplattingNTextures(renderTechniqueAccessor, container, filename, splattingNTextures);
    }
}

void CHeightmapTextureGenerator::createSplattingNTextures(ISharedRenderTechniqueAccessorRef renderTechniqueAccessor,
                                                          const std::shared_ptr<CHeightmapContainer>& container, const std::string& filename,
                                                          const std::array<CSharedTexture, E_SPLATTING_TEXTURE_MAX>& splattingNTextures)
{
    renderTechniqueAccessor->getGraphicsContext()->beginBackgroundContext();
    
    CSharedMaterial material = CHeightmapTextureGenerator::getSplattingTexturesMaterial(splattingNTextures);
    CSharedQuad quad = std::make_shared<CQuad>();
    
    std::shared_ptr<std::ofstream> stream = std::make_shared<std::ofstream>();
    stream->open(CHeightmapLoader::getSplattingTexturesN_MMapFilename(filename), std::ios::binary | std::ios::out | std::ios::trunc);
    if(!stream->is_open())
    {
        assert(false);
    }
    
    for(ui32 i = 0; i < container->getChunksNum().x; ++i)
    {
        for(ui32 j = 0; j < container->getChunksNum().y; ++j)
        {
            CHeightmapTextureGenerator::generateSplattingNTexture(renderTechniqueAccessor, container, splattingNTextures, i, j, stream);
        }
    }
    
    renderTechniqueAccessor->getGraphicsContext()->endBackgroundContext();
    stream->close();
}

void CHeightmapTextureGenerator::generateSplattingNTexture(ISharedRenderTechniqueAccessorRef renderTechniqueAccessor,
                                                           const std::shared_ptr<CHeightmapContainer>& container,
                                                           const std::array<CSharedTexture, E_SPLATTING_TEXTURE_MAX>& splattingNTextures,
                                                           ui32 i, ui32 j, const std::shared_ptr<std::ofstream> stream)
{
    ui32 index = i + j * container->getChunksNum().x;
    CHeightmapTextureGenerator::generateSplattingTexture(renderTechniqueAccessor, container, splattingNTextures, i, j, [stream, container, index](ui8 *data, ui32 size, E_LANDSCAPE_CHUNK_LOD LOD) {
        if(stream)
        {
            stream->write((char *)data, size * sizeof(ui8));
        }
        else
        {
            assert(container->getSplattingNTexturesMmap(index, LOD)->getPointer());
            memcpy(container->getSplattingNTexturesMmap(index, LOD)->getPointer(), data, size * sizeof(ui8));
        }
    });
}

void CHeightmapTextureGenerator::generateDeepTexture(const std::shared_ptr<CHeightmapContainer>& container, bool create,
                                                     ui32 offsetX, ui32 offsetY,
                                                     ui32 subWidth, ui32 subHeight)
{
    assert(container->getDeepTexture());
    container->getDeepTexture()->bind();
    
    ui8* data = nullptr;
    if(create)
    {
        data = new ui8[container->getMainSize().x * container->getMainSize().y];
        f32 maxDeep = kMaxSplattingTextureHeight;
        for(int i = 0; i < container->getMainSize().x; i++)
        {
            for(int j = 0; j < container->getMainSize().y; j++)
            {
                f32 height = CHeightmapAccessor::getHeight(container, glm::vec3(i, 0.0f, j));
                height = height <= 0.0f ? height : 0.0f;
                height /= maxDeep;
                height = std::max(0.0f, std::min((height + 1.0f) / 2.0f, 1.0f));
                ui8 color = static_cast<ui8>(height * 255);
                data[i + j * container->getMainSize().x] = color;
            }
        }
        
        ieTexImage2D(GL_TEXTURE_2D, 0,
#if defined(__OPENGL_30__)
                     GL_RED,
#else
                     GL_ALPHA,
#endif
                     container->getMainSize().x,
                     container->getMainSize().y,
                     0,
#if defined(__OPENGL_30__)
                     GL_RED,
#else
                     GL_ALPHA,
#endif
                     GL_UNSIGNED_BYTE, data);
    }
    else
    {
        assert(offsetX >= 0);
        assert(offsetX + subWidth < container->getDeepTexture()->getWidth());
        assert(offsetY >= 0);
        assert(offsetY + subHeight < container->getDeepTexture()->getHeight());
        
        f32 maxDeep = kMaxSplattingTextureHeight;
        
        data = new ui8[subWidth * subHeight];
        for(int i = 0; i < subWidth; i++)
        {
            for(int j = 0; j < subHeight; j++)
            {
                f32 height = CHeightmapAccessor::getHeight(container, glm::vec3(i + offsetX , 0.0, j + offsetY));
                height = height <= 0.0f ? height : 0.0f;
                height /= maxDeep;
                height = std::max(0.0f, std::min((height + 1.0f) / 2.0f, 1.0f));
                ui8 color = static_cast<ui8>(height * 255);
                data[i + j * subWidth] = color;
            }
        }
        glTexSubImage2D(GL_TEXTURE_2D, 0,
                        offsetX, offsetY,
                        subWidth, subHeight,
#if defined(__OPENGL_30__)
                        GL_RED,
#else
                        GL_ALPHA,
#endif
                        GL_UNSIGNED_BYTE, data);
    }
    delete[] data;
}

