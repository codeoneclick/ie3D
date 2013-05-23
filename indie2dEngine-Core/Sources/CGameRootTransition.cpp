//
//  CGameRootTransition.cpp
//  indi2dEngine
//
//  Created by Sergey Sergeev on 5/14/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CGameRootTransition.h"
#include "CResourceFabricator.h"

#ifdef __APPLE__
#include "COGLContext_iOS.h"
#else
#endif

CGameRootTransition::CGameRootTransition(const void* _glWindow)
{
#ifdef __APPLE__
    
	UIView* glWindow = (__bridge UIView*)_glWindow;
    assert([[glWindow layer] isKindOfClass:[CAEAGLLayer class]]);
    m_glContext = std::make_shared<COGLContext_iOS>(static_cast<CAEAGLLayer*>(glWindow.layer));
    
#else
    
    // TODO :
    
#endif
    
    m_resourceFabricator = std::make_shared<CResourceFabricator>();
    
}

CGameRootTransition::~CGameRootTransition(void)
{
    
}