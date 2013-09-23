//
//  CSceneGraph.cpp
//  indi2dEngine
//
//  Created by Sergey Sergeev on 5/14/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CSceneGraph.h"
#include "CSceneUpdateMgr.h"
#include "CCollisionMgr.h"
#include "CCamera.h"
#include "IGameObject.h"
#include "CSprite.h"
#include "CModel.h"
#include "COcean.h"
#include "CBillboard.h"
#include "CParticleEmitter.h"
#include "ICollisionHandler.h"

CSceneGraph::CSceneGraph(void) :
m_camera(nullptr),
m_renderMgr(nullptr),
m_sceneUpdateMgr(nullptr),
m_collisionMgr(nullptr),
m_inputContext(nullptr)
{
    for(ui32 i = 0; i < E_LIGHT_MAX; ++i)
    {
        m_lights[i] = nullptr;
    }
}

CSceneGraph::~CSceneGraph(void)
{
    m_spritesContainer.clear();
    m_billboardsContainer.clear();
    m_particlesContainer.clear();
}

void CSceneGraph::Set_Camera(std::shared_ptr<CCamera> _camera)
{
    assert(m_sceneUpdateMgr != nullptr);
    if(m_camera != nullptr)
    {
        m_sceneUpdateMgr->UnregisterSceneUpdateHandler(m_camera);
    }
    
    m_camera = _camera;
    assert(m_sceneUpdateMgr != nullptr);
    m_sceneUpdateMgr->RegisterSceneUpdateHandler(m_camera);
    
    for(const auto& iterator : m_gameObjectsContainer)
    {
        iterator->Set_Camera(m_camera);
    }
    
    assert(m_collisionMgr != nullptr);
    m_collisionMgr->Set_Camera(m_camera);
}

void CSceneGraph::Set_Light(std::shared_ptr<CLight> _light, E_LIGHTS _id)
{
    m_lights[_id] = _light;
    
    for(const auto& iterator : m_gameObjectsContainer)
    {
        iterator->Set_Light(_light, _id);
    }
}

void CSceneGraph::_InsertGameObject(std::shared_ptr<IGameObject> _gameObject)
{
    if(m_camera != nullptr)
    {
        _gameObject->Set_Camera(m_camera);
    }
    
    for(ui32 i = 0; i < E_LIGHT_MAX; ++i)
    {
        if(m_lights[i] != nullptr)
        {
            _gameObject->Set_Light(m_lights[i], static_cast<E_LIGHTS>(i));
        }
    }
    
    assert(m_sceneUpdateMgr != nullptr);
    assert(m_renderMgr != nullptr);
    
    _gameObject->Set_SceneUpdateMgr(m_sceneUpdateMgr);
    _gameObject->Set_RenderMgr(m_renderMgr);
    _gameObject->ListenSceneUpdateMgr(true);
    _gameObject->ListenRenderMgr(true);
    m_gameObjectsContainer.insert(_gameObject);
}

void CSceneGraph::_RemoveGameObject(std::shared_ptr<IGameObject> _gameObject)
{
    assert(m_sceneUpdateMgr != nullptr);
    assert(m_renderMgr != nullptr);
    
    _gameObject->Set_SceneUpdateMgr(nullptr);
    _gameObject->Set_RenderMgr(nullptr);
    _gameObject->ListenSceneUpdateMgr(false);
    _gameObject->ListenRenderMgr(false);
    m_gameObjectsContainer.erase(_gameObject);
}

void CSceneGraph::InsertSprite(std::shared_ptr<CSprite> _sprite)
{
    CSceneGraph::_InsertGameObject(_sprite);
    m_spritesContainer.insert(_sprite);
}

void CSceneGraph::RemoveSprite(std::shared_ptr<CSprite> _sprite)
{
    CSceneGraph::_RemoveGameObject(_sprite);
    m_spritesContainer.erase(_sprite);
}

void CSceneGraph::InsertModel(std::shared_ptr<CModel> _model)
{
    CSceneGraph::_InsertGameObject(_model);
    m_modelsContainer.insert(_model);
}

void CSceneGraph::RemoveModel(std::shared_ptr<CModel> _model)
{
    CSceneGraph::_RemoveGameObject(_model);
    m_modelsContainer.erase(_model);
}

void CSceneGraph::InsertOcean(std::shared_ptr<COcean> _ocean)
{
    CSceneGraph::_InsertGameObject(_ocean);
    m_oceansContainer.insert(_ocean);
}

void CSceneGraph::RemoveOcean(std::shared_ptr<COcean> _ocean)
{
    CSceneGraph::_RemoveGameObject(_ocean);
    m_oceansContainer.erase(_ocean);
}

void CSceneGraph::InsertBillboard(std::shared_ptr<CBillboard> _billboard)
{
    CSceneGraph::_InsertGameObject(_billboard);
    m_billboardsContainer.insert(_billboard);
}

void CSceneGraph::RemoveBillboard(std::shared_ptr<CBillboard> _billboard)
{
    CSceneGraph::_RemoveGameObject(_billboard);
    m_billboardsContainer.erase(_billboard);
}


void CSceneGraph::InsertParticleEmitter(std::shared_ptr<CParticleEmitter> _particleEmitter)
{
    CSceneGraph::_InsertGameObject(_particleEmitter);
    m_particlesContainer.insert(_particleEmitter);
}

void CSceneGraph::RemoveParticleEmitter(std::shared_ptr<CParticleEmitter> _particleEmitter)
{
    CSceneGraph::_RemoveGameObject(_particleEmitter);
    m_particlesContainer.erase(_particleEmitter);
}

void CSceneGraph::RegisterCollisionHandler(std::shared_ptr<ICollisionHandler> _handler)
{
    assert(m_collisionMgr != nullptr);
    m_collisionMgr->RegisterCollisionHandler(_handler);
}

void CSceneGraph::UnregisterCollisionHandler(std::shared_ptr<ICollisionHandler> _handler)
{
    assert(m_collisionMgr != nullptr);
    m_collisionMgr->UnregisterCollisionHandler(_handler);
}
