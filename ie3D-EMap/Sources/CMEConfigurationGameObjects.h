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

class CMEConfigurationModelBrush : public CConfigurationGameObject
{
public:
    
    const std::string kModelBrushMainNode;
    const std::string kModelBrushElementsConfigurationsNode;
    const std::string kModelBrushElementConfigurationNode;
    const std::string kModelBrushElementNameAttribute;
    const std::string kModelBrushElementFilenameAttribute;
    
private:
    
protected:
    
public:
    
    CMEConfigurationModelBrush(void);
    ~CMEConfigurationModelBrush(void);
    
    std::vector<std::string> getElementsNames(void) const;
    std::vector<std::string> getElementsFilenames(void) const;
    std::vector<ISharedConfiguration> getElementsConfigurations(void) const;
};

#endif
