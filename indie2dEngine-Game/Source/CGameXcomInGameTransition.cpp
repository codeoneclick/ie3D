//
//  CGameXcomInGameTransition.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 6/12/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CGameXcomInGameTransition.h"

CGameXcomInGameTransition::CGameXcomInGameTransition(const std::string& _filename, std::shared_ptr<IGraphicsContext> _graphicsContext, std::shared_ptr<CResourceAccessor> _resourceAccessor, std::shared_ptr<CTemplateAccessor> _templateAccessor) :
IGameTransition(_filename, _graphicsContext, _resourceAccessor, _templateAccessor)
{
    
}

CGameXcomInGameTransition::~CGameXcomInGameTransition(void)
{
    
}

