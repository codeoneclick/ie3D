//
//  CDemoController-OSX.cpp
//  ie3D-Demo
//
//  Created by sergey.sergeev on 9/19/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#include "CDemoController-OSX.h"
#include "IOGLWindow.h"
#include "CDemoGameController.h"
#include "CDemoGameTransition.h"
#include "CGameLoopExecutor.h"

CDemoControllerOSX::CDemoControllerOSX(NSView *openGLView)
{
    std::shared_ptr<IOGLWindow> window = std::make_shared<IOGLWindow>((__bridge void*)openGLView);
    gameController = std::make_shared<CDemoGameController>();
    gameTransition = gameController->CreateKOTHInGameTransition("transition.main.xml", window);
    gameController->RegisterTransition(gameTransition);
    gameController->GoToTransition("transition.main.xml");
}

CDemoControllerOSX::~CDemoControllerOSX(void)
{
    
}