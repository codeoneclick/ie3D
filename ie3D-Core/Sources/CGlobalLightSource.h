//
//  CGlobalLightSource.h
//  ie3D-Core
//
//  Created by sergey.sergeev on 10/20/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#ifndef CGlobalLightSource_h
#define CGlobalLightSource_h

#include "HCommon.h"
#include "ISceneUpdateHandler.h"

class CGlobalLightSource : public ISceneUpdateHandler
{
private:
    
protected:
    
    glm::mat4x4 m_view;
    glm::mat4x4 m_projection;
    
    f32 m_distanceToSun;
    f32 m_distanceToLookAt; // shadowmap specific
    
    f32 m_angle;
    glm::vec3 m_position;
    glm::vec3 m_lookAt;
    glm::vec3 m_rotationCenter;
    
    void onSceneUpdate(f32 deltatime);
    
public:
    
    CGlobalLightSource(f32 fov, f32 near, f32 far);
    ~CGlobalLightSource(void);
    
    glm::vec3 getPosition(void) const;
    
    void setLookAt(const glm::vec3& lookAt);
    glm::vec3 getLookAt(void) const;

    void setRotationCenter(const glm::vec3& rotationCenter);
    glm::vec3 getRotationCenter(void) const;
    
    void setAngle(f32 angle);
    f32 getAngle(void) const;
    
    void setDistanceToSun(f32 distance);
    void setDistanceToLookAt(f32 distance);
    
    glm::mat4x4 getViewMatrix(void) const;
    glm::mat4x4 getProjectionMatrix(void) const;
};


#endif
