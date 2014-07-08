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
#include "CEditableBrush.h"

CMESceneFabricator::CMESceneFabricator(CSharedConfigurationAccessorRef configurationAccessor,
                                       CSharedResourceAccessorRef resourceAccessor,
                                       ISharedRenderTechniqueAccessorRef renderTechniqueAccessor) :
CSceneFabricator(configurationAccessor, resourceAccessor, renderTechniqueAccessor)
{
    
}

CMESceneFabricator::~CMESceneFabricator(void)
{

}

CSharedEditableBrush CMESceneFabricator::createEditableBrush(const std::string &filename)
{
    assert(m_resourceAccessor != nullptr);
	assert(m_renderTechniqueAccessor != nullptr);
    CSharedEditableBrush editableBrush = std::make_shared<CEditableBrush>(m_resourceAccessor, m_renderTechniqueAccessor);
    assert(m_configurationAccessor != nullptr);
    std::shared_ptr<CMEConfigurationAccessor> configurationAccessor = std::static_pointer_cast<CMEConfigurationAccessor>(m_configurationAccessor);
    configurationAccessor->loadSelectionAreaConfiguration(filename, editableBrush);
    m_gameObjectsContainer.insert(editableBrush);
    return editableBrush;
}

void CMESceneFabricator::deleteEditableBrush(CSharedEditableBrushRef editableBrush)
{
    m_gameObjectsContainer.erase(editableBrush);
}