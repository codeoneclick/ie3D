//
//  CMEgoeScene.h
//  ie3D-EMap
//
//  Created by sergey.sergeev on 2/6/15.
//
//

#ifndef CMEgoeScene_h
#define CMEgoeScene_h

#include "IScene.h"
#include "HMEDeclaration.h"

class CMEgoeScene final :
public IScene,
public IGestureRecognizerHandler
{
private:
    
protected:
    
    CSharedGlobalLightSource m_globalLightSource;
    CSharedModel m_model;
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
    
    void setMeshFilenameCommand(const std::string& filename);
    void updateConfigurationMaterial(CSharedConfigurationMaterialRef configuration);
    
    CSharedConfigurationModel createTempConfigurationModel(const std::string& filename);
    
public:
    
    CMEgoeScene(IGameTransition* root);
    ~CMEgoeScene(void);
    
    void load(void);
    void update(f32 deltatime);
};


#endif
