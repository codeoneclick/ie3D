//
//  CMETankComplex.cpp
//  ie3D-EMap
//
//  Created by sergey.sergeev on 7/23/14.
//
//

#include "CMETankComplex.h"
#include "CMEConfigurationGameObjects.h"
#include "CLandscape.h"
#include "CMesh.h"
#include "CMaterial.h"
#include "CShader.h"
#include "CCamera.h"
#include "CModel.h"

CMETankComplex::CMETankComplex(CSharedResourceAccessorRef resourceAccessor,
                               ISharedRenderTechniqueAccessorRef renderTechniqueAccessor) :
IGameObject(resourceAccessor, renderTechniqueAccessor),
kTankPartBody("body"),
kTankPartTower("tower"),
kTankPartLeftTrack("left_track"),
kTankPartRightTrack("right_track")
{
    m_zOrder = E_GAME_OBJECT_Z_ORDER_MODEL;
    
    for(ui32 i = 0; i < m_parts.size(); ++i)
    {
        m_parts.at(i) = nullptr;
    }
}

CMETankComplex::~CMETankComplex(void)
{
    
}

void CMETankComplex::onSceneUpdate(f32)
{
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
    }
}

void CMETankComplex::onResourceLoaded(ISharedResourceRef resource, bool success)
{
    IGameObject::onResourceLoaded(resource, success);
}

void CMETankComplex::onConfigurationLoaded(ISharedConfigurationRef configuration, bool success)
{
    std::shared_ptr<CMEConfigurationTankComplex> tankComplexConfiguration = std::static_pointer_cast<CMEConfigurationTankComplex>(configuration);
    assert(m_resourceAccessor != nullptr);
    
    for(ui32 i = 0; i < tankComplexConfiguration->getModelsConfigurationParts().size(); ++i)
    {
        std::string partName = tankComplexConfiguration->getModelsConfigurationParts().at(i);
        ISharedConfiguration configuration = tankComplexConfiguration->getModelsConfigurations().at(i);
        CSharedModel model = nullptr;
        if(partName == kTankPartBody)
        {
            assert(m_parts.at(E_ME_TANK_PART_BODY) == nullptr);
            m_parts.at(E_ME_TANK_PART_BODY) = std::make_shared<CModel>(m_resourceAccessor, m_renderTechniqueAccessor);
            model = m_parts.at(E_ME_TANK_PART_BODY);
        }
        else if(partName == kTankPartTower)
        {
            assert(m_parts.at(E_ME_TANK_PART_TOWER) == nullptr);
            m_parts.at(E_ME_TANK_PART_TOWER) = std::make_shared<CModel>(m_resourceAccessor, m_renderTechniqueAccessor);
            model = m_parts.at(E_ME_TANK_PART_TOWER);
        }
        else if(partName == kTankPartLeftTrack)
        {
            assert(m_parts.at(E_ME_TANK_PART_LEFT_TRACK) == nullptr);
            m_parts.at(E_ME_TANK_PART_LEFT_TRACK) = std::make_shared<CModel>(m_resourceAccessor, m_renderTechniqueAccessor);
            model = m_parts.at(E_ME_TANK_PART_LEFT_TRACK);
        }
        else if(partName == kTankPartRightTrack)
        {
            assert(m_parts.at(E_ME_TANK_PART_RIGHT_TRACK) == nullptr);
            m_parts.at(E_ME_TANK_PART_RIGHT_TRACK) = std::make_shared<CModel>(m_resourceAccessor, m_renderTechniqueAccessor);
            model = m_parts.at(E_ME_TANK_PART_RIGHT_TRACK);
        }
        else
        {
            assert(false);
        }
        
        assert(model != nullptr);
        assert(configuration != nullptr);
        
        model->setSceneUpdateMgr(m_sceneUpdateMgr);
        model->setRenderTechniqueImporter(m_renderTechniqueImporter);
        model->setRenderTechniqueAccessor(m_renderTechniqueAccessor);
        
        model->setCamera(m_camera);
        
        model->enableRender(true);
        model->enableUpdate(true);
        
        ISharedConfigurationLoadingHandler handler = std::static_pointer_cast<IConfigurationLoadingHandler>(model);
        handler->onConfigurationLoaded(configuration, success);
    }
    
	IGameObject::enableRender(m_isNeedToRender);
    IGameObject::enableUpdate(m_isNeedToUpdate);
    
    m_status |= E_LOADING_STATUS_TEMPLATE_LOADED;
}

i32 CMETankComplex::zOrder(void)
{
    return m_zOrder;
}

bool CMETankComplex::checkOcclusion(void)
{
    return false;
}

ui32 CMETankComplex::numTriangles(void)
{
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
    }
    return 0;
}

void CMETankComplex::onBind(const std::string&)
{
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
    }
}

void CMETankComplex::onDraw(const std::string&)
{
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
    }
}

void CMETankComplex::onUnbind(const std::string&)
{
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
    }
}

void CMETankComplex::onBatch(const std::string&)
{

}

void CMETankComplex::setPosition(const glm::vec3 &position)
{
    IGameObject::setPosition(position);
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        for(ui32 i = 0; i < m_parts.size(); ++i)
        {
            assert(m_parts.at(i) != nullptr);
            m_parts.at(i)->setPosition(position);
        }
    }
}

void CMETankComplex::setRotation(const glm::vec3 &rotation)
{
    IGameObject::setRotation(rotation);
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        for(ui32 i = 0; i < m_parts.size(); ++i)
        {
            assert(m_parts.at(i) != nullptr);
            m_parts.at(i)->setRotation(rotation);
        }
    }
}

void CMETankComplex::setScale(const glm::vec3& scale)
{
    IGameObject::setScale(scale);
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        for(ui32 i = 0; i < m_parts.size(); ++i)
        {
            assert(m_parts.at(i) != nullptr);
            m_parts.at(i)->setScale(scale);
        }
    }
}
void CMETankComplex::setCamera(CSharedCameraRef camera)
{
    IGameObject::setCamera(camera);
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        for(ui32 i = 0; i < m_parts.size(); ++i)
        {
            assert(m_parts.at(i) != nullptr);
            m_parts.at(i)->setCamera(camera);
        }
    }
}

void CMETankComplex::setRenderTechniqueImporter(ISharedRenderTechniqueImporterRef techniqueImporter)
{
    IGameObject::setRenderTechniqueImporter(techniqueImporter);
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        for(ui32 i = 0; i < m_parts.size(); ++i)
        {
            assert(m_parts.at(i) != nullptr);
            m_parts.at(i)->setRenderTechniqueImporter(techniqueImporter);
        }
    }
}

void CMETankComplex::setRenderTechniqueAccessor(ISharedRenderTechniqueAccessorRef techniqueAccessor)
{
    IGameObject::setRenderTechniqueAccessor(techniqueAccessor);
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        for(ui32 i = 0; i < m_parts.size(); ++i)
        {
            assert(m_parts.at(i) != nullptr);
            m_parts.at(i)->setRenderTechniqueAccessor(techniqueAccessor);
        }
    }
}

void CMETankComplex::setSceneUpdateMgr(CSharedSceneUpdateMgrRef sceneUpdateMgr)
{
    IGameObject::setSceneUpdateMgr(sceneUpdateMgr);
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        for(ui32 i = 0; i < m_parts.size(); ++i)
        {
            assert(m_parts.at(i) != nullptr);
            m_parts.at(i)->setSceneUpdateMgr(sceneUpdateMgr);
        }
    }
}

void CMETankComplex::enableRender(bool value)
{
    IGameObject::enableRender(value);
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        for(ui32 i = 0; i < m_parts.size(); ++i)
        {
            assert(m_parts.at(i) != nullptr);
            m_parts.at(i)->enableRender(value);
        }
    }
}

void CMETankComplex::enableUpdate(bool value)
{
    IGameObject::enableUpdate(value);
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        for(ui32 i = 0; i < m_parts.size(); ++i)
        {
            assert(m_parts.at(i) != nullptr);
            m_parts.at(i)->enableUpdate(value);
        }
    }
}
