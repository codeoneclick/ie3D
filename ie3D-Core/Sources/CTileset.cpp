//
//  CTileset.cpp
//  ie3D-Core
//
//  Created by sergey.sergeev on 3/2/15.
//  Copyright (c) 2015 Sergey Sergeev. All rights reserved.
//

#include "CTileset.h"
#include "CTexture.h"

CTileset::CTileset(CSharedTextureRef texture, ui32 textureSetsCount,
                   const glm::ivec2& mapSize) :
m_textureSetsCount(textureSetsCount),
m_texture(texture),
m_mapSize(mapSize)
{
    m_bitsets.resize(textureSetsCount);
    m_texcoords.resize(textureSetsCount);
    
    assert(texture->getWidth() > k_MAX_TILES_IN_SET);
    assert(texture->getWidth() % k_MAX_TILES_IN_SET == 0);
    assert(texture->getHeight() > textureSetsCount);
    assert(texture->getHeight() % textureSetsCount == 0);
    
    ui32 texcoordStepU = texture->getWidth() / k_MAX_TILES_IN_SET;
    ui32 texcoordStepV = texture->getHeight() / textureSetsCount;
    
    for(ui32 i = 0; i < m_textureSetsCount; ++i)
    {
        for(ui32 j = 0; j < k_MAX_TILES_IN_SET; ++j)
        {
            m_bitsets[i][j] = j;
            ui32 u1 = static_cast<ui32>((j * texcoordStepU) / texture->getWidth());
            ui32 u2 = static_cast<ui32>(((j + 1) * texcoordStepU) / texture->getWidth());
            ui32 v1 = static_cast<ui32>((i * texcoordStepV) / texture->getHeight());
            ui32 v2 = static_cast<ui32>(((i + 1) * texcoordStepV) / texture->getHeight());
            m_texcoords[i][j] = glm::vec4(u1, v1,
                                          u2, v2);
        }
    }
    
    m_map.resize(m_mapSize.x * m_mapSize.y);
    for(ui32 i = 0; i < m_mapSize.x; ++i)
    {
        for(ui32 j = 0; j < m_mapSize.y; ++j)
        {
            std::get<0>(m_map[i + j * m_mapSize.x]) = 0;
            std::get<1>(m_map[i + j * m_mapSize.x]) = m_bitsets[0][0];
            std::get<2>(m_map[i + j * m_mapSize.x]) = m_texcoords[0][0];
        }
    }
}

CTileset::~CTileset(void)
{
    
}