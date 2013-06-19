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
#include "CBillboard.h"
#include "CParticleEmitter.h"
#include "CResourceAccessor.h"

CSceneFabricator::CSceneFabricator(std::shared_ptr<CTemplateAccessor> _templateAccessor, std::shared_ptr<CResourceAccessor> _resourceAccessor) :
m_templateAccessor(_templateAccessor),
m_resourceAccessor(_resourceAccessor)
{
    
}

CSceneFabricator::~CSceneFabricator(void)
{
    m_lightsContainer.clear();
    m_camerasContainer.clear();
    m_gameObjectsContainer.clear();
}

std::shared_ptr<CCamera> CSceneFabricator::CreateCamera(f32 _fov, f32 _near, f32 _far,const glm::vec4& _viewport)
{
    std::shared_ptr<CCamera> camera = std::make_shared<CCamera>(_fov, _near, _far, _viewport);
    m_camerasContainer.insert(camera);
    return camera;
}

void CSceneFabricator::DeleteCamera(std::shared_ptr<CCamera> _camera)
{
    m_camerasContainer.erase(_camera);
}

std::shared_ptr<CLight> CSceneFabricator::CreateLight(void)
{
    std::shared_ptr<CLight> light = std::make_shared<CLight>();
    m_lightsContainer.insert(light);
    return light;
}

void CSceneFabricator::DeleteLight(std::shared_ptr<CLight> _light)
{
    m_lightsContainer.erase(_light);
}

std::shared_ptr<CSprite> CSceneFabricator::CreateSprite(const std::string& _filename)
{
    return nullptr;
}

void CSceneFabricator::DeleteSprite(std::shared_ptr<CSprite> _sprite)
{
    
}

std::shared_ptr<CModel> CSceneFabricator::CreateModel(const std::string& _filename)
{
    assert(m_resourceAccessor != nullptr);
    std::shared_ptr<CModel> model = std::make_shared<CModel>(m_resourceAccessor);
    assert(m_templateAccessor != nullptr);
    m_templateAccessor->LoadModelTemplate(_filename, model);
    m_gameObjectsContainer.insert(model);
    return model;
}

void CSceneFabricator::DeleteModel(std::shared_ptr<CModel> _model)
{
    m_gameObjectsContainer.erase(_model);
}

std::shared_ptr<CBillboard> CSceneFabricator::CreateBillboard(const std::string& _filename)
{
    return nullptr;
}

void CSceneFabricator::DeleteBillboard(std::shared_ptr<CBillboard> _billboard)
{
    
}

std::shared_ptr<CParticleEmitter> CSceneFabricator::CreateParticleEmitter(const std::string& _filename)
{
    return nullptr;
}

void CSceneFabricator::DeleteParticleEmitter(std::shared_ptr<CParticleEmitter> _particleEmitter)
{
    
}
