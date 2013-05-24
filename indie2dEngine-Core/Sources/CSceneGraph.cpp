//
//  CSceneGraph.cpp
//  indi2dEngine
//
//  Created by Sergey Sergeev on 5/14/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CSceneGraph.h"
#include "CSceneUpdateMgr.h"
#include "CCamera.h"
#include "CGameObject.h"
#include "CSprite.h"
#include "CBillboard.h"
#include "CParticleEmitter.h"

CSceneGraph::CSceneGraph(void) :
m_camera(nullptr),
m_light(nullptr),
m_renderMgr(nullptr),
m_sceneUpdateMgr(nullptr),
m_collisionMgr(nullptr)
{

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
    
    for(auto iterator : m_gameObjectsContainer)
    {
        iterator->Set_Camera(m_camera);
    }
}

void CSceneGraph::Set_Light(std::shared_ptr<CLight> _light)
{
    m_light = _light;
    
    for(auto iterator : m_gameObjectsContainer)
    {
        iterator->Set_Light(m_light);
    }
}

void CSceneGraph::_InsertGameObject(std::shared_ptr<CGameObject> _gameObject)
{
    if(m_camera != nullptr)
    {
        _gameObject->Set_Camera(m_camera);
    }
    
    if(m_light != nullptr)
    {
        _gameObject->Set_Light(m_light);
    }
    
    assert(m_sceneUpdateMgr != nullptr);
    assert(m_renderMgr != nullptr);
    
    _gameObject->Set_SceneUpdateMgr(m_sceneUpdateMgr);
    _gameObject->Set_RenderMgr(m_renderMgr);
    _gameObject->ListenSceneUpdateMgr(true);
    _gameObject->ListenRenderMgr(true);
    m_gameObjectsContainer.insert(_gameObject);
}

void CSceneGraph::_RemoveGameObject(std::shared_ptr<CGameObject> _gameObject)
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
