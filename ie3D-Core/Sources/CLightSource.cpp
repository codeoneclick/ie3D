//
//  CLightSource.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/29/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CLightSource.h"

CLightSource::CLightSource(void) :
m_position(glm::vec3(0.0))
{
    
}

CLightSource::~CLightSource(void)
{
    
}

void CLightSource::setPosition(const glm::vec3 &position)
{
    m_position = position;
}

glm::vec3 CLightSource::getPosition(void) const
{
    return m_position;
}