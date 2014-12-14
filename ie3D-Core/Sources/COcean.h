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

    f32 m_width;
    f32 m_height;
    f32 m_altitude;
    f32 m_waveGeneratorTimer;
    f32 m_waveGeneratorInterval;
    f32 m_textureTileFactor;
    
protected:
    
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
    
    void onOcclusionQueryDraw(CSharedMaterialRef material);
    void onOcclusionQueryUpdate(void);
    
    void bindCustomShaderUniforms(CSharedMaterialRef material);
    
public:
    
    COcean(CSharedResourceAccessorRef resourceAccessor,
           ISharedRenderTechniqueAccessorRef renderTechniqueAccessor);
    ~COcean(void);
};

#endif 
