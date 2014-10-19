//
//  CLightSource.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/29/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CLightSource_h
#define CLightSource_h

#include "HCommon.h"
#include "ISceneUpdateHandler.h"

class CLightSource : public ISceneUpdateHandler
{
private:
    
protected:
    
    glm::mat4x4 m_view;
    glm::mat4x4 m_projection;
    
    glm::vec3 m_position;
    glm::vec3 m_lookAt;
    
    void onSceneUpdate(f32 deltatime);
    
public:
    
    CLightSource(f32 fov, f32 near, f32 far, f32 aspect);
    ~CLightSource(void);
    
    void setPosition(const glm::vec3& position);
    glm::vec3 getPosition(void) const;
    
    void setLookAt(const glm::vec3& lookAt);
    glm::vec3 getLookAt(void) const;
    
    glm::mat4x4 getViewMatrix(void) const;
    glm::mat4x4 getProjectionMatrix(void) const;
};

#endif
