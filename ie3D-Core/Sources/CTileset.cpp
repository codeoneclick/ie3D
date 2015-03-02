//
//  CTileset.cpp
//  ie3D-Core
//
//  Created by sergey.sergeev on 3/2/15.
//  Copyright (c) 2015 Sergey Sergeev. All rights reserved.
//

#include "CTileset.h"

CTileset::CTileset(void)
{
    for(ui32 i = 0; i < k_MAX_TILES_IN_SET; ++i)
    {
        m_bitsets[i] = i;
        std::cout<<m_bitsets[i][0]<<m_bitsets[i][1]<<m_bitsets[i][2]<<m_bitsets[i][3]<<std::endl;
    }
}

CTileset::~CTileset(void)
{
    
}