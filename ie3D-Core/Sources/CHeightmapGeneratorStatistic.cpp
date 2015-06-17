//
//  CHeightmapGeneratorStatistic.cpp
//  ie3D-Core
//
//  Created by sergey.sergeev on 6/17/15.
//  Copyright (c) 2015 Sergey Sergeev. All rights reserved.
//

#include "CHeightmapGeneratorStatistic.h"

CHeightmapGeneratorStatistic::CHeightmapGeneratorStatistic(void) :
m_callback(nullptr)
{
    
}

CHeightmapGeneratorStatistic::~CHeightmapGeneratorStatistic(void)
{
    m_callback = nullptr;
}

void CHeightmapGeneratorStatistic::setCallback(const T_STATISTIC_CALLBACK& callback)
{
    m_callback = callback;
}

CHeightmapGeneratorStatistic::T_STATISTIC_CALLBACK CHeightmapGeneratorStatistic::getCallback(void) const
{
    return m_callback;
}

void CHeightmapGeneratorStatistic::update(const std::string& operationName, E_HEIGHTMAP_GENERATION_STATUS status, const std::string& message)
{
    if(m_callback)
    {
        m_callback(operationName, status, message);
    }
}