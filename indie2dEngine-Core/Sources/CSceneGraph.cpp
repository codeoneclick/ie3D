//
//  CSceneGraph.cpp
//  indi2dEngine
//
//  Created by Sergey Sergeev on 5/14/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CSceneGraph.h"

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
/*
void CSceneGraph::Set_Camera(std::shared_ptr<CCamera> _camera)
{
    assert(m_sceneUpdateMgr != nullptr);
    if(m_camera != nullptr)
    {
        m_sceneUpdateMgr->RemoveEventListener(m_camera);
    }
    
    m_camera = _camera;
    assert(m_updateMgr != nullptr);
    m_updateMgr->AddEventListener(m_camera);
    
    for(std::set<CShape3d*>::iterator iterator = m_shapes3dContainer.begin(); iterator != m_shapes3dContainer.end(); ++iterator)
    {
        CShape3d* shape3d = *iterator;
        shape3d->Set_Camera(m_camera);
    }
    
    for(std::set<CParticleEmitter*>::iterator iterator = m_particleEmittersContainer.begin(); iterator != m_particleEmittersContainer.end(); ++iterator)
    {
        CParticleEmitter* particleEmitter = *iterator;
        particleEmitter->Set_Camera(m_camera);
    }
}

void CSceneGraph::Set_Light(CLight* _light)
{
    m_light = _light;
    
    for(std::set<CShape3d*>::iterator iterator = m_shapes3dContainer.begin(); iterator != m_shapes3dContainer.end(); ++iterator)
    {
        CShape3d* shape3d = *iterator;
        shape3d->Set_Light(m_light);
    }
    for(std::set<CParticleEmitter*>::iterator iterator = m_particleEmittersContainer.begin(); iterator != m_particleEmittersContainer.end(); ++iterator)
    {
        CParticleEmitter* particleEmitter = *iterator;
        particleEmitter->Set_Light(m_light);
    }
}

void CSceneGraph::Set_Landscape(CLandscape* _landscape)
{
    if(m_landscape != nullptr)
    {
        m_landscape->ListenUpdateMgr(false);
        m_landscape->ListenRenderMgr(false);
        m_landscape = nullptr;
    }
    
    m_landscape = _landscape;
    
    assert(m_updateMgr != nullptr);
    assert(m_renderMgr != nullptr);
    
    m_landscape->Set_Camera(m_camera);
    m_landscape->Set_Light(m_light);
    m_landscape->Set_UpdateMgr(m_updateMgr);
    m_landscape->Set_RenderMgr(m_renderMgr);
    m_landscape->ListenUpdateMgr(true);
    m_landscape->ListenRenderMgr(true);
    
    if(m_ocean != nullptr)
    {
        m_ocean->Set_HeightmapTexture(m_landscape->Get_HeightmapTexture());
        m_landscape->Set_Clipping(glm::vec4(0.0f, 1.0f, 0.0f, m_ocean->Get_Altitude()), E_RENDER_MODE_WORLD_SPACE_REFLECTION);
        m_landscape->Set_Clipping(glm::vec4(0.0f, -1.0f, 0.0f, m_ocean->Get_Altitude()), E_RENDER_MODE_WORLD_SPACE_REFRACTION);
    }
    
    for(std::set<CLandscapeDecal*>::iterator iterator = m_landscapeDecalsContainer.begin(); iterator != m_landscapeDecalsContainer.end(); ++iterator)
    {
        CLandscapeDecal* landscapeDecal = *iterator;
        landscapeDecal->Set_HeightmapData(m_landscape->Get_HeightmapData());
        landscapeDecal->Set_HeightmapWidth(m_landscape->Get_HeightmapWidth());
        landscapeDecal->Set_HeightmapHeight(m_landscape->Get_HeightmapHeight());
    }
    
    assert(m_collisionMgr != nullptr);
    m_collisionMgr->Set_CollisionBounds(glm::vec2(m_landscape->Get_Position().x, m_landscape->Get_Position().z), glm::vec2(m_landscape->Get_HeightmapWidth(), m_landscape->Get_HeightmapHeight()));
}

void CSceneGraph::Set_Ocean(COcean *_ocean)
{
    if(m_ocean != nullptr)
    {
        m_ocean->ListenUpdateMgr(false);
        m_ocean->ListenRenderMgr(false);
        m_ocean = nullptr;
    }
    
    m_ocean = _ocean;
    
    assert(m_updateMgr != nullptr);
    assert(m_renderMgr != nullptr);
    
    m_ocean->Set_Camera(m_camera);
    m_ocean->Set_Light(m_light);
    m_ocean->Set_UpdateMgr(m_updateMgr);
    m_ocean->Set_RenderMgr(m_renderMgr);
    m_ocean->ListenUpdateMgr(true);
    m_ocean->ListenRenderMgr(true);
    
    m_ocean->Set_ReflectionTexture(m_renderMgr->Get_TextureWorldSpaceRenderMode(E_RENDER_MODE_WORLD_SPACE_REFLECTION));
    m_ocean->Set_RefractionTexture(m_renderMgr->Get_TextureWorldSpaceRenderMode(E_RENDER_MODE_WORLD_SPACE_REFRACTION));
    
    if(m_landscape != nullptr)
    {
        m_ocean->Set_HeightmapTexture(m_landscape->Get_HeightmapTexture());
        m_landscape->Set_Clipping(glm::vec4(0.0f, 1.0f, 0.0f, m_ocean->Get_Altitude()), E_RENDER_MODE_WORLD_SPACE_REFLECTION);
        m_landscape->Set_Clipping(glm::vec4(0.0f, -1.0f, 0.0f, m_ocean->Get_Altitude()), E_RENDER_MODE_WORLD_SPACE_REFRACTION);
    }
    
    for(std::set<CShape3d*>::iterator iterator = m_shapes3dContainer.begin(); iterator != m_shapes3dContainer.end(); ++iterator)
    {
        CShape3d* shape3d = *iterator;
        shape3d->Set_Clipping(glm::vec4(0.0f, 1.0f, 0.0f, m_ocean->Get_Altitude()), E_RENDER_MODE_WORLD_SPACE_REFLECTION);
        shape3d->Set_Clipping(glm::vec4(0.0f, -1.0f, 0.0f, m_ocean->Get_Altitude()), E_RENDER_MODE_WORLD_SPACE_REFRACTION);
    }
}

void CSceneGraph::InsertShape3d(CShape3d *_shape3d)
{
    CShape3d* shape3d = _shape3d;
    
    if(m_camera != nullptr)
    {
        shape3d->Set_Camera(m_camera);
    }
    
    if(m_light != nullptr)
    {
        shape3d->Set_Light(m_light);
    }
    
    assert(m_updateMgr != nullptr);
    assert(m_renderMgr != nullptr);
    
    shape3d->Set_UpdateMgr(m_updateMgr);
    shape3d->Set_RenderMgr(m_renderMgr);
    shape3d->ListenUpdateMgr(true);
    shape3d->ListenRenderMgr(true);
    
    if(m_ocean != nullptr)
    {
        shape3d->Set_Clipping(glm::vec4(0.0f, 1.0f, 0.0f, m_ocean->Get_Altitude()), E_RENDER_MODE_WORLD_SPACE_REFLECTION);
        shape3d->Set_Clipping(glm::vec4(0.0f, -1.0f, 0.0f, m_ocean->Get_Altitude()), E_RENDER_MODE_WORLD_SPACE_REFRACTION);
    }
    
    m_shapes3dContainer.insert(shape3d);
}

void CSceneGraph::RemoveShape3d(CShape3d *_shape3d)
{
    CShape3d* shape3d = _shape3d;
    shape3d->ListenUpdateMgr(false);
    shape3d->ListenRenderMgr(false);
    m_shapes3dContainer.erase(shape3d);
}

void CSceneGraph::InsertParticleEmitter(CParticleEmitter* _particleEmitter)
{
    CParticleEmitter* particleEmitter = _particleEmitter;
    
    if(m_camera != nullptr)
    {
        particleEmitter->Set_Camera(m_camera);
    }
    
    if(m_light != nullptr)
    {
        particleEmitter->Set_Light(m_light);
    }
    
    assert(m_updateMgr != nullptr);
    assert(m_renderMgr != nullptr);
    
    particleEmitter->Set_UpdateMgr(m_updateMgr);
    particleEmitter->Set_RenderMgr(m_renderMgr);
    particleEmitter->ListenUpdateMgr(true);
    particleEmitter->ListenRenderMgr(true);
    
    m_particleEmittersContainer.insert(particleEmitter);
}

void CSceneGraph::RemoveParticleEmitter(CParticleEmitter* _particleEmitter)
{
    CParticleEmitter* particleEmitter = _particleEmitter;
    particleEmitter->ListenUpdateMgr(false);
    particleEmitter->ListenRenderMgr(false);
    m_particleEmittersContainer.erase(particleEmitter);
}

void CSceneGraph::InsertLandscapeDecal(CLandscapeDecal* _landscapeDecal)
{
    CLandscapeDecal* landscapeDecal = _landscapeDecal;
    
    if(m_camera != nullptr)
    {
        landscapeDecal->Set_Camera(m_camera);
    }
    
    if(m_light != nullptr)
    {
        landscapeDecal->Set_Light(m_light);
    }
    
    if(m_landscape != nullptr)
    {
        landscapeDecal->Set_HeightmapData(m_landscape->Get_HeightmapData());
        landscapeDecal->Set_HeightmapWidth(m_landscape->Get_HeightmapWidth());
        landscapeDecal->Set_HeightmapHeight(m_landscape->Get_HeightmapHeight());
    }
    
    assert(m_updateMgr != nullptr);
    assert(m_renderMgr != nullptr);
    
    landscapeDecal->Set_UpdateMgr(m_updateMgr);
    landscapeDecal->Set_RenderMgr(m_renderMgr);
    landscapeDecal->ListenUpdateMgr(true);
    landscapeDecal->ListenRenderMgr(true);
    
    m_landscapeDecalsContainer.insert(landscapeDecal);
}

void CSceneGraph::RemoveLandscapeDecal(CLandscapeDecal* _landscapeDecal)
{
    CLandscapeDecal* landscapeDecal = _landscapeDecal;
    landscapeDecal->ListenUpdateMgr(false);
    landscapeDecal->ListenRenderMgr(false);
    m_landscapeDecalsContainer.erase(landscapeDecal);
}

void CSceneGraph::InsertCollider(CCollisionCallback_INTERFACE* _collider, bool _isStatic)
{
    assert(m_collisionMgr != nullptr);
    m_collisionMgr->AddCollisionEventListener(_collider, _isStatic);
}

void CSceneGraph::RemoveCollider(CCollisionCallback_INTERFACE* _collider)
{
    assert(m_collisionMgr != nullptr);
    m_collisionMgr->RemoveCollisionEventListener(_collider);
}

void CSceneGraph::FillUIView(IUIView_INTERFACE *_view, const std::string &_filename)
{
    assert(m_uiMgr != nullptr);
    m_uiMgr->FillUIView(_view, _filename);
}

void CSceneGraph::AddUIEventListener(CUIEventCallback_INTERFACE* _listener)
{
    assert(m_uiMgr != nullptr);
    m_uiMgr->AddUIEventListener(_listener);
}

void CSceneGraph::RemoveUIEventListener(CUIEventCallback_INTERFACE* _listener)
{
    assert(m_uiMgr != nullptr);
    m_uiMgr->RemoveUIEventListener(_listener);
}*/