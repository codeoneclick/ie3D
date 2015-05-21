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
#include "HDeclaration.h"

class CHeightmapContainer;

class CHeightmapTextureGenerator
{
private:
    
    static void createSplattingTextureMasks(const std::shared_ptr<CHeightmapContainer>& container, const std::string& filename);
    static void createSplattingTextures(ISharedRenderTechniqueAccessorRef renderTechniqueAccessor,
                                        const std::shared_ptr<CHeightmapContainer>& container, const std::string& filename,
                                        const std::array<CSharedTexture, 3>& splattingTextures);
    static void createSplattingNormalTextures(ISharedRenderTechniqueAccessorRef renderTechniqueAccessor,
                                              const std::shared_ptr<CHeightmapContainer>& container, const std::string& filename,
                                              const std::array<CSharedTexture, 3>& splattingTextures);
    
protected:
    
public:
    
    static const glm::ivec2 kSplattingTextureMaskSize;
    static const glm::ivec2 kSplattingTextureSize_LOD1;
    static const glm::ivec2 kSplattingTextureSize_LOD2;
    static const glm::ivec2 kSplattingTextureSize_LOD3;
    static const glm::ivec2 kSplattingTextureSize_LOD4;
    
    CHeightmapTextureGenerator(void);
    ~CHeightmapTextureGenerator(void);
    
    static void generateSplattingMasks(const std::shared_ptr<CHeightmapContainer>& container, const std::string& filename);
    static void generateSplattingTextures(ISharedRenderTechniqueAccessorRef renderTechniqueAccessor,
                                          const std::shared_ptr<CHeightmapContainer>& container, const std::string& filename,
                                          const std::array<CSharedTexture, 3>& splattingTextures);
    static void generateSplattingNormalTextures(ISharedRenderTechniqueAccessorRef renderTechniqueAccessor,
                                                const std::shared_ptr<CHeightmapContainer>& container, const std::string& filename,
                                                const std::array<CSharedTexture, 3>& splattingNormalTextures);
    
};

#endif
