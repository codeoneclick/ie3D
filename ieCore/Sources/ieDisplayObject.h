//
//  ieDisplayObject.h
//  ieCore
//
//  Created by sergey.sergeev on 4/7/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#ifndef __ieCore__ieDisplayObject__
#define __ieCore__ieDisplayObject__

#include "ieDisplayObjectContainer.h"

class ieDisplayObject : public ieDisplayObjectContainer
{
private:
    
protected:
    
    glm::mat4 m_matrixScale;
    glm::mat4 m_matrixRotation;
    glm::mat4 m_matrixTranslation;
    glm::mat4 m_matrixWorld;
    
    glm::vec3 m_position;
    glm::vec3 m_rotation;
    glm::vec3 m_scale;
    
    ieSharedMesh m_mesh;
    std::unordered_map<std::string, ieSharedMaterial> m_materials;
    
    virtual void onUpdate(ieSharedEventRef event);
    virtual void onDraw(ieSharedEventRef event);
    
    virtual void onAdded(ieSharedEventRef event);
    virtual void onRemoved(ieSharedEventRef event);
    
    ieDisplayObject(void);
    
public:
    
    virtual ~ieDisplayObject(void);
    
};

#endif
