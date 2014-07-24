//
//  CMEConfigurationGameObjects.cpp
//  ie3D-Demo
//
//  Created by Sergey Sergeev on 6/7/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#include "CMEConfigurationGameObjects.h"

CMEConfigurationLandscapeBrush::CMEConfigurationLandscapeBrush(void) :
CConfigurationGameObject(E_CONFIGURATION_CLASS_CUSTOM),
kLandscapeBrushMainNode("landscape_brush"),
kLandscapeBrushSizeXAttribute("size_x"),
kLandscapeBrushSizeYAttribute("size_y")
{
    
}

CMEConfigurationLandscapeBrush::~CMEConfigurationLandscapeBrush(void)
{
    
}

glm::vec2 CMEConfigurationLandscapeBrush::getSize(void) const
{
    const auto& iteratorX = m_attributes.find(kLandscapeBrushMainNode + ":" +
                                              kLandscapeBrushSizeXAttribute);
    assert(iteratorX != m_attributes.end());
    assert(iteratorX->second.size() != 0);
    f32 x = iteratorX->second[0]->getScalar<f32>();
    
    const auto& iteratorY = m_attributes.find(kLandscapeBrushMainNode + ":" +
                                              kLandscapeBrushSizeYAttribute);
    assert(iteratorY != m_attributes.end());
    assert(iteratorY->second.size() != 0);
    f32 y = iteratorY->second[0]->getScalar<f32>();
    
    return glm::vec2(x, y);
}


CMEConfigurationTankComplex::CMEConfigurationTankComplex(void) :
CConfigurationGameObject(E_CONFIGURATION_CLASS_CUSTOM),
kTankComplexMainNode("complex_model"),
kTankComplexModelsConfigurationsNode("models"),
kTankComplexModelConfigurationsNode("model"),
kTankComplexModelFilenameAttribute("filename"),
kTankComplexModelPartAttribute("part")
{
    
}

CMEConfigurationTankComplex::~CMEConfigurationTankComplex(void)
{
    
}

std::vector<std::string> CMEConfigurationTankComplex::getModelsConfigurationsFilenames(void) const
{
    const auto& iterator = m_attributes.find(kTankComplexModelsConfigurationsNode + ":" +
                                             kTankComplexModelConfigurationsNode + ":" +
                                             kTankComplexModelFilenameAttribute);
    assert(iterator != m_attributes.end());
    std::vector<std::string> filenames;
    for(const auto& filename : iterator->second)
    {
        filenames.push_back(filename->getString());
    }
    return filenames;
}

std::vector<std::string> CMEConfigurationTankComplex::getModelsConfigurationParts(void) const
{
    const auto& iterator = m_attributes.find(kTankComplexModelsConfigurationsNode + ":" +
                                             kTankComplexModelConfigurationsNode + ":" +
                                             kTankComplexModelPartAttribute);
    assert(iterator != m_attributes.end());
    std::vector<std::string> filenames;
    for(const auto& filename : iterator->second)
    {
        filenames.push_back(filename->getString());
    }
    return filenames;
}

std::vector<ISharedConfiguration> CMEConfigurationTankComplex::getModelsConfigurations(void) const
{
    const auto& iterator = m_configurations.find(kTankComplexModelsConfigurationsNode + ":" +
                                                 kTankComplexModelConfigurationsNode + ":" +
                                                 kTankComplexModelFilenameAttribute);
    assert(iterator != m_configurations.end());
    return iterator->second;
}
