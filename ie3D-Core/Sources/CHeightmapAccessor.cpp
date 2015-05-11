//
//  CHeightmapAccessor.cpp
//  ie3D-Core
//
//  Created by sergey.sergeev on 5/11/15.
//  Copyright (c) 2015 Sergey Sergeev. All rights reserved.
//

#include "CHeightmapAccessor.h"
#include "CHeightmapContainer.h"

static ui32 g_heightmapGUID = 0;

CHeightmapAccessor::CHeightmapAccessor(void) :
m_container(std::make_shared<CHeightmapContainer>())
{
    
}

CHeightmapAccessor::~CHeightmapAccessor(void)
{
    
}

void CHeightmapAccessor::create(const std::string& filename, const std::function<void(void)>& callback)
{
    
}