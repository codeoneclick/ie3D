//
//  CHeightmapTextureGenerator.h
//  ie3D-Core
//
//  Created by sergey.sergeev on 5/11/15.
//  Copyright (c) 2015 Sergey Sergeev. All rights reserved.
//

#ifndef CHeightmapTextureGenerator_h
#define CHeightmapTextureGenerator_h

#include "HCommon.h"
#include "HEnums.h"
#include "HDeclaration.h"

class CHeightmapContainer;

class CHeightmapTextureGenerator
{
private:
    
    // create mask splatting textures.
    static void createSplattingMTextures(const std::shared_ptr<CHeightmapContainer>& container, const std::string& filename);
    
    // create diffuse splatting textures.
    static void createSplattingDTextures(ISharedRenderTechniqueAccessorRef renderTechniqueAccessor,
                                         const std::shared_ptr<CHeightmapContainer>& container, const std::string& filename,
                                         const std::array<CSharedTexture, E_SPLATTING_TEXTURE_MAX>& splattingDTextures);
    
    // create normal splatting textures.
    static void createSplattingNTextures(ISharedRenderTechniqueAccessorRef renderTechniqueAccessor,
                                         const std::shared_ptr<CHeightmapContainer>& container, const std::string& filename,
                                         const std::array<CSharedTexture, E_SPLATTING_TEXTURE_MAX>& splattingNTextures);
    
    static void generateSplattingTexture(ISharedRenderTechniqueAccessorRef renderTechniqueAccessor,
                                         const std::shared_ptr<CHeightmapContainer>& container,
                                         const std::array<CSharedTexture, E_SPLATTING_TEXTURE_MAX>& splattingDTextures,
                                         ui32 i, ui32 j, const std::function<void(ui8 *, ui32, E_LANDSCAPE_CHUNK_LOD LOD)>& callback);
    
    static CSharedMaterial getSplattingTexturesMaterial(const std::array<CSharedTexture, E_SPLATTING_TEXTURE_MAX>& splattingTextures);
    
protected:
    
public:
    
    static const f32 kMinSplattingTextureHeight;
    static const f32 kMaxSplattingTextureHeight;
    static const ui8 kSplattingTextureChannels;
    static const glm::ivec2 kSplattingTextureMaskSize;
    static const glm::ivec2 kSplattingTextureSize_LOD1;
    static const glm::ivec2 kSplattingTextureSize_LOD2;
    static const glm::ivec2 kSplattingTextureSize_LOD3;
    static const glm::ivec2 kSplattingTextureSize_LOD4;
    
    CHeightmapTextureGenerator(void);
    ~CHeightmapTextureGenerator(void);
    
    // generate mask splatting textures.
    static void generateSplattingMTextures(const std::shared_ptr<CHeightmapContainer>& container, const std::string& filename);
    static void generateSplattingMTexture(const std::shared_ptr<CHeightmapContainer>& container,
                                          ui32 i, ui32 j, const std::shared_ptr<std::ofstream> stream = nullptr);
    
    // generate diffuse splatting textures.
    static void generateSplattingDTextures(ISharedRenderTechniqueAccessorRef renderTechniqueAccessor,
                                           const std::shared_ptr<CHeightmapContainer>& container, const std::string& filename,
                                           const std::array<CSharedTexture, E_SPLATTING_TEXTURE_MAX>& splattingDTextures);
    static void generateSplattingDTexture(ISharedRenderTechniqueAccessorRef renderTechniqueAccessor,
                                          const std::shared_ptr<CHeightmapContainer>& container,
                                          const std::array<CSharedTexture, E_SPLATTING_TEXTURE_MAX>& splattingDTextures,
                                          ui32 i, ui32 j, const std::shared_ptr<std::ofstream> stream = nullptr);
    
    static void generateSplattingNTextures(ISharedRenderTechniqueAccessorRef renderTechniqueAccessor,
                                           const std::shared_ptr<CHeightmapContainer>& container, const std::string& filename,
                                           const std::array<CSharedTexture, E_SPLATTING_TEXTURE_MAX>& splattingNTextures);
    
    static void generateSplattingNTexture(ISharedRenderTechniqueAccessorRef renderTechniqueAccessor,
                                          const std::shared_ptr<CHeightmapContainer>& container,
                                          const std::array<CSharedTexture, E_SPLATTING_TEXTURE_MAX>& splattingNTextures,
                                          ui32 i, ui32 j, const std::shared_ptr<std::ofstream> stream = nullptr);
    
    static void generateDeepTexture(const std::shared_ptr<CHeightmapContainer>& container, bool create,
                                    ui32 offsetX, ui32 offsetY,
                                    ui32 subWidth, ui32 subHeight);
    
};

#endif
