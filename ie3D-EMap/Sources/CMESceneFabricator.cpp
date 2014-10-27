//
//  CMESceneFabricator.cpp
//  ie3D-EMap
//
//  Created by sergey.sergeev on 6/9/14.
//
//

#include "CMESceneFabricator.h"
#include "CMEConfigurationAccessor.h"
#include "CResourceAccessor.h"
#include "CMELandscapeBrush.h"
#include "CMEPlacementModel.h"

CMESceneFabricator::CMESceneFabricator(CSharedConfigurationAccessorRef configurationAccessor,
                                       CSharedResourceAccessorRef resourceAccessor,
                                       ISharedRenderTechniqueAccessorRef renderTechniqueAccessor) :
CESceneFabricator(configurationAccessor, resourceAccessor, renderTechniqueAccessor)
{
    
}

CMESceneFabricator::~CMESceneFabricator(void)
{

}

CMESharedLandscapeBrush CMESceneFabricator::createLandscapeBrush(const std::string &filename)
{
    assert(m_resourceAccessor != nullptr);
	assert(m_renderTechniqueAccessor != nullptr);
    CMESharedLandscapeBrush landscapeBrush = std::make_shared<CMELandscapeBrush>(m_resourceAccessor, m_renderTechniqueAccessor);
    assert(m_configurationAccessor != nullptr);
    std::shared_ptr<CMEConfigurationAccessor> configurationAccessor = std::static_pointer_cast<CMEConfigurationAccessor>(m_configurationAccessor);
    configurationAccessor->loadLandscapeBrushConfiguration(filename, landscapeBrush);
    m_gameObjectsContainer.insert(landscapeBrush);
    return landscapeBrush;
}

CMESharedPlacementModel CMESceneFabricator::createPlacementModel(const std::string &filename)
{
    assert(m_resourceAccessor != nullptr);
    assert(m_renderTechniqueAccessor != nullptr);
    CMESharedPlacementModel placementModel = std::make_shared<CMEPlacementModel>(m_resourceAccessor, m_renderTechniqueAccessor);
    assert(m_configurationAccessor != nullptr);
    std::shared_ptr<CMEConfigurationAccessor> configurationAccessor = std::static_pointer_cast<CMEConfigurationAccessor>(m_configurationAccessor);
    configurationAccessor->loadPlacementModelConfiguration(filename, placementModel);
    m_gameObjectsContainer.insert(placementModel);
    return placementModel;
}