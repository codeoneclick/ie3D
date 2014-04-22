//
//  CNavigator.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 7/23/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CNavigator_h
#define CNavigator_h

#include "HCommon.h"

class CNavigator
{
private:
    
protected:
    
    f32 m_moveForwardSpeed;
    f32 m_moveBackwardSpeed;
    f32 m_strafeSpeed;
    f32 m_steerSpeed;
    
    glm::vec3 m_position;
    glm::vec3 m_rotation;
    
public:
    
    CNavigator(f32 _moveForwardSpeed,
               f32 _moveBackwardSpeed,
               f32 _strafeSpeed,
               f32 _steerSpeed);
    
    ~CNavigator(void);
    
    virtual bool MoveForward(void);
    virtual bool MoveBackward(void);
    virtual bool MoveLeft(void);
    virtual bool MoveRight(void);
    
    virtual void SteerLeft(void);
    virtual void SteerRight(void);
    
    inline glm::vec3 Get_Position(void)
    {
        return m_position;
    };
    
    virtual void Set_Position(const glm::vec3& _position);
    
    inline glm::vec3 Get_Rotation(void)
    {
        return m_rotation;
    };
    
    virtual inline void Set_Rotation(const glm::vec3& _rotation)
    {
        m_rotation = _rotation;
    };
    
    inline void Set_MoveForwardSpeed(f32 _speed)
    {
        m_moveForwardSpeed = _speed;
    };
    
    inline void Set_MoveBackwardSpeed(f32 _speed)
    {
        m_moveBackwardSpeed = _speed;
    };
    
    inline void Set_StrafeSpeed(f32 _speed)
    {
        m_strafeSpeed = _speed;
    };
    
    inline void Set_SteerSpeed(f32 _speed)
    {
        m_steerSpeed = _speed;
    };
};
#endif 
