//
//  CModel.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/28/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CModel.h"
#include "CMaterial.h"
#include "CShader.h"
#include "CTexture.h"
#include "CCamera.h"
#include "CLight.h"
#include "CResourceAccessor.h"
#include "CConfigurationGameObjects.h"
#include "CBatchingMgr.h"
#include "CMesh.h"
#include "CAnimationMixer.h"

CModel::CModel(CSharedResourceAccessorRef resourceAccessor,
               ISharedRenderTechniqueAccessorRef renderTechniqueAccessor) :
IGameObject(resourceAccessor, renderTechniqueAccessor),
m_animationMixer(nullptr)
{
    m_zOrder = E_GAME_OBJECT_Z_ORDER_MODEL;

	m_materialBindImposer = [this](CSharedMaterialRef material)
    {
        material->getShader()->setMatrix4x4(m_isBatching ? glm::mat4x4(1.0f) : m_matrixWorld, E_SHADER_UNIFORM_MATRIX_WORLD);
        material->getShader()->setMatrix4x4(m_camera->Get_ProjectionMatrix(), E_SHADER_UNIFORM_MATRIX_PROJECTION);
        material->getShader()->setMatrix4x4(!material->isReflecting() ? m_camera->Get_ViewMatrix() : m_camera->Get_ViewReflectionMatrix(), E_SHADER_UNIFORM_MATRIX_VIEW);
        material->getShader()->setMatrix4x4(m_camera->Get_MatrixNormal(), E_SHADER_UNIFORM_MATRIX_NORMAL);
        
        material->getShader()->setVector3(m_camera->Get_Position(), E_SHADER_UNIFORM_VECTOR_CAMERA_POSITION);
        material->getShader()->setVector4(material->getClippingPlane(), E_SHADER_UNIFORM_VECTOR_CLIP_PLANE);
        material->getShader()->setFloat(m_camera->Get_Near(), E_SHADER_UNIFORM_FLOAT_CAMERA_NEAR);
        material->getShader()->setFloat(m_camera->Get_Far(), E_SHADER_UNIFORM_FLOAT_CAMERA_FAR);
        
        material->getShader()->setInt(m_isBatching ? 0 : 1, E_SHADER_UNIFORM_INT_FLAG_01);
    };
}

CModel::~CModel(void)
{
 
}

void CModel::onSceneUpdate(f32 deltatime)
{
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        IGameObject::onSceneUpdate(deltatime);
        if(m_animationMixer != nullptr)
        {
            m_animationMixer->update(deltatime);
        }
    }
}

void CModel::onResourceLoaded(ISharedResourceRef resource, bool success)
{
    IGameObject::onResourceLoaded(resource, success);
    if(resource->getResourceClass() == E_RESOURCE_CLASS_MESH)
    {
        assert(m_animationMixer == nullptr);
        CSharedMesh mesh = std::static_pointer_cast<CMesh>(resource);
        m_animationMixer = std::make_shared<CAnimationMixer>(mesh->getSkeletonData(),
                                                             mesh->getSequenceData());
    }
}

void CModel::onConfigurationLoaded(ISharedConfigurationRef configuration, bool success)
{
    IGameObject::onConfigurationLoaded(configuration, success);
    std::shared_ptr<CConfigurationModel> modelConfiguration = std::static_pointer_cast<CConfigurationModel>(configuration);
    assert(m_resourceAccessor != nullptr);
    m_mesh = m_resourceAccessor->getMesh(modelConfiguration->getFilename());
    m_mesh->registerLoadingHandler(shared_from_this());
    assert(m_mesh != nullptr);
    m_isBatching = modelConfiguration->isBatching();
    
	IGameObject::enableRender(m_isNeedToRender);
    IGameObject::enableUpdate(m_isNeedToUpdate);
    m_status |= E_LOADING_STATUS_TEMPLATE_LOADED;
}

i32  CModel::zOrder(void)
{
    return m_zOrder;
}

bool CModel::checkOcclusion(void)
{
    return IGameObject::checkOcclusion();
}

ui32 CModel::numTriangles(void)
{
    return IGameObject::numTriangles();
}

void CModel::onBind(const std::string& mode)
{
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        assert(m_materials.find(mode) != m_materials.end());
        IGameObject::onBind(mode);
    }
}

void CModel::onDraw(const std::string& mode)
{
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        assert(m_camera != nullptr);
        assert(m_materials.find(mode) != m_materials.end());
        
        CSharedMaterial material = m_materials.find(mode)->second;
        assert(material->getShader() != nullptr);
        
        if(!m_isBatching && m_animationMixer != nullptr)
        {
            m_materialBindImposer(material);
            material->getShader()->setMatrixArray4x4(m_animationMixer->getTransformations(),
                                                     m_animationMixer->getTransformationSize(),
                                                     E_SHADER_UNIFORM_MATRIX_BONES);
            IGameObject::onDraw(mode);
        }
    }
}

void CModel::onUnbind(const std::string& mode)
{
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        assert(m_materials.find(mode) != m_materials.end());
        IGameObject::onUnbind(mode);
    }
}

void CModel::onBatch(const std::string& mode)
{
    IGameObject::onBatch(mode);
}

void CModel::setAnimation(const std::string& name)
{

}

