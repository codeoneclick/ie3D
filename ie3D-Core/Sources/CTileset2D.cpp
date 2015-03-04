//
//  CTileset2D.cpp
//  ie3D-Core
//
//  Created by sergey.sergeev on 3/2/15.
//  Copyright (c) 2015 Sergey Sergeev. All rights reserved.
//

#include "CTileset2D.h"
#include "CTexture.h"

CTileset2D::CTileset2D(CSharedTextureRef texture, ui32 bitsetTypesCount,
                   const glm::ivec2& mapSize) :
m_bitsetTypesCount(bitsetTypesCount),
m_texture(texture),
m_mapSize(mapSize)
{
    m_bitsets.resize(m_bitsetTypesCount);
    m_texcoords.resize(m_bitsetTypesCount);
    
    assert(texture->getWidth() > k_MAX_TILES_IN_SET);
    assert(texture->getWidth() % k_MAX_TILES_IN_SET == 0);
    assert(texture->getHeight() > m_bitsetTypesCount);
    assert(texture->getHeight() % m_bitsetTypesCount == 0);
    
    f32 texcoordStepU = texture->getWidth() / k_MAX_TILES_IN_SET;
    f32 texcoordStepV = texture->getHeight() / m_bitsetTypesCount;
    
    for(ui32 i = 0; i < m_bitsetTypesCount; ++i)
    {
        for(ui32 j = 0; j < k_MAX_TILES_IN_SET; ++j)
        {
            m_bitsets[i][j] = j;
            f32 u1 = static_cast<f32>((j * texcoordStepU) / texture->getWidth());
            f32 u2 = static_cast<f32>(((j + 1) * texcoordStepU) / texture->getWidth());
            f32 v1 = static_cast<f32>((i * texcoordStepV) / texture->getHeight());
            f32 v2 = static_cast<f32>(((i + 1) * texcoordStepV) / texture->getHeight());
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

CTileset2D::~CTileset2D(void)
{
    
}

void CTileset2D::paint(i32 x, i32 y, ui32 bitsetType)
{
    for(i32 i = x - 1; i <= x + 1; ++i)
    {
        for(i32 j = y - 1; j <= y + 1; ++j)
        {
            ui32 index = i + j * m_mapSize.x;
            if(i == x && j == y)
            {
                for(ui32 k = 0; k < k_MAX_BIT_IN_TILE; ++k)
                {
                    std::get<1>(m_map[index])[k] = 1;
                }
                std::get<2>(m_map[index]) = m_texcoords[0][std::get<1>(m_map[index]).to_ulong()];
                continue;
            }
            if(i == x && j != y)
            {
                std::get<1>(m_map[index])[j < y ? 0 : 2] = 1;
                std::get<1>(m_map[index])[j < y ? 1 : 3] = 1;
                std::get<2>(m_map[index]) = m_texcoords[0][std::get<1>(m_map[index]).to_ulong()];
                continue;
            }
            if(i != x && j == y)
            {
                std::get<1>(m_map[index])[i < x ? 1 : 0] = 1;
                std::get<1>(m_map[index])[i < x ? 3 : 2] = 1;
                std::get<2>(m_map[index]) = m_texcoords[0][std::get<1>(m_map[index]).to_ulong()];
                continue;
            }
            if(i != x && j < y)
            {
                std::get<1>(m_map[index])[i < x ? 1 : 0] = 1;
                std::get<2>(m_map[index]) = m_texcoords[0][std::get<1>(m_map[index]).to_ulong()];
                continue;
            }
            if(i < x && j != y)
            {
                std::get<1>(m_map[index])[j < y ? 1 : 3] = 1;
                std::get<2>(m_map[index]) = m_texcoords[0][std::get<1>(m_map[index]).to_ulong()];
                continue;
            }
            if(i > x && j > y)
            {
                std::get<1>(m_map[index])[2] = 1;
                std::get<2>(m_map[index]) = m_texcoords[0][std::get<1>(m_map[index]).to_ulong()];
                continue;
            }
        }
    }
}