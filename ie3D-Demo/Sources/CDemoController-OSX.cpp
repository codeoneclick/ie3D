//
//  CDemoController-OSX.cpp
//  ie3D-Demo
//
//  Created by sergey.sergeev on 9/19/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#include "CDemoController-OSX.h"
#include "IOGLWindow.h"
#include "CDemoGameController.h"
#include "CDemoGameTransition.h"
#include "CGameLoopExecutor.h"
#include "CDEUIToSceneCommands.h"

#define key_w 119
#define key_a 97
#define key_d 100
#define key_s 115

CDemoControllerOSX::CDemoControllerOSX(NSView *openGLView)
{
    std::shared_ptr<IOGLWindow> window = std::make_shared<IOGLWindow>((__bridge void*)openGLView);
    m_gameController = std::make_shared<CDemoGameController>(window);
    m_gameTransition = std::make_shared<CDemoGameTransition>("transition.main.xml");
    m_gameController->addTransition(m_gameTransition);
    m_gameController->gotoTransition("transition.main.xml");
    
    m_keysState[key_w] = false;
    m_keysState[key_a] = false;
    m_keysState[key_d] = false;
    m_keysState[key_s] = false;
}

CDemoControllerOSX::~CDemoControllerOSX(void)
{
    
}

void CDemoControllerOSX::create(void)
{
    m_gameTransition->addGestureRecognizerHandler(shared_from_this());
}

void CDemoControllerOSX::onGestureRecognizerPressed(const glm::ivec2& point, E_INPUT_BUTTON inputButton)
{
    
}

void CDemoControllerOSX::onGestureRecognizerMoved(const glm::ivec2& point)
{
    
}

void CDemoControllerOSX::onGestureRecognizerDragged(const glm::ivec2& point, E_INPUT_BUTTON inputButton)
{
    
}

void CDemoControllerOSX::onGestureRecognizerReleased(const glm::ivec2& point, E_INPUT_BUTTON inputButton)
{
    
}

void CDemoControllerOSX::onGestureRecognizerWheelScroll(E_SCROLL_WHEEL_DIRECTION)
{
    
}

void CDemoControllerOSX::onKeyDown(i32 key)
{
    m_keysState[key] = true;
    CDemoControllerOSX::updateState();
    
    std::shared_ptr<CDemoGameTransition> transition = std::static_pointer_cast<CDemoGameTransition>(m_gameTransition);
    transition->getUIToSceneCommands()->executeOnKeyDownCommand(key);
}

void CDemoControllerOSX::onKeyUp(i32 key)
{
    m_keysState[key] = false;
    CDemoControllerOSX::updateState();
    
    std::shared_ptr<CDemoGameTransition> transition = std::static_pointer_cast<CDemoGameTransition>(m_gameTransition);
    transition->getUIToSceneCommands()->executeOnKeyUpCommand(key);
}

void CDemoControllerOSX::updateState(void)
{
    std::shared_ptr<CDemoGameTransition> transition = std::static_pointer_cast<CDemoGameTransition>(m_gameTransition);
    
    if(m_keysState[key_s] && m_keysState[key_a])
    {
        transition->getUIToSceneCommands()->executeSetCharacterSteerStateCommand(E_CHARACTER_CONTROLLER_STEER_STATE_LEFT);
        transition->getUIToSceneCommands()->executeSetCharacterMoveStateCommand(E_CHARACTER_CONTROLLER_MOVE_STATE_BACKWARD);
    }
    else if(m_keysState[key_w] && m_keysState[key_d])
    {
        transition->getUIToSceneCommands()->executeSetCharacterSteerStateCommand(E_CHARACTER_CONTROLLER_STEER_STATE_RIGHT);
        transition->getUIToSceneCommands()->executeSetCharacterMoveStateCommand(E_CHARACTER_CONTROLLER_MOVE_STATE_FORWARD);
    }
    else if(m_keysState[key_s] && m_keysState[key_d])
    {
        transition->getUIToSceneCommands()->executeSetCharacterSteerStateCommand(E_CHARACTER_CONTROLLER_STEER_STATE_RIGHT);
        transition->getUIToSceneCommands()->executeSetCharacterMoveStateCommand(E_CHARACTER_CONTROLLER_MOVE_STATE_BACKWARD);
    }
    else if(m_keysState[key_w] && m_keysState[key_a])
    {
        transition->getUIToSceneCommands()->executeSetCharacterSteerStateCommand(E_CHARACTER_CONTROLLER_STEER_STATE_LEFT);
        transition->getUIToSceneCommands()->executeSetCharacterMoveStateCommand(E_CHARACTER_CONTROLLER_MOVE_STATE_FORWARD);
    }
    else if(m_keysState[key_d])
    {
        transition->getUIToSceneCommands()->executeSetCharacterSteerStateCommand(E_CHARACTER_CONTROLLER_STEER_STATE_RIGHT);
        transition->getUIToSceneCommands()->executeSetCharacterMoveStateCommand(E_CHARACTER_CONTROLLER_MOVE_STATE_NONE);
    }
    else if(m_keysState[key_a])
    {
        transition->getUIToSceneCommands()->executeSetCharacterSteerStateCommand(E_CHARACTER_CONTROLLER_STEER_STATE_LEFT);
        transition->getUIToSceneCommands()->executeSetCharacterMoveStateCommand(E_CHARACTER_CONTROLLER_MOVE_STATE_NONE);
    }
    else if(m_keysState[key_s])
    {
        transition->getUIToSceneCommands()->executeSetCharacterSteerStateCommand(E_CHARACTER_CONTROLLER_STEER_STATE_NONE);
        transition->getUIToSceneCommands()->executeSetCharacterMoveStateCommand(E_CHARACTER_CONTROLLER_MOVE_STATE_BACKWARD);
    }
    else if(m_keysState[key_w])
    {
        transition->getUIToSceneCommands()->executeSetCharacterSteerStateCommand(E_CHARACTER_CONTROLLER_STEER_STATE_NONE);
        transition->getUIToSceneCommands()->executeSetCharacterMoveStateCommand(E_CHARACTER_CONTROLLER_MOVE_STATE_FORWARD);
    }
    else
    {
        transition->getUIToSceneCommands()->executeSetCharacterSteerStateCommand(E_CHARACTER_CONTROLLER_STEER_STATE_NONE);
        transition->getUIToSceneCommands()->executeSetCharacterMoveStateCommand(E_CHARACTER_CONTROLLER_MOVE_STATE_NONE);
    }
}