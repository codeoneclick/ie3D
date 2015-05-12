//
//  CHeightmapAccessor.cpp
//  ie3D-Core
//
//  Created by sergey.sergeev on 5/11/15.
//  Copyright (c) 2015 Sergey Sergeev. All rights reserved.
//

#include "CHeightmapAccessor.h"
#include "CHeightmapContainer.h"
#include "CHeightmapLoader.h"
#include "CHeightmapGeometryGenerator.h"

CHeightmapAccessor::CHeightmapAccessor(void) :
m_container(std::make_shared<CHeightmapContainer>())
{
    
}

CHeightmapAccessor::~CHeightmapAccessor(void)
{
    
}

void CHeightmapAccessor::create(const std::string& filename, const std::function<void(void)>& callback)
{
    std::tuple<glm::ivec2, std::vector<f32>> heights = CHeightmapLoader::getHeights(filename);
    CHeightmapGeometryGenerator::generate(m_container, filename, std::get<0>(heights), std::get<1>(heights),
                                          callback);
}