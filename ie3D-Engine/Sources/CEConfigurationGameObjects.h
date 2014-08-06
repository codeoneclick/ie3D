//
//  CEConfigurationGameObjects.h
//  ie3D-Engine
//
//  Created by sergey.sergeev on 8/6/14.
//  Copyright (c) 2014 sergey.sergeev. All rights reserved.
//

#ifndef CEConfigurationGameObjects_h
#define CEConfigurationGameObjects_h

#include "CConfigurationGameObjects.h"
#include "HDeclaration.h"

class CEConfigurationComplexModel : public CConfigurationGameObject
{
public:
    
    const std::string kComplexModelMainNode;
    const std::string kComplexModelsConfigurationsNode;
    const std::string kComplexModelConfigurationsNode;
    const std::string kComplexModelFilenameAttribute;
    const std::string kComplexModelPartAttribute;
    
private:
    
protected:
    
public:
    
    CEConfigurationComplexModel(void);
    ~CEConfigurationComplexModel(void);
    
    std::vector<std::string> getModelsConfigurationParts(void) const;
    std::vector<std::string> getModelsConfigurationsFilenames(void) const;
    std::vector<ISharedConfiguration> getModelsConfigurations(void) const;
    
};


#endif
