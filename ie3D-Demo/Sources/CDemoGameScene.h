//
//  CDemoGameScene.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 7/22/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CDemoGameScene_h
#define CDemoGameScene_h

#include "HCommon.h"
#include "IScene.h"
#include "HDEDeclaration.h"
#include "HDEEnums.h"

class CMapDragController;

class CDemoGameScene final : public IScene
{
private:
    
protected:
    
    std::shared_ptr<CMapDragController> m_mapDragController;
    CSharedGlobalLightSource m_globalLightSource;
    CSharedSkyBox m_skybox;
    //CSharedAtmosphericScattering m_atmosphericScattering;
    //CSharedModel m_model;
    std::map<std::string, CSharedModel> m_models;
    CSharedLandscape m_landscape;
    
    E_CHARACTER_CONTROLLER_MOVE_STATE m_characterControllerMoveState;
    E_CHARACTER_CONTROLLER_STEER_STATE m_characterControllerSteerState;
    
    CSharedGameObjectNavigator m_gameObjectNavigator;
    ISharedCharacterController m_characterController;
    
    CESharedComplexModel m_lightTank;
    CESharedComplexModel m_mediumTank;
    CESharedComplexModel m_heavyTank;
    
    CDESharedUIToSceneCommands m_uiToSceneCommands;
    CDESharedSceneToUICommands m_sceneToUICommands;
    
    void onCharacterMoveStateChanged(E_CHARACTER_CONTROLLER_MOVE_STATE state);
    void onCharacterSteerStateChanged(E_CHARACTER_CONTROLLER_STEER_STATE state);
    
    void onGestureRecognizerPressed(const glm::ivec2& point, E_INPUT_BUTTON inputButton) {};
    void onGestureRecognizerDragged(const glm::ivec2& point, E_INPUT_BUTTON inputButton) {};
    void onGestureRecognizerMoved(const glm::ivec2& point) {};
    void onGestureRecognizerReleased(const glm::ivec2& point, E_INPUT_BUTTON inputButton) {};
    void onGestureRecognizerWheelScroll(E_SCROLL_WHEEL_DIRECTION direction) {};
    
    void onKeyDown(i32 key);
    void onKeyUp(i32 key);
    
    void onCollision(const glm::vec3& position, ISharedGameObjectRef gameObject);
    
public:
    
    CDemoGameScene(IGameTransition* root);
    ~CDemoGameScene(void);
    
    void load(void);
    void update(f32 deltatime);
    
    CDESharedUIToSceneCommands getUIToSceneCommands(void) const;
    void setSceneToUICommands(CDESharedSceneToUICommandsRef commands);
};

#endif 
