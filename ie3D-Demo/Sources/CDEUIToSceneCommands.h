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
    
    typedef std::function<void(E_UI_NAVIGATION_COMMAND)> __SET_NAVIGATION_COMMAND;

    __SET_NAVIGATION_COMMAND m_setNavigationCommand;
    
protected:
    
public:
    
    CDEUIToSceneCommands(void);
    ~CDEUIToSceneCommands(void);
    
    void connectSetNavigationCommand(const __SET_NAVIGATION_COMMAND& command);
    void executeSetNavigationCommand(E_UI_NAVIGATION_COMMAND direction);
};


#endif
