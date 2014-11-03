//
//  CMEModelsScene.h
//  ie3D-EMap
//
//  Created by sergey.sergeev on 11/3/14.
//
//

#ifndef CMEModelsScene_h
#define CMEModelsScene_h

#include "IScene.h"
#include "HMEDeclaration.h"

class CMEModelsScene final :
public IScene,
public IGestureRecognizerHandler
{
private:
    
protected:
    
    CSharedGlobalLightSource m_globalLightSource;
    CSharedModel m_model;
    
    void onConfigurationLoaded(ISharedConfigurationRef configuration);
    void onResourceLoaded(ISharedResourceRef resource);
    
    std::vector<ISharedGameObject> colliders(void);
    void onCollision(const glm::vec3& position, ISharedGameObjectRef gameObject, E_INPUT_BUTTON inputButton);
    
    void onGestureRecognizerPressed(const glm::ivec2& point, E_INPUT_BUTTON inputButton);
    void onGestureRecognizerDragged(const glm::ivec2& point, E_INPUT_BUTTON inputButton);
    void onGestureRecognizerMoved(const glm::ivec2& point);
    void onGestureRecognizerReleased(const glm::ivec2& point, E_INPUT_BUTTON inputButton);
    void onGestureRecognizerWheelScroll(E_SCROLL_WHEEL_DIRECTION direction);
    
    void onKeyUp(i32 key);
    void onKeyDown(i32 key);
    
public:
    
    CMEModelsScene(IGameTransition* root);
    ~CMEModelsScene(void);
    
    void load(void);
    void update(f32 deltatime);
};

#endif
