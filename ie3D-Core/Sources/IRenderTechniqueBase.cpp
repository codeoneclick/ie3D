//
//  IRenderTechniqueBase.cpp
//  ie3D-Core
//
//  Created by sergey.sergeev on 7/2/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#include "IRenderTechniqueBase.h"

IRenderTechniqueBase::IRenderTechniqueBase(ui32 frameWidth, ui32 frameHeight, const std::string& name, ui32 index) :
m_frameWidth(frameWidth),
m_frameHeight(frameHeight),
m_name(name),
m_index(index),
m_clearColor(glm::vec4(0.5, 0.5, 0.5, 1.0))
{
    
}

IRenderTechniqueBase::~IRenderTechniqueBase(void)
{
    
}

ui32 IRenderTechniqueBase::getIndex(void) const
{
    return m_index;
}

ui32 IRenderTechniqueBase::getFrameWidth(void) const
{
    assert(m_frameWidth != 0);
    return m_frameWidth;
}

ui32 IRenderTechniqueBase::getFrameHeight(void) const
{
    assert(m_frameHeight != 0);
    return m_frameHeight;
}

std::string IRenderTechniqueBase::getName(void) const
{
    return m_name;
}

void IRenderTechniqueBase::setClearColor(const glm::vec4& clearColor)
{
    m_clearColor = clearColor;
}