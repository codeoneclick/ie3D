//
//  CIESAMainTransition.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 6/12/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "СIESAMainTransition.h"

CIESAMainTransition::CIESAMainTransition(const std::string& _filename, std::shared_ptr<IGraphicsContext> _graphicsContext, std::shared_ptr<IInputContext> _inputContext, std::shared_ptr<CResourceAccessor> _resourceAccessor, std::shared_ptr<CTemplateAccessor> _templateAccessor) :
IFabricator(_templateAccessor, _resourceAccessor),
IGameTransition(_filename, _graphicsContext, _inputContext, _resourceAccessor, _templateAccessor)
{
    
}

CIESAMainTransition::~CIESAMainTransition(void)
{
    
}

void CIESAMainTransition::_OnLoaded(void)
{
    m_isLoaded = true;
}

void CIESAMainTransition::_OnGameLoopUpdate(f32 _deltatime)
{

}