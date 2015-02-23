//
//  CComponentDebugRendering.h
//  ie3D-Core
//
//  Created by sergey.sergeev on 2/17/15.
//  Copyright (c) 2015 Sergey Sergeev. All rights reserved.
//

#ifndef CComponentDebugRendering_h
#define CComponentDebugRendering_h

#include "IComponent.h"
#include "CMaterial.h"

class CComponentDebugRendering : public IComponent
{
private:
    
    CSharedBoundingBox m_boundingBox;
    CSharedMaterial m_material;
    std::function<void(CSharedMaterialRef)> m_materialBindImposer;
    
    CSharedFrustum m_cameraFrustum;

    typedef std::function<bool(CSharedFrustumRef)> T_CHECK_IN_CAMERA_FRUSTUM_COMMAND;
    T_CHECK_IN_CAMERA_FRUSTUM_COMMAND m_checkInCameraFrustumCommand;
    
protected:
    
public:
    
    CComponentDebugRendering(CSharedResourceAccessorRef resourceAccessor,
                             ISharedRenderTechniqueAccessorRef renderTechniqueAccessor,
                             CSharedFrustumRef cameraFrustum,
                             const glm::vec3& minBound,
                             const glm::vec3& maxBound,
                             const std::function<void(CSharedMaterialRef)>& materialBindImposer);
    ~CComponentDebugRendering(void);
    
    E_COMPONENT_CLASS getClass(void) const;
    
    void draw(const std::string& techniqueName);
    
    void setCheckInCameraFrustumCommand(const T_CHECK_IN_CAMERA_FRUSTUM_COMMAND& command);
    bool isInCameraFrustum(void) const;
};

#endif
