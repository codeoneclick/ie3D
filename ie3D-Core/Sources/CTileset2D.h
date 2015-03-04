//
//  CTileset2D.h
//  ie3D-Core
//
//  Created by sergey.sergeev on 3/2/15.
//  Copyright (c) 2015 Sergey Sergeev. All rights reserved.
//

#ifndef CTileset2D_h
#define CTileset2D_h

#include "HCommon.h"
#include "HEnums.h"
#include "HDeclaration.h"

#define k_MAX_TILES_IN_SET 16
#define k_MAX_BIT_IN_TILE 4

class CTileset2D
{
private:
    
protected:
    
    std::vector<std::array<std::bitset<k_MAX_BIT_IN_TILE>, k_MAX_TILES_IN_SET>> m_bitsets;
    std::vector<std::array<glm::vec4, k_MAX_TILES_IN_SET>> m_texcoords;
    
    ui32 m_bitsetTypesCount;
    CSharedTexture m_texture;
    
    glm::ivec2 m_mapSize;
    
public:
    
    CTileset2D(CSharedTextureRef texture, ui32 bitsetTypesCount,
             const glm::ivec2& mapSize);
    ~CTileset2D(void);
    
    std::vector<std::tuple<ui32, std::bitset<k_MAX_BIT_IN_TILE>, glm::vec4>> m_map;
    void paint(i32 x, i32 y, ui32 bitsetType);
};

#endif
