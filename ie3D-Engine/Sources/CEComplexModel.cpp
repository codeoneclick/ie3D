//
//  CEComplexModel.cpp
//  ie3D-Engine
//
//  Created by sergey.sergeev on 8/6/14.
//  Copyright (c) 2014 sergey.sergeev. All rights reserved.
//

#include "CEComplexModel.h"
#include "CMesh.h"
#include "CMaterial.h"
#include "CShader.h"
#include "CCamera.h"
#include "CModel.h"

CEComplexModel::CEComplexModel(CSharedResourceAccessorRef resourceAccessor,
                               ISharedRenderTechniqueAccessorRef renderTechniqueAccessor) :
IGameObject(resourceAccessor, renderTechniqueAccessor)
{
    m_zOrder = E_GAME_OBJECT_Z_ORDER_MODEL;
}

CEComplexModel::~CEComplexModel(void)
{
    
}

void CEComplexModel::onSceneUpdate(f32)
{
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        
    }
}

void CEComplexModel::onResourceLoaded(ISharedResourceRef resource, bool success)
{
    IGameObject::onResourceLoaded(resource, success);
}

void CEComplexModel::onConfigurationLoaded(ISharedConfigurationRef configuration, bool success)
{
    std::shared_ptr<CEConfigurationComplexModel> complexModelConfiguration = std::static_pointer_cast<CEConfigurationComplexModel>(configuration);
    assert(m_resourceAccessor != nullptr);
    
    for(ui32 i = 0; i < complexModelConfiguration->getModelsConfigurationParts().size(); ++i)
    {
        std::string partName = complexModelConfiguration->getModelsConfigurationParts().at(i);
        ISharedConfiguration configuration = complexModelConfiguration->getModelsConfigurations().at(i);
        CSharedModel model = std::make_shared<CModel>(m_resourceAccessor, m_renderTechniqueAccessor);
        m_parts.insert(std::make_pair(partName, model));
        
        assert(model != nullptr);
        assert(configuration != nullptr);
        
        model->setSceneUpdateMgr(m_sceneUpdateMgr);
        model->setRenderTechniqueImporter(m_renderTechniqueImporter);
        model->setRenderTechniqueAccessor(m_renderTechniqueAccessor);
        
        model->setCamera(m_camera);
        model->setCameraFrustum(m_cameraFrustum);
        model->setGlobalLightSource(m_globalLightSource);

        model->enableRender(true);
        model->enableUpdate(true);
        
        ISharedConfigurationLoadingHandler handler = std::static_pointer_cast<IConfigurationLoadingHandler>(model);
        handler->onConfigurationLoaded(configuration, success);
    }
    
	IGameObject::enableRender(m_isNeedToRender);
    IGameObject::enableUpdate(m_isNeedToUpdate);
    
    m_status |= E_LOADING_STATUS_TEMPLATE_LOADED;
}

i32 CEComplexModel::zOrder(void)
{
    return m_zOrder;
}

bool CEComplexModel::checkOcclusion(void)
{
    return false;
}

ui32 CEComplexModel::numTriangles(void)
{
    return 0;
}

void CEComplexModel::onBind(const std::string&)
{

}

void CEComplexModel::onDraw(const std::string&)
{

}

void CEComplexModel::onUnbind(const std::string&)
{

}

void CEComplexModel::onBatch(const std::string&)
{
    
}

void CEComplexModel::setPosition(const glm::vec3 &position)
{
    IGameObject::setPosition(position);
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        for(const auto& iterator : m_parts)
        {
            assert(iterator.second != nullptr);
            iterator.second->setPosition(position);
        }
    }
}

void CEComplexModel::setRotation(const glm::vec3 &rotation)
{
    IGameObject::setRotation(rotation);
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        for(const auto& iterator : m_parts)
        {
            assert(iterator.second != nullptr);
            iterator.second->setRotation(rotation);
        }
    }
}

void CEComplexModel::setScale(const glm::vec3& scale)
{
    IGameObject::setScale(scale);
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        for(const auto& iterator : m_parts)
        {
            assert(iterator.second != nullptr);
            iterator.second->setScale(scale);
        }
    }
}
void CEComplexModel::setCamera(CSharedCameraRef camera)
{
    IGameObject::setCamera(camera);
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        for(const auto& iterator : m_parts)
        {
            assert(iterator.second != nullptr);
            iterator.second->setCamera(camera);
        }
    }
}

void CEComplexModel::setCameraFrustum(CSharedFrustumRef frustum)
{
    IGameObject::setCameraFrustum(frustum);
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        for(const auto& iterator : m_parts)
        {
            assert(iterator.second != nullptr);
            iterator.second->setCameraFrustum(frustum);
        }
    }
}

void CEComplexModel::setGlobalLightSource(CSharedGlobalLightSourceRef lightSource)
{
    IGameObject::setGlobalLightSource(lightSource);
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        for(const auto& iterator : m_parts)
        {
            assert(iterator.second != nullptr);
            iterator.second->setGlobalLightSource(lightSource);
        }
    }
}

void CEComplexModel::setRenderTechniqueImporter(ISharedRenderTechniqueImporterRef techniqueImporter)
{
    IGameObject::setRenderTechniqueImporter(techniqueImporter);
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        for(const auto& iterator : m_parts)
        {
            assert(iterator.second != nullptr);
            iterator.second->setRenderTechniqueImporter(techniqueImporter);
        }
    }
}

void CEComplexModel::setRenderTechniqueAccessor(ISharedRenderTechniqueAccessorRef techniqueAccessor)
{
    IGameObject::setRenderTechniqueAccessor(techniqueAccessor);
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        for(const auto& iterator : m_parts)
        {
            assert(iterator.second != nullptr);
            iterator.second->setRenderTechniqueAccessor(techniqueAccessor);
        }
    }
}

void CEComplexModel::setSceneUpdateMgr(CSharedSceneUpdateMgrRef sceneUpdateMgr)
{
    IGameObject::setSceneUpdateMgr(sceneUpdateMgr);
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        for(const auto& iterator : m_parts)
        {
            assert(iterator.second != nullptr);
            iterator.second->setSceneUpdateMgr(sceneUpdateMgr);
        }
    }
}

void CEComplexModel::enableRender(bool value)
{
    IGameObject::enableRender(value);
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        for(const auto& iterator : m_parts)
        {
            assert(iterator.second != nullptr);
            iterator.second->enableRender(value);
        }
    }
}

void CEComplexModel::enableUpdate(bool value)
{
    IGameObject::enableUpdate(value);
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        for(const auto& iterator : m_parts)
        {
            assert(iterator.second != nullptr);
            iterator.second->enableUpdate(value);
        }
    }
}
