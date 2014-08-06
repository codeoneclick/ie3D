//
//  CEConfigurationGameObjects.cpp
//  ie3D-Engine
//
//  Created by sergey.sergeev on 8/6/14.
//  Copyright (c) 2014 sergey.sergeev. All rights reserved.
//

#include "CEConfigurationGameObjects.h"

CEConfigurationComplexModel::CEConfigurationComplexModel(void) :
CConfigurationGameObject(E_CONFIGURATION_CLASS_CUSTOM),
kComplexModelMainNode("complex_model"),
kComplexModelsConfigurationsNode("models"),
kComplexModelConfigurationsNode("model"),
kComplexModelFilenameAttribute("filename"),
kComplexModelPartAttribute("part")
{
    
}

CEConfigurationComplexModel::~CEConfigurationComplexModel(void)
{
    
}

std::vector<std::string> CEConfigurationComplexModel::getModelsConfigurationsFilenames(void) const
{
    const auto& iterator = m_attributes.find(kComplexModelsConfigurationsNode + ":" +
                                             kComplexModelConfigurationsNode + ":" +
                                             kComplexModelFilenameAttribute);
    assert(iterator != m_attributes.end());
    std::vector<std::string> filenames;
    for(const auto& filename : iterator->second)
    {
        filenames.push_back(filename->getString());
    }
    return filenames;
}

std::vector<std::string> CEConfigurationComplexModel::getModelsConfigurationParts(void) const
{
    const auto& iterator = m_attributes.find(kComplexModelsConfigurationsNode + ":" +
                                             kComplexModelConfigurationsNode + ":" +
                                             kComplexModelPartAttribute);
    assert(iterator != m_attributes.end());
    std::vector<std::string> filenames;
    for(const auto& filename : iterator->second)
    {
        filenames.push_back(filename->getString());
    }
    return filenames;
}

std::vector<ISharedConfiguration> CEConfigurationComplexModel::getModelsConfigurations(void) const
{
    const auto& iterator = m_configurations.find(kComplexModelsConfigurationsNode + ":" +
                                                 kComplexModelConfigurationsNode + ":" +
                                                 kComplexModelFilenameAttribute);
    assert(iterator != m_configurations.end());
    return iterator->second;
}
