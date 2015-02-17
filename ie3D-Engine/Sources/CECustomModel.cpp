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
    m_status |= E_LOADING_STATUS_TEMPLATE_LOADED;
}

void CECustomModel::onDraw(CSharedMaterialRef material)
{
    if((m_status & E_LOADING_STATUS_TEMPLATE_LOADED) &&
       m_mesh != nullptr)
    {
        IGameObject::onDraw(material);
    }
}

void CECustomModel::setMesh(CSharedMeshRef mesh)
{
    m_mesh = mesh;
}
