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
#include "IInputContext.h"

class CDemoGameController;
class IGameTransition;

class CDemoControllerOSX :
public IGestureRecognizerHandler,
public std::enable_shared_from_this<CDemoControllerOSX>
{
private:
    
protected:
    
    std::shared_ptr<CDemoGameController> gameController;
    std::shared_ptr<IGameTransition> gameTransition;
    std::map<i32, bool> m_keysState;
    
    void onGestureRecognizerPressed(const glm::ivec2& point, E_INPUT_BUTTON inputButton);
    void onGestureRecognizerMoved(const glm::ivec2& point);
    void onGestureRecognizerDragged(const glm::ivec2& point, E_INPUT_BUTTON inputButton);
    void onGestureRecognizerReleased(const glm::ivec2& point, E_INPUT_BUTTON inputButton);
    void onGestureRecognizerWheelScroll(E_SCROLL_WHEEL_DIRECTION);
    
    void onKeyDown(i32 key);
    void onKeyUp(i32 key);
    
    void updateState(void);

public:
    
    CDemoControllerOSX(NSView *openGLView);
    ~CDemoControllerOSX(void);
    
    void create(void);
};

#endif
