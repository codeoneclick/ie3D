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
#include "CLight.h"
#include "CModel.h"
#include "COcean.h"
#include "CSkyBox.h"
#include "CLandscape.h"
#include "CParticleEmitter.h"
#include "CResourceAccessor.h"
#include "CRenderMgr.h"

CSceneFabricator::CSceneFabricator(CSharedConfigurationAccessorRef configurationAccessor,
                                   CSharedResourceAccessorRef resourceAccessor,
                                   ISharedScreenSpaceRenderAccessor screenSpaceTextureAccessor) :
IFabricator(configurationAccessor, resourceAccessor, screenSpaceTextureAccessor)
{
    
}

CSceneFabricator::~CSceneFabricator(void)
{
    m_camerasContainer.clear();
    m_gameObjectsContainer.clear();
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

CSharedModel CSceneFabricator::createModel(const std::string& filename)
{
    assert(m_resourceAccessor != nullptr);
	assert(m_screenSpaceTextureAccessor != nullptr);
    CSharedModel model = std::make_shared<CModel>(m_resourceAccessor, m_screenSpaceTextureAccessor);
    assert(m_configurationAccessor != nullptr);
    m_configurationAccessor->LoadModelConfiguration(filename, model);
    m_gameObjectsContainer.insert(model);
    return model;
}

void CSceneFabricator::deleteModel(CSharedModelRef model)
{
    m_gameObjectsContainer.erase(model);
}

CSharedOcean CSceneFabricator::createOcean(const std::string &filename)
{
    assert(m_resourceAccessor != nullptr);
	assert(m_screenSpaceTextureAccessor != nullptr);
    CSharedOcean ocean = std::make_shared<COcean>(m_resourceAccessor, m_screenSpaceTextureAccessor);
    assert(m_configurationAccessor != nullptr);
    m_configurationAccessor->LoadOceanConfiguration(filename, ocean);
    m_gameObjectsContainer.insert(ocean);
    return ocean;
}

void CSceneFabricator::deleteOcean(CSharedOceanRef ocean)
{
    m_gameObjectsContainer.erase(ocean);
}

CSharedLandscape CSceneFabricator::createLandscape(const std::string &filename)
{
    assert(m_resourceAccessor != nullptr);
	assert(m_screenSpaceTextureAccessor != nullptr);
    CSharedLandscape landscape = std::make_shared<CLandscape>(m_resourceAccessor, m_screenSpaceTextureAccessor);
    assert(m_configurationAccessor != nullptr);
    m_configurationAccessor->LoadLandscapeConfiguration(filename, landscape);
    m_gameObjectsContainer.insert(landscape);
    return landscape;
}

void CSceneFabricator::deleteLandscape(CSharedLandscapeRef landscape)
{
    m_gameObjectsContainer.erase(landscape);
}

CSharedParticleEmitter CSceneFabricator::createParticleEmitter(const std::string& filename)
{
    assert(m_resourceAccessor != nullptr);
	assert(m_screenSpaceTextureAccessor != nullptr);
    CSharedParticleEmitter particleEmitter = std::make_shared<CParticleEmitter>(m_resourceAccessor, m_screenSpaceTextureAccessor);
    assert(m_configurationAccessor != nullptr);
    m_configurationAccessor->LoadParticleEmitterConfiguration(filename, particleEmitter);
    m_gameObjectsContainer.insert(particleEmitter);
    return particleEmitter;
}

void CSceneFabricator::deleteParticleEmitter(CSharedParticleEmitterRef particleEmitter)
{
    m_gameObjectsContainer.erase(particleEmitter);
}

CSharedSkyBox CSceneFabricator::createSkyBox(const std::string& filename)
{
    assert(m_resourceAccessor != nullptr);
	assert(m_screenSpaceTextureAccessor != nullptr);
    CSharedSkyBox skyBox = std::make_shared<CSkyBox>(m_resourceAccessor, m_screenSpaceTextureAccessor);
    assert(m_configurationAccessor != nullptr);
    m_configurationAccessor->loadSkyBoxConfiguration(filename, skyBox);
    m_gameObjectsContainer.insert(skyBox);
    return skyBox;
}

void CSceneFabricator::deleteSkyBox(CSharedSkyBoxRef skyBox)
{
    m_gameObjectsContainer.erase(skyBox);
}
