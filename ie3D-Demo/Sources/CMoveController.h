//
//  CMoveController.h
//  indieEngineDemo
//
//  Created by Sergey Sergeev on 12/18/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CMoveController_h
#define CMoveController_h

#include "IInputContext.h"

class IMoveControllerHandler
{
private:
    
    friend class CMoveController;
    
protected:
    
    IMoveControllerHandler(void) = default;
    
    virtual void onMoveControllerUpdate(ui32 direction) = 0;
    
public:
    
    virtual ~IMoveControllerHandler(void) = default;
};

typedef std::shared_ptr<IMoveControllerHandler> ISharedMoveControllerHandler;
typedef const ISharedMoveControllerHandler& ISharedMoveControllerHandlerRef;

class CMoveController : public IGestureRecognizerHandler
{
private:
    
protected:
    
    std::set<ISharedMoveControllerHandler> m_handlers;
    glm::ivec2 m_point;
    
    void onGestureRecognizerPressed(const glm::ivec2& point);
    void onGestureRecognizerMoved(const glm::ivec2& point);
    void onGestureRecognizerReleased(const glm::ivec2& point);
    
    void update(ui32 direction);
    
public:
    
    CMoveController(void);
    ~CMoveController(void);
    
    void addMoveControllerHandler(ISharedMoveControllerHandlerRef handler);
    void removeMoveControllerHandler(ISharedMoveControllerHandlerRef handler);
};

#endif
