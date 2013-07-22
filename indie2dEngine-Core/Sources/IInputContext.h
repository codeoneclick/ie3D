//
//  CInputMgr.h
//  iGaia-CartoonPanzers
//
//  Created by Sergey Sergeev on 3/27/13.
//
//

#ifndef IInputContext_h
#define IInputContext_h

#include "HCommon.h"

class IInputTapRecognizerHandler;

class IInputContext
{
private:

protected:

    std::set<std::shared_ptr<IInputTapRecognizerHandler> > m_handlers;

public:

    IInputContext(void* _window);
    ~IInputContext(void);

    void TapRecognizerPressed(const glm::ivec2& _point);
    void TapRecognizerMoved(const glm::ivec2& _point);
    void TapRecognizerReleased(const glm::ivec2& _point);

    void RegisterTapRecognizerHandler(std::shared_ptr<IInputTapRecognizerHandler> _handler);
    void UnregisterTapRecognizerHandler(std::shared_ptr<IInputTapRecognizerHandler> _handler);
};

#endif 
