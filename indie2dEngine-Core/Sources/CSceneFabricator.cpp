//
//  CSceneFabricator.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/28/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CSceneFabricator.h"

CSceneFabricator::CSceneFabricator(void)
{
    
}

CSceneFabricator::~CSceneFabricator(void)
{
    m_lightsContainer.clear();
    m_camerasContainer.clear();
    m_gameObjectsContainer.clear();
}