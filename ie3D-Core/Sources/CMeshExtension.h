//
//  CMeshExtension.h
//  ie3D-Core
//
//  Created by sergey.sergeev on 4/10/15.
//  Copyright (c) 2015 Sergey Sergeev. All rights reserved.
//

#ifndef CMeshExtension_h
#define CMeshExtension_h

#include "CMesh.h"

class CMeshExtension
{
private:
    
protected:
    
public:
    
    static CSharedMesh createBox(const glm::vec3& minBound, const glm::vec3& maxBound, const glm::u8vec4& color = glm::u8vec4(255));
    static CSharedMesh createSphere(f32 radius, i32 rings, i32 sectors, const glm::u8vec4& color = glm::u8vec4(255));
};

#endif
