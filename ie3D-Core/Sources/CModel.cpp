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
#include "CGlobalLightSource.h"
#include "CResourceAccessor.h"
#include "CConfigurationGameObjects.h"
#include "CBatchingMgr.h"
#include "CMesh.h"
#include "CAnimationMixer.h"

CModel::CModel(CSharedResourceAccessorRef resourceAccessor,
               ISharedRenderTechniqueAccessorRef renderTechniqueAccessor) :
IGameObject(resourceAccessor, renderTechniqueAccessor),
m_animationMixer(nullptr),
m_isAnimated(false)
{
    m_zOrder = E_GAME_OBJECT_Z_ORDER_MODEL;
}

CModel::~CModel(void)
{
 
}

void CModel::onSceneUpdate(f32 deltatime)
{
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        IGameObject::onSceneUpdate(deltatime);
        if(m_animationMixer != nullptr && m_isAnimated)
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
        m_isAnimated = mesh->getSkeletonData() != nullptr && mesh->getSequenceData() != nullptr;
        if(m_isAnimated)
        {
            m_animationMixer = std::make_shared<CAnimationMixer>(mesh->getSkeletonData(),
                                                                 mesh->getSequenceData());
        }
    }
}

void CModel::onConfigurationLoaded(ISharedConfigurationRef configuration, bool success)
{
    IGameObject::onConfigurationLoaded(configuration, success);
    std::shared_ptr<CConfigurationModel> modelConfiguration = std::static_pointer_cast<CConfigurationModel>(configuration);
    assert(m_resourceAccessor != nullptr);
    m_mesh = m_resourceAccessor->getMesh(modelConfiguration->getFilename());
    m_mesh->addLoadingHandler(shared_from_this());
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
        IGameObject::onBind(mode);
    }
}

void CModel::onDraw(const std::string& mode)
{
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        IGameObject::onDraw(mode);
    }
}

void CModel::onUnbind(const std::string& mode)
{
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        IGameObject::onUnbind(mode);
    }
}

void CModel::onBatch(const std::string& mode)
{
    IGameObject::onBatch(mode);
}

void CModel::bindCustomShaderUniforms(CSharedMaterialRef material)
{
    IGameObject::bindCustomShaderUniforms(material);
    material->getShader()->setInt(m_isAnimated ? 1 : 0, E_SHADER_UNIFORM_INT_FLAG_01);
    if(!m_isBatching && m_animationMixer != nullptr && m_isAnimated)
    {
        material->getShader()->setMatrixArray4x4(m_animationMixer->getTransformations(),
                                                 m_animationMixer->getTransformationSize(),
                                                 E_SHADER_UNIFORM_MATRIX_BONES);
    }
}

void CModel::setAnimation(const std::string& name)
{
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED &&
       m_animationMixer != nullptr)
    {
        m_animationMixer->setAnimation(name);
    }
}

