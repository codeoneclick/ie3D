//
//  CCamera.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/24/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CCamera_h
#define CCamera_h

#include "HCommon.h"
#include "ISceneUpdateHandler.h"

class CCamera : public ISceneUpdateHandler
{
private:
    
    glm::mat4 m_matrixV;
    glm::mat4 m_matrixP;
    glm::mat4 m_matrixN;
    glm::mat4 m_matrixIV;
    glm::mat4 m_matrixVP;
    glm::mat4 m_matrixIVP;
    
    glm::vec3 m_position;
    glm::vec3 m_lookAt;
    glm::vec3 m_up;
    f32 m_rotation;
    glm::vec3 m_distance;

    f32 m_fov;
    f32 m_aspect;
    f32 m_near;
    f32 m_far;
    
    glm::ivec4 m_viewport;

protected:
    
    void onSceneUpdate(f32 deltatime);
    
public:
    
    CCamera(f32 fov, f32 near, f32 far, glm::ivec4 viewport);
    ~CCamera(void);
    
    inline glm::mat4 getVMatrix(void) const;
    inline glm::mat4 getPMatrix(void) const;
    inline glm::mat4 getNMatrix(void) const;
    inline glm::mat4 getIVMatrix(void) const;
    inline glm::mat4 getVPMatrix(void) const;
    inline glm::mat4 getIVPMatrix(void) const;
    
    inline void setPosition(const glm::vec3& position);
    inline glm::vec3 getPosition(void) const;
    
    inline void setLookAt(const glm::vec3& lookAt);
    inline glm::vec3 getLookAt(void) const;
    
    inline glm::vec3 getUpVector(void) const;
    
    inline void setRotation(f32 rotation);
    inline f32 getRotation(void) const;
    
    inline void setDistanceToLookAt(const glm::vec3& distance);
    inline glm::vec3 getDistanceToLookAt(void) const;
    
    inline void setFOV(f32 fov);
    inline f32 getFOV(void) const;
    
    inline f32 getAspect(void) const;
    inline f32 getNear(void) const;
    inline f32 getFar(void) const;
    
    inline glm::ivec4 getViewport(void) const;
    
    glm::mat4 getSMatrix(const glm::vec3& position); // sphetical
    glm::mat4 getCMatrix(const glm::vec3& position); // cylindrical
};

#include "CCamera.hpp"

#endif 
