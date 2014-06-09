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

class CMEConfigurationSelectionArea : public CConfigurationGameObject
{
public:
    
    const std::string kSelectionAreaMainNode;
    const std::string kSelectionAreaSizeXAttribute;
    const std::string kSelectionAreaSizeYAttribute;
    
private:
    
protected:
    
public:
    
    CMEConfigurationSelectionArea(void);
    ~CMEConfigurationSelectionArea(void);
    
    glm::vec2 getSize(void) const;
};


#endif
