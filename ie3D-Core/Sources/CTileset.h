//
//  CTileset.h
//  ie3D-Core
//
//  Created by sergey.sergeev on 3/2/15.
//  Copyright (c) 2015 Sergey Sergeev. All rights reserved.
//

#ifndef CTileset_h
#define CTileset_h

#include "HCommon.h"
#include "HEnums.h"
#include "HDeclaration.h"

#define k_MAX_TILES_IN_SET 16
#define k_MAX_BIT_IN_TILE 4

class CTileset
{
private:
    
protected:
    
    std::vector<std::array<std::bitset<k_MAX_BIT_IN_TILE>, k_MAX_TILES_IN_SET>> m_bitsets;
    std::vector<std::array<glm::vec4, k_MAX_TILES_IN_SET>> m_texcoords;
    
    ui32 m_textureSetsCount;
    CSharedTexture m_texture;
    
    glm::ivec2 m_mapSize;
    std::vector<std::tuple<ui32, std::bitset<k_MAX_BIT_IN_TILE>, glm::vec4>> m_map;
    
public:
    
    CTileset(CSharedTextureRef texture, ui32 textureSetsCount,
             const glm::ivec2& mapSize);
    ~CTileset(void);
};

#endif
