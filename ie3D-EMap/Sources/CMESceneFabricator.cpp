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
#include "CSelectionArea.h"

CMESceneFabricator::CMESceneFabricator(CSharedConfigurationAccessorRef configurationAccessor,
                                       CSharedResourceAccessorRef resourceAccessor,
                                       ISharedRenderTechniqueAccessorRef renderTechniqueAccessor) :
CSceneFabricator(configurationAccessor, resourceAccessor, renderTechniqueAccessor)
{
    
}

CMESceneFabricator::~CMESceneFabricator(void)
{

}

CSharedSelectionArea CMESceneFabricator::createSelectionArea(const std::string &filename)
{
    assert(m_resourceAccessor != nullptr);
	assert(m_renderTechniqueAccessor != nullptr);
    CSharedSelectionArea selectionArea = std::make_shared<CSelectionArea>(m_resourceAccessor, m_renderTechniqueAccessor);
    assert(m_configurationAccessor != nullptr);
    std::shared_ptr<CMEConfigurationAccessor> configurationAccessor = std::static_pointer_cast<CMEConfigurationAccessor>(m_configurationAccessor);
    configurationAccessor->loadSelectionAreaConfiguration(filename, selectionArea);
    m_gameObjectsContainer.insert(selectionArea);
    return selectionArea;
}

void CMESceneFabricator::deleteSelectionArea(CSharedSelectionAreaRef selectionArea)
{
    m_gameObjectsContainer.erase(selectionArea);
}