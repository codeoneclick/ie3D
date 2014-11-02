//
//  CCustomModel.cpp
//  ie3D-Engine
//
//  Created by sergey.sergeev on 10/27/14.
//  Copyright (c) 2014 sergey.sergeev. All rights reserved.
//

#include "CECustomModel.h"
#include "CMaterial.h"
#include "CShader.h"
#include "CCamera.h"
#include "CGlobalLightSource.h"

CECustomModel::CECustomModel(CSharedResourceAccessorRef resourceAccessor,
                             ISharedRenderTechniqueAccessorRef renderTechniqueAccessor) :
IGameObject(resourceAccessor, renderTechniqueAccessor)
{
    m_zOrder = E_GAME_OBJECT_Z_ORDER_CUSTOM;
}

CECustomModel::~CECustomModel(void)
{
    
}

void CECustomModel::onSceneUpdate(f32 deltatime)
{
    if((m_status & E_LOADING_STATUS_TEMPLATE_LOADED) &&
       m_mesh != nullptr)
    {
        IGameObject::onSceneUpdate(deltatime);
    }
}

void CECustomModel::onResourceLoaded(ISharedResourceRef resource, bool success)
{
    IGameObject::onResourceLoaded(resource, success);
}

void CECustomModel::onConfigurationLoaded(ISharedConfigurationRef configuration, bool success)
{
    IGameObject::onConfigurationLoaded(configuration, success);
    
    IGameObject::enableRender(m_isNeedToRender);
    IGameObject::enableUpdate(m_isNeedToUpdate);
    m_status |= E_LOADING_STATUS_TEMPLATE_LOADED;
}

i32 CECustomModel::zOrder(void)
{
    return m_zOrder;
}

bool CECustomModel::checkOcclusion(void)
{
    return IGameObject::checkOcclusion();
}

ui32 CECustomModel::numTriangles(void)
{
    return IGameObject::numTriangles();
}

void CECustomModel::onBind(const std::string& mode)
{
    if((m_status & E_LOADING_STATUS_TEMPLATE_LOADED) &&
       m_mesh != nullptr)
    {
        IGameObject::onBind(mode);
    }
}

void CECustomModel::onDraw(const std::string& mode)
{
    if((m_status & E_LOADING_STATUS_TEMPLATE_LOADED) &&
       m_mesh != nullptr)
    {
        IGameObject::onDraw(mode);
    }
}

void CECustomModel::onUnbind(const std::string& mode)
{
    if((m_status & E_LOADING_STATUS_TEMPLATE_LOADED) &&
       m_mesh != nullptr)
    {
        IGameObject::onUnbind(mode);
    }
}

void CECustomModel::onBatch(const std::string& mode)
{
    IGameObject::onBatch(mode);
}

void CECustomModel::setMesh(CSharedMeshRef mesh)
{
    m_mesh = mesh;
}
