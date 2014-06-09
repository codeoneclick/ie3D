//
//  CMEConfigurationGameObjects.cpp
//  ie3D-Demo
//
//  Created by Sergey Sergeev on 6/7/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#include "CMEConfigurationGameObjects.h"

CMEConfigurationSelectionArea::CMEConfigurationSelectionArea(void) :
CConfigurationGameObject(E_CONFIGURATION_CLASS_CUSTOM),
kSelectionAreaMainNode("selection_area"),
kSelectionAreaSizeXAttribute("size_x"),
kSelectionAreaSizeYAttribute("size_y")
{
    
}

CMEConfigurationSelectionArea::~CMEConfigurationSelectionArea(void)
{
    
}

glm::vec2 CMEConfigurationSelectionArea::getSize(void) const
{
    const auto& iteratorX = m_attributes.find(kSelectionAreaMainNode + ":" +
                                              kSelectionAreaSizeXAttribute);
    assert(iteratorX != m_attributes.end());
    assert(iteratorX->second.size() != 0);
    f32 x = iteratorX->second[0]->getScalar<f32>();
    
    const auto& iteratorY = m_attributes.find(kSelectionAreaMainNode + ":" +
                                              kSelectionAreaSizeYAttribute);
    assert(iteratorY != m_attributes.end());
    assert(iteratorY->second.size() != 0);
    f32 y = iteratorY->second[0]->getScalar<f32>();
    
    return glm::vec2(x, y);
}
