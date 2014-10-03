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

class CLightSource
{
private:
    
protected:
    
    glm::vec3 m_position;
    
public:
    
    CLightSource(void);
    ~CLightSource(void);
    
    void setPosition(const glm::vec3& position);
    glm::vec3 getPosition(void) const;
};

#endif
