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
#include "CMEModelBrush.h"
#include "CMESceneStage.h"

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
    configurationAccessor->getMEConfigurationLandscapeBrush(filename, landscapeBrush);
    m_gameObjectsContainer.insert(landscapeBrush);
    return landscapeBrush;
}

CMESharedModelBrush CMESceneFabricator::createModelBrush(const std::string &filename)
{
    assert(m_resourceAccessor != nullptr);
    assert(m_renderTechniqueAccessor != nullptr);
    CMESharedModelBrush modelBrush = std::make_shared<CMEModelBrush>(m_resourceAccessor, m_renderTechniqueAccessor);
    assert(m_configurationAccessor != nullptr);
    std::shared_ptr<CMEConfigurationAccessor> configurationAccessor = std::static_pointer_cast<CMEConfigurationAccessor>(m_configurationAccessor);
    configurationAccessor->getMEConfigurationModelBrush(filename, modelBrush);
    m_gameObjectsContainer.insert(modelBrush);
    return modelBrush;
}

CMESharedSceneStage CMESceneFabricator::createSceneStage(const std::string& filename)
{
    assert(m_resourceAccessor != nullptr);
    assert(m_renderTechniqueAccessor != nullptr);
    CMESharedSceneStage sceneStage = std::make_shared<CMESceneStage>(m_resourceAccessor, m_renderTechniqueAccessor);
    assert(m_configurationAccessor != nullptr);
    std::shared_ptr<CMEConfigurationAccessor> configurationAccessor = std::static_pointer_cast<CMEConfigurationAccessor>(m_configurationAccessor);
    configurationAccessor->getMEConfigurationSceneStage(filename, sceneStage);
    m_gameObjectsContainer.insert(sceneStage);
    return sceneStage;
}