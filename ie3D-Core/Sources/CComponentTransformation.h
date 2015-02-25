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
    
    glm::mat4 m_matrixT;
    glm::mat4 m_matrixR;
    glm::mat4 m_matrixS;
    
    glm::mat4 m_matrixM;
    
    bool m_isMMatrixComputed;
    
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
    
    static glm::mat4 getMMatrix(CSharedComponentTransformationRef componentTransformation);
    static glm::mat4 getMVPMatrix(CSharedComponentTransformationRef componentTransformation, CSharedCameraRef camera);
    static glm::mat4 getIMVPMatrix(CSharedComponentTransformationRef componentTransformation, CSharedCameraRef camera);
};

#endif
