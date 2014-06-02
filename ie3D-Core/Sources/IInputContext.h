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
#include "HDeclaration.h"
#include "HEnums.h"

class IGestureRecognizerHandler
{
private:
    
    friend class IInputContext;
    
protected:
    
    IGestureRecognizerHandler(void) = default;
    
    virtual void onGestureRecognizerPressed(const glm::ivec2& point, E_INPUT_BUTTON inputButton) = 0;
    virtual void onGestureRecognizerMoved(const glm::ivec2& point, E_INPUT_BUTTON inputButton) = 0;
    virtual void onGestureRecognizerReleased(const glm::ivec2& point, E_INPUT_BUTTON inputButton) = 0;
    
public:
    
    virtual ~IGestureRecognizerHandler(void) = default;
};

class IInputContext
{
private:

protected:

#if defined(__WIN32__)

	static IInputContext* m_sharedInstance;
	POINT m_mousePosition;
	bool m_isTapped;
    
#elif defined(__NDK__)
    
#endif

    std::set<ISharedGestureRecognizerHandler> m_handlers;
    IInputContext(void) = default;
    
public:
    
    virtual ~IInputContext(void);
    
#if defined(__WIN32__)
    
	static LRESULT CALLBACK inputProcess(HWND hwnd,
                                         UINT message,
                                         WPARAM paramW,
                                         LPARAM paramL);
    
#endif
    
    static std::shared_ptr<IInputContext> createInputContext(ISharedOGLWindowRef window,
                                                             E_PLATFORM_API api);
    
    void gestureRecognizerPressed(const glm::ivec2& point, E_INPUT_BUTTON inputButton);
    void gestureRecognizerMoved(const glm::ivec2& point, E_INPUT_BUTTON inputButton);
    void gestureRecognizerReleased(const glm::ivec2& point, E_INPUT_BUTTON inputButton);
    
    void addGestureRecognizerHandler(ISharedGestureRecognizerHandlerRef handler);
    void removeGestureRecognizerHandler(ISharedGestureRecognizerHandlerRef handler);
    
#if defined(__NDK__)
    
    static void nativeCallTapRecognizerPressed(const glm::ivec2& point);
    static void nativeCallTapRecognizerMoved(const glm::ivec2& point);
    static void nativeCallTapRecognizerReleased(const glm::ivec2& point);
    
#endif
    
};

#endif 
