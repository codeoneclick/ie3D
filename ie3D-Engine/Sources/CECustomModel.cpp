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
        assert(m_materials.find(mode) != m_materials.end());
        IGameObject::onBind(mode);
    }
}

void CECustomModel::onDraw(const std::string& mode)
{
    if((m_status & E_LOADING_STATUS_TEMPLATE_LOADED) &&
       m_mesh != nullptr)
    {
        assert(m_camera != nullptr);
        assert(m_materials.find(mode) != m_materials.end());
        
        CSharedMaterial material = m_materials.find(mode)->second;
        assert(material->getShader() != nullptr);
        
        material->getShader()->setMatrix4x4(m_isBatching ? glm::mat4x4(1.0f) : m_matrixWorld, E_SHADER_UNIFORM_MATRIX_WORLD);
        material->getShader()->setMatrix4x4(m_camera->Get_ProjectionMatrix(), E_SHADER_UNIFORM_MATRIX_PROJECTION);
        material->getShader()->setMatrix4x4(!material->isReflecting() ? m_camera->Get_ViewMatrix() : m_camera->Get_ViewReflectionMatrix(), E_SHADER_UNIFORM_MATRIX_VIEW);
        material->getShader()->setMatrix4x4(m_camera->Get_MatrixNormal(), E_SHADER_UNIFORM_MATRIX_NORMAL);
        
        material->getShader()->setVector3(m_camera->Get_Position(), E_SHADER_UNIFORM_VECTOR_CAMERA_POSITION);
        material->getShader()->setVector4(material->getClippingPlane(), E_SHADER_UNIFORM_VECTOR_CLIP_PLANE);
        material->getShader()->setFloat(m_camera->Get_Near(), E_SHADER_UNIFORM_FLOAT_CAMERA_NEAR);
        material->getShader()->setFloat(m_camera->Get_Far(), E_SHADER_UNIFORM_FLOAT_CAMERA_FAR);
        
        material->getShader()->setMatrix4x4(m_globalLightSource->getProjectionMatrix(),
                                            E_SHADER_UNIFORM_MATRIX_GLOBAL_LIGHT_PROJECTION);
        material->getShader()->setMatrix4x4(m_globalLightSource->getViewMatrix(),
                                            E_SHADER_UNIFORM_MATRIX_GLOBAL_LIGHT_VIEW);
        IGameObject::onDraw(mode);
    }
}

void CECustomModel::onUnbind(const std::string& mode)
{
    if((m_status & E_LOADING_STATUS_TEMPLATE_LOADED) &&
       m_mesh != nullptr)
    {
        assert(m_materials.find(mode) != m_materials.end());
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