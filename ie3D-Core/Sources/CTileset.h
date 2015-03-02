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
    
    CSharedTexture m_texture;
    std::array<std::bitset<k_MAX_BIT_IN_TILE> ,k_MAX_TILES_IN_SET> m_bitsets;
    std::array<glm::vec4, k_MAX_TILES_IN_SET> m_texcoords;
    
public:
    
    CTileset(CSharedTextureRef texture);
    ~CTileset(void);
};

#endif
