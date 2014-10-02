//
//  CAtmosphericScattering.h
//  ie3D-Core
//
//  Created by sergey.sergeev on 9/24/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#ifndef CAtmosphericScattering_h
#define CAtmosphericScattering_h

#include "IGameObject.h"

class CAtmosphericScattering : public IGameObject
{
private:
    
protected:
    
    i32 m_size;
    i32 m_numSamples;
    f32 m_innerRadius;
    f32 m_outerRadius;
    f32 m_scale_01;
    f32 m_scale_02;
    f32 m_rayleighScaleDepth;
    glm::vec3 m_invWaveLength;
    f32 m_kr4PI;
    f32 m_km4PI;

    void onSceneUpdate(f32 deltatime);
    
    void onResourceLoaded(ISharedResourceRef resource, bool success);
    void onConfigurationLoaded(ISharedConfigurationRef configuration, bool success);
    
    i32  zOrder(void);
    bool checkOcclusion(void);
    ui32 numTriangles(void);
    void onBind(const std::string& mode);
    void onDraw(const std::string& mode);
    void onUnbind(const std::string& mode);
    void onBatch(const std::string& mode);
    
public:
    
    CAtmosphericScattering(CSharedResourceAccessorRef resourceAccessor,
                           ISharedRenderTechniqueAccessorRef renderTechniqueAccessor);
    ~CAtmosphericScattering(void);
};

#endif
