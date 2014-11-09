//
//  CFrustum.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/24/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CFrustum_h
#define CFrustum_h

#include "HCommon.h"
#include "HEnums.h"

class CCamera;
class CFrustum
{
private:
    
    glm::vec4 m_planes[E_FRUSTUM_PLANE_MAX];
    CCamera* m_camera;
    
protected:
    
    glm::vec4 _CreatePlane(const glm::vec3& _point_01, const glm::vec3& _point_02, const glm::vec3& _point_03);
    
    static f32 getDistanceToPlane(const glm::vec4& plane, const glm::vec3& point);
    static glm::vec3 getPlaneABC(const glm::vec4& plane);
    static f32 getPlaneD(const glm::vec4& plane);
    
public:
    
    CFrustum(CCamera* camera);
    ~CFrustum(void);
    
    void Update(void);
    
    E_FRUSTUM_BOUND_RESULT IsPointInFrustum(const glm::vec3& _point);
    E_FRUSTUM_BOUND_RESULT IsSphereInFrumstum(const glm::vec3& _center, f32 _radius);
    E_FRUSTUM_BOUND_RESULT IsBoundBoxInFrustum(const glm::vec3& _maxBound, const glm::vec3& _minBound);
};

#endif
