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
#include "CModel.h"
#include "COcean.h"
#include "CSkyBox.h"
#include "CLandscape.h"
#include "CParticleEmitter.h"
#include "IInputContext.h"

CSceneGraph::CSceneGraph(void) :
m_camera(nullptr),
m_oceansContainer(nullptr),
m_landscapeContainer(nullptr),
m_collisionMgr(nullptr),
m_inputContext(nullptr)
{
    /*std::for_each(m_lights.begin(), m_lights.end(), [](std::shared_ptr<CLight> _light)
                  {
                      _light = nullptr;
                  });*/
}

CSceneGraph::~CSceneGraph(void)
{
    m_spritesContainer.clear();
    m_billboardsContainer.clear();
    m_particlesContainer.clear();
}

void CSceneGraph::Set_Camera(const std::shared_ptr<CCamera>& _camera)
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
        iterator->setCamera(m_camera);
    }
    
    assert(m_collisionMgr != nullptr);
    m_collisionMgr->setCamera(m_camera);
}

/*void CSceneGraph::Set_Light(const std::shared_ptr<CLight>& _light, E_LIGHTS _id)
{
   m_lights[_id] = _light;
    
    std::for_each(m_gameObjectsContainer.begin(), m_gameObjectsContainer.end(), [_light, _id](std::shared_ptr<IGameObject> _gameObject)
                  {
                      _gameObject->Set_Light(_light, _id);
                  });
}
*/
void CSceneGraph::_InsertGameObject(const std::shared_ptr<IGameObject>& _gameObject)
{
    if(m_camera != nullptr)
    {
        _gameObject->setCamera(m_camera);
    }
    
    /*for(ui32 i = 0; i < E_LIGHT_MAX; ++i)
    {
        if(m_lights[i] != nullptr)
        {
            _gameObject->Set_Light(m_lights[i], static_cast<E_LIGHTS>(i));
        }
    }*/
    
    assert(m_sceneUpdateMgr != nullptr);
    assert(m_renderMgr != nullptr);
    
    _gameObject->setSceneUpdateMgr(m_sceneUpdateMgr);
    _gameObject->setRenderMgr(m_renderMgr);
    _gameObject->listenSceneUpdateMgr(true);
    _gameObject->listenRenderMgr(true);
    m_gameObjectsContainer.insert(_gameObject);
}

void CSceneGraph::_RemoveGameObject(const std::shared_ptr<IGameObject>& _gameObject)
{
    assert(m_sceneUpdateMgr != nullptr);
    assert(m_renderMgr != nullptr);
    
    _gameObject->setSceneUpdateMgr(nullptr);
    _gameObject->setRenderMgr(nullptr);
    _gameObject->listenSceneUpdateMgr(false);
    _gameObject->listenRenderMgr(false);
    m_gameObjectsContainer.erase(_gameObject);
}

void CSceneGraph::InsertSprite(const std::shared_ptr<CSprite>& _sprite)
{
    //CSceneGraph::_InsertGameObject(_sprite);
    //m_spritesContainer.insert(_sprite);
}

void CSceneGraph::RemoveSprite(const std::shared_ptr<CSprite>& _sprite)
{
    //CSceneGraph::_RemoveGameObject(_sprite);
    //m_spritesContainer.erase(_sprite);
}

void CSceneGraph::InsertModel(const std::shared_ptr<CModel>& _model)
{
    CSceneGraph::_InsertGameObject(_model);
    m_modelsContainer.insert(_model);
}

void CSceneGraph::RemoveModel(const std::shared_ptr<CModel>& _model)
{
    CSceneGraph::_RemoveGameObject(_model);
    m_modelsContainer.erase(_model);
}

void CSceneGraph::InsertOcean(const std::shared_ptr<COcean>& _ocean)
{
    CSceneGraph::_InsertGameObject(_ocean);
    m_oceansContainer = _ocean;
}

void CSceneGraph::RemoveOcean(const std::shared_ptr<COcean>& _ocean)
{
    CSceneGraph::_RemoveGameObject(_ocean);
    m_oceansContainer = nullptr;
}

void CSceneGraph::InsertSkyBox(CSharedSkyBoxRef skybox)
{
    CSceneGraph::_InsertGameObject(skybox);
}

void CSceneGraph::RemoveSkyBox(CSharedSkyBoxRef skybox)
{
    CSceneGraph::_RemoveGameObject(skybox);
}

void CSceneGraph::InsertLandscape(const std::shared_ptr<CLandscape> &_landscape)
{
    CSceneGraph::_InsertGameObject(_landscape);
    m_landscapeContainer = _landscape;
    
    
}

void CSceneGraph::RemoveLandscape(const std::shared_ptr<CLandscape> &_landscape)
{
    CSceneGraph::_RemoveGameObject(_landscape);
    m_landscapeContainer = nullptr;
}

void CSceneGraph::InsertBillboard(const std::shared_ptr<CBillboard>& _billboard)
{
    //CSceneGraph::_InsertGameObject(_billboard);
    //m_billboardsContainer.insert(_billboard);
}

void CSceneGraph::RemoveBillboard(const std::shared_ptr<CBillboard>& _billboard)
{
    //CSceneGraph::_RemoveGameObject(_billboard);
    //m_billboardsContainer.erase(_billboard);
}

void CSceneGraph::InsertParticleEmitter(const std::shared_ptr<CParticleEmitter>& _particleEmitter)
{
    CSceneGraph::_InsertGameObject(_particleEmitter);
    m_particlesContainer.insert(_particleEmitter);
}

void CSceneGraph::RemoveParticleEmitter(const std::shared_ptr<CParticleEmitter>& _particleEmitter)
{
    CSceneGraph::_RemoveGameObject(_particleEmitter);
    m_particlesContainer.erase(_particleEmitter);
}

void CSceneGraph::addCollisionHandler(ISharedCollisionHandlerRef handler)
{
    assert(m_collisionMgr != nullptr);
    m_collisionMgr->addCollisionHandler(handler);
}

void CSceneGraph::removeCollisionHandler(ISharedCollisionHandlerRef handler)
{
    assert(m_collisionMgr != nullptr);
    m_collisionMgr->removeCollisionHandler(handler);
}

void CSceneGraph::addGestureRecognizerHandler(ISharedGestureRecognizerHandlerRef handler)
{
    assert(m_inputContext != nullptr);
    m_inputContext->addGestureRecognizerHandler(handler);
}

void CSceneGraph::removeGestureRecognizerHandler(ISharedGestureRecognizerHandlerRef handler)
{
    assert(m_inputContext != nullptr);
    m_inputContext->removeGestureRecognizerHandler(handler);
}
