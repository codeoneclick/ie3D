//
//  CMEpoeScene.h
//  ie3D-EMap-OSX
//
//  Created by sergey.sergeev on 7/28/15.
//
//

#ifndef CMEpoeScene_h
#define CMEpoeScene_h

#include "IScene.h"
#include "HMEDeclaration.h"

class CMEpoeScene final :
public IScene
{
private:
    
protected:
    
    CSharedGlobalLightSource m_globalLightSource;
    CSharedParticleEmitter m_particle;
    CSharedLandscape m_landscape;
    CSharedSkyBox m_skybox;
    glm::ivec2 m_previousDraggedPoint;
    
    void onGestureRecognizerPressed(const glm::ivec2& point, E_INPUT_BUTTON inputButton);
    void onGestureRecognizerDragged(const glm::ivec2& point, E_INPUT_BUTTON inputButton);
    void onGestureRecognizerMoved(const glm::ivec2& point);
    void onGestureRecognizerReleased(const glm::ivec2& point, E_INPUT_BUTTON inputButton);
    void onGestureRecognizerWheelScroll(E_SCROLL_WHEEL_DIRECTION direction);
    
    void onKeyUp(i32 key);
    void onKeyDown(i32 key);
    
    void onConfigurationLoaded(ISharedConfigurationRef configuration);
    
public:
    
    CMEpoeScene(IGameTransition* root);
    ~CMEpoeScene(void);
    
    void load(void);
    void update(f32 deltatime);
};

#endif
