//
//  CComponentSceneUpdate.h
//  ie3D-Core
//
//  Created by sergey.sergeev on 2/23/15.
//  Copyright (c) 2015 Sergey Sergeev. All rights reserved.
//

#ifndef CComponentSceneUpdate_h
#define CComponentSceneUpdate_h

#include "IComponent.h"
#include "CMaterial.h"

class CComponentSceneUpdate : public IComponent
{
private:
    
    typedef std::function<void(f32)> T_SCENE_UPDATE_COMMAND;
    T_SCENE_UPDATE_COMMAND m_sceneUpdateCommand;
    
protected:
    
public:
    
    CComponentSceneUpdate(void);
    ~CComponentSceneUpdate(void);
    
    E_COMPONENT_CLASS getClass(void) const;
    
    void setSceneUpdateCommand(const T_SCENE_UPDATE_COMMAND& command);
    void sceneUpdate(f32 deltatime);
};

#endif
