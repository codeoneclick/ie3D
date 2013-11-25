//
//  CSceneFabricator.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/28/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CSceneFabricator.h"
#include "CTemplateAccessor.h"
#include "CCamera.h"
#include "CLight.h"
#include "CSprite.h"
#include "CModel.h"
#include "COcean.h"
#include "CLandscape.h"
#include "CBillboard.h"
#include "CParticleEmitter.h"
#include "CResourceAccessor.h"
#include "CRenderMgr.h"

CSceneFabricator::CSceneFabricator(const std::shared_ptr<CTemplateAccessor>& _templateAccessor, const std::shared_ptr<CResourceAccessor>& _resourceAccessor) :
IFabricator(_templateAccessor, _resourceAccessor)
{
    
}

CSceneFabricator::~CSceneFabricator(void)
{
    m_lightsContainer.clear();
    m_camerasContainer.clear();
    m_gameObjectsContainer.clear();
}

std::shared_ptr<CCamera> CSceneFabricator::CreateCamera(f32 _fov, f32 _near, f32 _far,const glm::ivec4& _viewport)
{
    std::shared_ptr<CCamera> camera = std::make_shared<CCamera>(_fov, _near, _far, _viewport);
    m_camerasContainer.insert(camera);
    return camera;
}

void CSceneFabricator::DeleteCamera(const std::shared_ptr<CCamera>& _camera)
{
    m_camerasContainer.erase(_camera);
}

std::shared_ptr<CLight> CSceneFabricator::CreateLight(void)
{
    std::shared_ptr<CLight> light = std::make_shared<CLight>();
    m_lightsContainer.insert(light);
    return light;
}

void CSceneFabricator::DeleteLight(const std::shared_ptr<CLight>& _light)
{
    m_lightsContainer.erase(_light);
}

std::shared_ptr<CSprite> CSceneFabricator::CreateSprite(const std::string& _filename)
{
    return nullptr;
}

void CSceneFabricator::DeleteSprite(const std::shared_ptr<CSprite>& _sprite)
{
    
}

std::shared_ptr<CModel> CSceneFabricator::CreateModel(const std::string& _filename)
{
    assert(m_resourceAccessor != nullptr);
	assert(m_screenSpaceTextureAccessor != nullptr);
    std::shared_ptr<CModel> model = std::make_shared<CModel>(m_resourceAccessor, m_screenSpaceTextureAccessor);
    assert(m_templateAccessor != nullptr);
    m_templateAccessor->LoadModelTemplate(_filename, model);
    m_gameObjectsContainer.insert(model);
    return model;
}

void CSceneFabricator::DeleteModel(const std::shared_ptr<CModel>& _model)
{
    m_gameObjectsContainer.erase(_model);
}

std::shared_ptr<COcean> CSceneFabricator::CreateOcean(const std::string &_filename)
{
    assert(m_resourceAccessor != nullptr);
	assert(m_screenSpaceTextureAccessor != nullptr);
    std::shared_ptr<COcean> ocean = std::make_shared<COcean>(m_resourceAccessor, m_screenSpaceTextureAccessor);
    assert(m_templateAccessor != nullptr);
    m_templateAccessor->LoadOceanTemplate(_filename, ocean);
    m_gameObjectsContainer.insert(ocean);
    return ocean;
}

void CSceneFabricator::DeleteOcean(const std::shared_ptr<COcean>& _ocean)
{
    m_gameObjectsContainer.erase(_ocean);
}

std::shared_ptr<CLandscape> CSceneFabricator::CreateLandscape(const std::string &_filename)
{
    assert(m_resourceAccessor != nullptr);
	assert(m_screenSpaceTextureAccessor != nullptr);
    std::shared_ptr<CLandscape> landscape = std::make_shared<CLandscape>(m_resourceAccessor, m_screenSpaceTextureAccessor);
    assert(m_templateAccessor != nullptr);
    m_templateAccessor->LoadLandscapeTemplate(_filename, landscape);
    m_gameObjectsContainer.insert(landscape);
    return landscape;
}

void CSceneFabricator::DeleteLandscape(const std::shared_ptr<CLandscape> &_landscape)
{
    
}

std::shared_ptr<CBillboard> CSceneFabricator::CreateBillboard(const std::string& _filename)
{
    return nullptr;
}

void CSceneFabricator::DeleteBillboard(const std::shared_ptr<CBillboard>& _billboard)
{
    
}

std::shared_ptr<CParticleEmitter> CSceneFabricator::CreateParticleEmitter(const std::string& _filename)
{
    assert(m_resourceAccessor != nullptr);
	assert(m_screenSpaceTextureAccessor != nullptr);
    std::shared_ptr<CParticleEmitter> particleEmitter = std::make_shared<CParticleEmitter>(m_resourceAccessor, m_screenSpaceTextureAccessor);
    assert(m_templateAccessor != nullptr);
    m_templateAccessor->LoadParticleEmitterTemplate(_filename, particleEmitter);
    m_gameObjectsContainer.insert(particleEmitter);
    return particleEmitter;
}

void CSceneFabricator::DeleteParticleEmitter(const std::shared_ptr<CParticleEmitter>& _particleEmitter)
{
    m_gameObjectsContainer.erase(_particleEmitter);
}
