//
//  CTileset.cpp
//  ie3D-Core
//
//  Created by sergey.sergeev on 3/2/15.
//  Copyright (c) 2015 Sergey Sergeev. All rights reserved.
//

#include "CTileset.h"
#include "CTexture.h"

CTileset::CTileset(CSharedTextureRef texture) :
m_texture(texture)
{
    assert(texture->getWidth() > 16);
    assert(texture->getWidth() % 16 == 0);
    
    ui32 texcoordStep = texture->getWidth() / 16;
    for(ui32 i = 0; i < k_MAX_TILES_IN_SET; ++i)
    {
        m_bitsets[i] = i;
        ui32 u1 = static_cast<ui32>((i * texcoordStep) / texture->getWidth());
        ui32 u2 = static_cast<ui32>(((i + 1) * texcoordStep) / texture->getWidth());
        m_texcoords[i] = glm::vec4(u1, 0,
                                   u2, 1);
    }
}

CTileset::~CTileset(void)
{
    
}