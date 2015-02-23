//
//  CComponentSceneUpdate.cpp
//  ie3D-Core
//
//  Created by sergey.sergeev on 2/23/15.
//  Copyright (c) 2015 Sergey Sergeev. All rights reserved.
//

#include "CComponentSceneUpdate.h"

CComponentSceneUpdate::CComponentSceneUpdate(void) :
m_sceneUpdateCommand(nullptr)
{

}

CComponentSceneUpdate::~CComponentSceneUpdate(void)
{
    
}

E_COMPONENT_CLASS CComponentSceneUpdate::getClass(void) const
{
    return E_COMPONENT_CLASS_SCENE_UPDATE;
}

void CComponentSceneUpdate::setSceneUpdateCommand(const T_SCENE_UPDATE_COMMAND &command)
{
    m_sceneUpdateCommand = command;
}

void CComponentSceneUpdate::sceneUpdate(f32 deltatime)
{
    if(m_sceneUpdateCommand)
    {
        m_sceneUpdateCommand(deltatime);
    }
}
