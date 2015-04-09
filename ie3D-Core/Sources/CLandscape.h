//
//  CLandscape.h
//  iGaia
//
//  Created by sergey.sergeev on 1/16/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef CLandscape_h
#define CLandscape_h

#include "IGameObject.h"
#include "HDeclaration.h"
#include "IEditableLandscape.h"

class CLandscape : public IGameObject,
public IEditableLandscape
{
private:

    f32 m_tillingTexcoord[E_SHADER_SAMPLER_MAX];
    
    
protected:

    void onSceneUpdate(f32 deltatime);
    
    void onResourceLoaded(ISharedResourceRef resource, bool success);
    void onConfigurationLoaded(ISharedConfigurationRef configuration, bool success);
    
    bool isInCameraFrustum(CSharedFrustumRef cameraFrustum);
    void onDraw(CSharedMaterialRef material);
    void onBind(CSharedMaterialRef material);
    void onUnbind(CSharedMaterialRef material);
    
    void bindCustomShaderUniforms(CSharedMaterialRef material);
    
    bool isPointInBoundPlane(const glm::vec3& point,
                             const glm::vec3& minBound,
                             const glm::vec3& maxBound);
    E_LANDSCAPE_CHUNK_LOD getLOD(const glm::vec3& point,
                                 const glm::vec3& minBound,
                                 const glm::vec3& maxBound);
    
public:

    CLandscape(CSharedResourceAccessorRef resourceAccessor,
               ISharedRenderTechniqueAccessorRef renderTechniqueAccessor);
    ~CLandscape(void);
    
    void setTillingTexcoord(f32 value, E_SHADER_SAMPLER sampler);
    f32 getTillingTexcoord(E_SHADER_SAMPLER sampler) const;
    
    std::vector<ISharedGameObject> getChunks(void) const;

    glm::ivec2 getHeightmapSize(void) const;
    
    f32 getHeight(const glm::vec3& position) const;
    glm::vec2 getAngleOnHeightmapSurface(const glm::vec3& position) const;
};

#endif
