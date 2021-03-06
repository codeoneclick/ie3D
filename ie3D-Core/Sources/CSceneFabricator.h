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
    std::set<CSharedGlobalLightSource> m_lightSourcesContainer;

public:
    
    CSceneFabricator(CSharedConfigurationAccessorRef configurationAccessor,
                     CSharedResourceAccessorRef resourceAccessor,
                     ISharedRenderTechniqueAccessorRef renderTechniqueAccessor);
    
    virtual ~CSceneFabricator(void);
    
    CSharedCamera createCamera(f32 fov, f32 near, f32 far,const glm::ivec4& viewport);
    void deleteCamera(CSharedCameraRef camera);
    
    CSharedGlobalLightSource createGlobalLightSource(f32 fov, f32 near, f32 far);
    void deleteGlobalLightSource(CSharedGlobalLightSourceRef lightSource);
    
    CSharedModel createModel(const std::string& filename);
    CSharedOcean createOcean(const std::string& filename);
    CSharedSkyBox createSkybox(const std::string& filename);
    CSharedAtmosphericScattering createAtmosphericScattering(const std::string& filename);
    CSharedLandscape createLandscape(const std::string& filename);
    CSharedParticleEmitter createParticleEmitter(const std::string& filename);
    
    void deleteGameObject(ISharedGameObjectRef gameObject);

};

#endif
