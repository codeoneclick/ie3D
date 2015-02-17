//
//  COcean.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 7/24/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef COcean_h
#define COcean_h

#include "IGameObject.h"

class COcean: public IGameObject
{
private:

    f32 m_size;
    f32 m_altitude;
    f32 m_waveGeneratorTimer;
    f32 m_waveGeneratorInterval;
    f32 m_textureTileFactor;
    
protected:
    
    void onSceneUpdate(f32 deltatime);
    
    void onResourceLoaded(ISharedResourceRef resource, bool success);
    void onConfigurationLoaded(ISharedConfigurationRef configuration, bool success);
    
    bool isInCameraFrustum(CSharedFrustumRef cameraFrustum);
    void onDraw(CSharedMaterialRef material);
    
    void bindCustomShaderUniforms(CSharedMaterialRef material);
    
public:
    
    COcean(CSharedResourceAccessorRef resourceAccessor,
           ISharedRenderTechniqueAccessorRef renderTechniqueAccessor);
    ~COcean(void);
};

#endif 
