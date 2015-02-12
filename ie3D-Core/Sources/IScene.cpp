//
//  IScene.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 7/23/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "IScene.h"
#include "IUICommands.h"

IScene::IScene(IGameTransition* root) :
m_root(root),
m_uiToSceneCommands(std::make_shared<IUICommands>()),
m_sceneToUICommands(nullptr)
{
    
}

IScene::~IScene(void)
{
    //m_models.clear();
    //m_particles.clear();
    m_colliders.clear();
}

std::vector<ISharedGameObject> IScene::colliders(void)
{
    return m_colliders;
}

void IScene::onCollision(const glm::vec3 &position, ISharedGameObjectRef gameObject, E_INPUT_BUTTON inputButton)
{
    
}

void IScene::setSceneToUICommands(ISharedUICommandsRef commands)
{
    m_sceneToUICommands = commands;
}

ISharedUICommands IScene::getUIToSceneCommands(void) const
{
    return m_uiToSceneCommands;
}