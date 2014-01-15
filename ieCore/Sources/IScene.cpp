//
//  IScene.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 7/23/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "IScene.h"

IScene::IScene(IGameTransition* _root) :
m_root(_root)
{
    
}

IScene::~IScene(void)
{
    m_models.clear();
    m_lights.clear();
    m_particles.clear();
    m_colliders.clear();
}

std::vector<std::shared_ptr<IGameObject> > IScene::_OnGetColliders(void)
{
    return m_colliders;
}