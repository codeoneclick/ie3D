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

#if defined(__WIN32__)

	static IInputContext* m_sharedInstance;
	POINT m_mousePosition;
	bool m_isTapped;
    
#elif defined(__NDK__)
    
    static IInputContext* m_sharedInstance;
    
#endif

public:

    IInputContext(void* _window);
    ~IInputContext(void);

#if defined(__WIN32__)

	static LRESULT CALLBACK InputProcess(HWND _hwnd, UINT _message, WPARAM _paramW, LPARAM _paramL);

#endif

    void TapRecognizerPressed(const glm::ivec2& _point);
    void TapRecognizerMoved(const glm::ivec2& _point);
    void TapRecognizerReleased(const glm::ivec2& _point);

    void RegisterTapRecognizerHandler(std::shared_ptr<IInputTapRecognizerHandler> _handler);
    void UnregisterTapRecognizerHandler(std::shared_ptr<IInputTapRecognizerHandler> _handler);
    
#if defined(__NDK__)
    
    static void NativeCallTapRecognizerPressed(const glm::ivec2& _point);
    static void NativeCallTapRecognizerMoved(const glm::ivec2& _point);
    static void NativeCallTapRecognizerReleased(const glm::ivec2& _point);
    
#endif
};

#endif 
