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

class CHeightmapContainer;

class CHeightmapTextureGenerator
{
private:
    
    static void createSplattingTextureMasks(const std::shared_ptr<CHeightmapContainer>& container, const std::string& filename);
    static void createSplattingTextures(const std::shared_ptr<CHeightmapContainer>& container, const std::string& filename);
    
protected:
    
public:
    
    static glm::ivec2 kSplattingTextureMaskSize;
    
    CHeightmapTextureGenerator(void);
    ~CHeightmapTextureGenerator(void);
    
    static void generate(const std::shared_ptr<CHeightmapContainer>& container, const std::string& filename);

};

#endif
