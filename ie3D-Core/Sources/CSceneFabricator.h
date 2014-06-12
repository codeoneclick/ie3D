//
//  CSceneFabricator.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/28/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CSceneFabricator_h
#define CSceneFabricator_h

#include "HCommon.h"
#include "IFabricator.h"
#include "HDeclaration.h"

class CSceneFabricator : public IFabricator
{
private:

protected:
    
    std::set<ISharedGameObject> m_gameObjectsContainer;
    std::set<CSharedCamera> m_camerasContainer;

public:
    
    CSceneFabricator(CSharedConfigurationAccessorRef configurationAccessor,
                     CSharedResourceAccessorRef resourceAccessor,
                     ISharedScreenSpaceRenderAccessor screenSpaceTextureAccessor);
    
    virtual ~CSceneFabricator(void);
    
    CSharedCamera createCamera(f32 fov, f32 near, f32 far,const glm::ivec4& viewport);
    void deleteCamera(CSharedCameraRef camera);
    
    CSharedModel createModel(const std::string& filename);
    void deleteModel(CSharedModelRef model);
    
    CSharedOcean createOcean(const std::string& filename);
    void deleteOcean(CSharedOceanRef ocean);
    
    CSharedSkyBox createSkyBox(const std::string& filename);
    void deleteSkyBox(CSharedSkyBoxRef skyBox);
    
    CSharedLandscape createLandscape(const std::string& filename);
    void deleteLandscape(CSharedLandscapeRef landscape);
    
    CSharedParticleEmitter createParticleEmitter(const std::string& filename);
    void deleteParticleEmitter(CSharedParticleEmitterRef particleEmitter);
};

#endif
