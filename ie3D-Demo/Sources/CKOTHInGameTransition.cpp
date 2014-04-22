//
//  CKOTHInGameTransition.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 6/12/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CKOTHInGameTransition.h"
#include "CKOTHInGameScene.h"

CKOTHInGameTransition::CKOTHInGameTransition(const std::string& _filename, std::shared_ptr<IGraphicsContext> _graphicsContext, std::shared_ptr<IInputContext> _inputContext, std::shared_ptr<CResourceAccessor> _resourceAccessor, std::shared_ptr<CConfigurationAccessor> _templateAccessor) :
IFabricator(_templateAccessor, _resourceAccessor),
IGameTransition(_filename, _graphicsContext, _inputContext, _resourceAccessor, _templateAccessor)
{
    
}

CKOTHInGameTransition::~CKOTHInGameTransition(void)
{
    
}

void CKOTHInGameTransition::_OnLoaded(void)
{																			
    m_scene = std::make_shared<CKOTHInGameScene>(this);
    m_scene->load();
    m_isLoaded = true;
}

void CKOTHInGameTransition::_OnGameLoopUpdate(f32 deltatime)
{
    if(m_isLoaded && m_scene != nullptr)
    {
        m_scene->update(deltatime);
    }
}