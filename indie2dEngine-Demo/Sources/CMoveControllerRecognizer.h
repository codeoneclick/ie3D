//
//  CMoveControllerRecognizer.h
//  indieEngineDemo
//
//  Created by Sergey Sergeev on 12/18/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CMoveControllerRecognizer_h
#define CMoveControllerRecognizer_h

#include "IInputTapRecognizerHandler.h"

class IMoveControllerHandler;

class CMoveControllerRecognizer :
public IInputTapRecognizerHandler
{
private:
    
protected:
    
    std::set<std::shared_ptr<IMoveControllerHandler> > m_handlers;
    glm::ivec2 m_point;
    
    void _OnInputTapRecognizerPressed(const glm::ivec2& _point);
    void _OnInputTapRecognizerMoved(const glm::ivec2& _point);
    void _OnInputTapRecognizerReleased(const glm::ivec2& _point);
    
    void _OnUpdate(ui32 _direction);
    
public:
    
    CMoveControllerRecognizer(void);
    ~CMoveControllerRecognizer(void);
    
    void RegisterMoveControllerHandler(std::shared_ptr<IMoveControllerHandler> _handler);
    void UnregisterMoveControllerHandler(std::shared_ptr<IMoveControllerHandler> _handler);
};

#endif
