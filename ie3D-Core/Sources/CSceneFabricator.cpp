//
//  CSceneFabricator.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/28/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CSceneFabricator.h"
#include "CConfigurationAccessor.h"
#include "CCamera.h"
#include "CGlobalLightSource.h"
#include "CModel.h"
#include "COcean.h"
#include "CSkyBox.h"
#include "CLandscape.h"
#include "CAtmosphericScattering.h"
#include "CParticleEmitter.h"
#include "CResourceAccessor.h"

CSceneFabricator::CSceneFabricator(CSharedConfigurationAccessorRef configurationAccessor,
                                   CSharedResourceAccessorRef resourceAccessor,
                                   ISharedRenderTechniqueAccessorRef renderTechniqueAccessor) :
IFabricator(configurationAccessor, resourceAccessor, renderTechniqueAccessor)
{
    
}

CSceneFabricator::~CSceneFabricator(void)
{
    m_camerasContainer.clear();
    m_gameObjectsContainer.clear();
    m_lightSourcesContainer.clear();
}

CSharedCamera CSceneFabricator::createCamera(f32 fov, f32 near, f32 far,const glm::ivec4& viewport)
{
    CSharedCamera camera = std::make_shared<CCamera>(fov, near, far, viewport);
    m_camerasContainer.insert(camera);
    return camera;
}

void CSceneFabricator::deleteCamera(CSharedCameraRef camera)
{
    m_camerasContainer.erase(camera);
}

CSharedGlobalLightSource CSceneFabricator::createGlobalLightSource(f32 fov, f32 near, f32 far)
{
    CSharedGlobalLightSource lightSource = std::make_shared<CGlobalLightSource>(fov, near, far);
    m_lightSourcesContainer.insert(lightSource);
    return lightSource;
}

void CSceneFabricator::deleteGlobalLightSource(CSharedGlobalLightSourceRef lightSource)
{
    m_lightSourcesContainer.erase(lightSource);
}

CSharedModel CSceneFabricator::createModel(const std::string& filename)
{
    assert(m_resourceAccessor != nullptr);
	assert(m_renderTechniqueAccessor != nullptr);
    CSharedModel model = std::make_shared<CModel>(m_resourceAccessor, m_renderTechniqueAccessor);
    assert(m_configurationAccessor != nullptr);
    m_configurationAccessor->getConfigurationModel(filename, model);
    m_gameObjectsContainer.insert(model);
    return model;
}

CSharedOcean CSceneFabricator::createOcean(const std::string &filename)
{
    assert(m_resourceAccessor != nullptr);
	assert(m_renderTechniqueAccessor != nullptr);
    CSharedOcean ocean = std::make_shared<COcean>(m_resourceAccessor, m_renderTechniqueAccessor);
    assert(m_configurationAccessor != nullptr);
    m_configurationAccessor->getConfigurationOcean(filename, ocean);
    m_gameObjectsContainer.insert(ocean);
    return ocean;
}

CSharedLandscape CSceneFabricator::createLandscape(const std::string &filename)
{
    assert(m_resourceAccessor != nullptr);
	assert(m_renderTechniqueAccessor != nullptr);
    CSharedLandscape landscape = std::make_shared<CLandscape>(m_resourceAccessor, m_renderTechniqueAccessor);
    assert(m_configurationAccessor != nullptr);
    m_configurationAccessor->getConfigurationLandscape(filename, landscape);
    m_gameObjectsContainer.insert(landscape);
    return landscape;
}

CSharedParticleEmitter CSceneFabricator::createParticleEmitter(const std::string& filename)
{
    assert(m_resourceAccessor != nullptr);
	assert(m_renderTechniqueAccessor != nullptr);
    CSharedParticleEmitter particleEmitter = std::make_shared<CParticleEmitter>(m_resourceAccessor, m_renderTechniqueAccessor);
    assert(m_configurationAccessor != nullptr);
    m_configurationAccessor->getConfigurationParticleEmitter(filename, particleEmitter);
    m_gameObjectsContainer.insert(particleEmitter);
    return particleEmitter;
}

CSharedSkyBox CSceneFabricator::createSkybox(const std::string& filename)
{
    assert(m_resourceAccessor != nullptr);
	assert(m_renderTechniqueAccessor != nullptr);
    CSharedSkyBox skybox = std::make_shared<CSkyBox>(m_resourceAccessor, m_renderTechniqueAccessor);
    assert(m_configurationAccessor != nullptr);
    m_configurationAccessor->getConfigurationSkybox(filename, skybox);
    m_gameObjectsContainer.insert(skybox);
    return skybox;
    return nullptr;
}

CSharedAtmosphericScattering CSceneFabricator::createAtmosphericScattering(const std::string& filename)
{
    /*assert(m_resourceAccessor != nullptr);
    assert(m_renderTechniqueAccessor != nullptr);
    CSharedAtmosphericScattering atmosphericScattering = std::make_shared<CAtmosphericScattering>(m_resourceAccessor, m_renderTechniqueAccessor);
    assert(m_configurationAccessor != nullptr);
    m_configurationAccessor->loadAtmosphericScatteringConfiguration(filename, atmosphericScattering);
    m_gameObjectsContainer.insert(atmosphericScattering);
    return atmosphericScattering;*/
    return nullptr;
}

void CSceneFabricator::deleteGameObject(ISharedGameObjectRef gameObject)
{
    m_gameObjectsContainer.erase(gameObject);
}


