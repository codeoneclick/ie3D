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
    f32 _Get_DistanceToPlane(const glm::vec4& _plane, const glm::vec3& _point);
    inline glm::vec3 _Get_PlaneNormal(const glm::vec4& _plane)
    {
        return glm::vec3(_plane.x, _plane.y, _plane.z);
    };
    
    inline f32 _Get_PlaneOffset(const glm::vec4& _plane)
    {
        return _plane.w;
    };
    
public:
    
    CFrustum(CCamera* _camera);
    ~CFrustum(void);
    
    void Update(void);
    
    E_FRUSTUM_BOUND_RESULT IsPointInFrustum(const glm::vec3& _point);
    E_FRUSTUM_BOUND_RESULT IsSphereInFrumstum(const glm::vec3& _center, f32 _radius);
    E_FRUSTUM_BOUND_RESULT IsBoundBoxInFrustum(const glm::vec3& _maxBound, const glm::vec3& _minBound);
};

#endif
