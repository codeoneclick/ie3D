//
//  CCharacterController.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 7/23/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CCharacterController_h
#define CCharacterController_h

#include "HCommon.h"
#include "HKOTHEnums.h"

class CCamera;
class IGameObject;
class CNavigator;

class CCharacterController
{
private:
    
protected:
    
    std::shared_ptr<CCamera> m_camera;
    std::shared_ptr<IGameObject> m_character;
    std::shared_ptr<CNavigator> m_navigator;
    
    E_MOVE_CONTROLLER_DIRECTION m_moveDirection;
    E_ROTATE_CONTROLLER_DIRECTION m_rotateDirection;
    
    glm::vec3 m_position;
    glm::vec3 m_rotation;
    
    void OnMoveControllerUpdate(ui32 _direction);
    void OnRotateControllerUpdate(ui32 _direction);
    
public:
    
    CCharacterController(void);
    ~CCharacterController(void);
    
    void OnUpdate(f32 _deltatime);
    
    inline void Set_Camera(std::shared_ptr<CCamera> _camera)
    {
        m_camera = _camera;
    };
    
    inline void Set_Character(std::shared_ptr<IGameObject> _character)
    {
        m_character = _character;
    };
    
	inline void Set_Navigator(std::shared_ptr<CNavigator> _navigator)
	{
		m_navigator = _navigator;
	};
    
    void Set_Position(const glm::vec3& _position);
    
    inline glm::vec3 Get_Position(void)
    {
        return m_position;
    };
    
    void Set_Rotation(const glm::vec3& _rotation);
    
    inline glm::vec3 Get_Rotation(void)
    {
        return m_rotation;
    };
};

#endif
