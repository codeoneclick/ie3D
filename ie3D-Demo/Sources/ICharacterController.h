//
//  ICharacterController.h
//  ie3D-Demo
//
//  Created by sergey.sergeev on 8/12/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#ifndef ICharacterController_h
#define ICharacterController_h

#include "CGameObjectNavigator.h"

class ICharacterController : public IGameObjectNavigatorHandler
{
private:
    
protected:
    
    void onPositionChanged(const glm::vec3& position);
    void onRotationChanged(const glm::vec3& rotation);
    
public:
    
};

#endif
