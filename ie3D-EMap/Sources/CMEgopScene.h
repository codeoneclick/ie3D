//
//  CMEgopScene.h
//  ie3D-EMap
//
//  Created by sergey.sergeev on 11/3/14.
//
//

#ifndef CMEgopScene_h
#define CMEgopScene_h

#include "IScene.h"
#include "HMEDeclaration.h"

class CMEgopScene final :
public IScene,
public IGestureRecognizerHandler
{
private:
    
protected:
    
    CSharedGlobalLightSource m_globalLightSource;
    CSharedModel m_model;
    
    void onGestureRecognizerPressed(const glm::ivec2& point, E_INPUT_BUTTON inputButton);
    void onGestureRecognizerDragged(const glm::ivec2& point, E_INPUT_BUTTON inputButton);
    void onGestureRecognizerMoved(const glm::ivec2& point);
    void onGestureRecognizerReleased(const glm::ivec2& point, E_INPUT_BUTTON inputButton);
    void onGestureRecognizerWheelScroll(E_SCROLL_WHEEL_DIRECTION direction);
    
    void onKeyUp(i32 key);
    void onKeyDown(i32 key);
    
    void setModelConfigurationFilenameCommand(const std::string& filename);
    
public:
    
    CMEgopScene(IGameTransition* root);
    ~CMEgopScene(void);
    
    void load(void);
    void update(f32 deltatime);
};

#endif
