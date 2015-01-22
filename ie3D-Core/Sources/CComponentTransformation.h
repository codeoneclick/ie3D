//
//  CComponentTransformation.h
//  ie3D-Core
//
//  Created by sergey.sergeev on 1/22/15.
//  Copyright (c) 2015 Sergey Sergeev. All rights reserved.
//

#ifndef CComponentTransformation_h
#define CComponentTransformation_h

#include "IComponent.h"

class CComponentTransformation : public IComponent
{
private:
    
    glm::vec3 m_position;
    glm::vec3 m_rotation;
    glm::vec3 m_scale;
    
    glm::mat4 m_matrixTranslation;
    glm::mat4 m_matrixRotation;
    glm::mat4 m_matrixScaling;
    
    glm::mat4 m_matrixTransformation;
    
    bool m_isComputed;
    
protected:
    
public:
    
    CComponentTransformation(void);
    ~CComponentTransformation(void);
    
    E_COMPONENT_CLASS getClass(void) const;
    
    void setPosition(const glm::vec3& position);
    void setRotation(const glm::vec3& rotation);
    void setScale(const glm::vec3& scale);
    
    glm::vec3 getPosition(void) const;
    glm::vec3 getRotation(void) const;
    glm::vec3 getScale(void) const;
    
    glm::mat4 getTransformation(void);
};

#endif
