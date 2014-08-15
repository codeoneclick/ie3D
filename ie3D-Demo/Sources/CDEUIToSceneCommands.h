//
//  CDEUIToSceneCommands.h
//  ie3D-Demo
//
//  Created by sergey.sergeev on 8/12/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#ifndef CDEUIToSceneCommands_h
#define CDEUIToSceneCommands_h

#include "HCommon.h"
#include "HDEEnums.h"

class CDEUIToSceneCommands
{
private:
    
    typedef std::function<void(E_CHARACTER_CONTROLLER_MOVE_STATE)> __SET_CHARACTER_MOVE_STATE_COMMAND;
    typedef std::function<void(E_CHARACTER_CONTROLLER_STEER_STATE)> __SET_CHARACTER_STEER_STATE_COMMAND;

    __SET_CHARACTER_MOVE_STATE_COMMAND m_setCharacterMoveStateCommand;
    __SET_CHARACTER_STEER_STATE_COMMAND m_setCharacterSteerStateCommand;
    
protected:
    
public:
    
    CDEUIToSceneCommands(void);
    ~CDEUIToSceneCommands(void);
    
    void connectSetCharacterMoveStateCommand(const __SET_CHARACTER_MOVE_STATE_COMMAND& command);
    void executeSetCharacterMoveStateCommand(E_CHARACTER_CONTROLLER_MOVE_STATE state);
    
    void connectSetCharacterSteerStateCommand(const __SET_CHARACTER_STEER_STATE_COMMAND& command);
    void executeSetCharacterSteerStateCommand(E_CHARACTER_CONTROLLER_STEER_STATE state);
};


#endif
