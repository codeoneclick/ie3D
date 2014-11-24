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
#include "HDeclaration.h"
#include "ISceneUpdateHandler.h"

class CFrustum : public ISceneUpdateHandler
{
private:
    
    std::array<glm::vec4, E_FRUSTUM_PLANE_MAX> m_planes;
    CSharedCamera m_camera;
    
protected:
    
    glm::vec4 createPlane(const glm::vec3& point_01, const glm::vec3& point_02, const glm::vec3& point_03);
    
    static f32 getDistanceToPlane(const glm::vec4& plane, const glm::vec3& point);
    static glm::vec3 getPlaneABC(const glm::vec4& plane);
    static f32 getPlaneD(const glm::vec4& plane);
    
    void onSceneUpdate(f32 deltatime);
    
public:
    
    CFrustum(CSharedCameraRef camera);
    ~CFrustum(void);
    
    E_FRUSTUM_BOUND_RESULT isPointInFrustum(const glm::vec3& point);
    E_FRUSTUM_BOUND_RESULT isSphereInFrumstum(const glm::vec3& center, f32 radius);
    E_FRUSTUM_BOUND_RESULT isBoundBoxInFrustum(const glm::vec3& maxBound, const glm::vec3& minBound);
};

#endif
