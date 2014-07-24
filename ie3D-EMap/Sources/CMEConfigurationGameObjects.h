//
//  CMEConfigurationGameObjects.h
//  ie3D-Demo
//
//  Created by Sergey Sergeev on 6/7/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#ifndef CMEConfigurationGameObjects_h
#define CMEConfigurationGameObjects_h

#include "CConfigurationGameObjects.h"
#include "HDeclaration.h"

class CMEConfigurationLandscapeBrush : public CConfigurationGameObject
{
public:
    
    const std::string kLandscapeBrushMainNode;
    const std::string kLandscapeBrushSizeXAttribute;
    const std::string kLandscapeBrushSizeYAttribute;
    
private:
    
protected:
    
public:
    
    CMEConfigurationLandscapeBrush(void);
    ~CMEConfigurationLandscapeBrush(void);
    
    glm::vec2 getSize(void) const;
};

class CMEConfigurationTankComplex : public CConfigurationGameObject
{
public:

    const std::string kTankComplexMainNode;
    const std::string kTankComplexModelsConfigurationsNode;
    const std::string kTankComplexModelConfigurationsNode;
    const std::string kTankComplexModelFilenameAttribute;
    const std::string kTankComplexModelPartAttribute;
    
private:
    
protected:
    
public:
    
    CMEConfigurationTankComplex(void);
    ~CMEConfigurationTankComplex(void);
    
    std::vector<std::string> getModelsConfigurationParts(void) const;
    std::vector<std::string> getModelsConfigurationsFilenames(void) const;
    std::vector<ISharedConfiguration> getModelsConfigurations(void) const;

};


#endif
