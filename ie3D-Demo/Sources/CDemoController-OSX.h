//
//  CDemoController-OSX.h
//  ie3D-Demo
//
//  Created by sergey.sergeev on 9/19/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#ifndef CDemoControllerOSX_h
#define CDemoControllerOSX_h

#include "HCommon.h"
#include <Cocoa/Cocoa.h>

class CDemoGameController;
class IGameTransition;

class CDemoControllerOSX
{
private:
    
protected:
    
    std::shared_ptr<CDemoGameController> gameController;
    std::shared_ptr<IGameTransition> gameTransition;
    
public:
    CDemoControllerOSX(NSView *openGLView);
    ~CDemoControllerOSX(void);
};

#endif
