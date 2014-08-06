//
//  CESceneFabricator.cpp
//  ie3D-Engine
//
//  Created by sergey.sergeev on 8/6/14.
//  Copyright (c) 2014 sergey.sergeev. All rights reserved.
//

#include "CESceneFabricator.h"
#include "CEConfigurationAccessor.h"
#include "CResourceAccessor.h"
#include "CEComplexModel.h"

CESceneFabricator::CESceneFabricator(CSharedConfigurationAccessorRef configurationAccessor,
                                       CSharedResourceAccessorRef resourceAccessor,
                                       ISharedRenderTechniqueAccessorRef renderTechniqueAccessor) :
CSceneFabricator(configurationAccessor, resourceAccessor, renderTechniqueAccessor)
{
    
}

CESceneFabricator::~CESceneFabricator(void)
{
    
}

CESharedComplexModel CESceneFabricator::createComplexModel(const std::string& filename)
{
    assert(m_resourceAccessor != nullptr);
	assert(m_renderTechniqueAccessor != nullptr);
    CESharedComplexModel complexModel = std::make_shared<CEComplexModel>(m_resourceAccessor, m_renderTechniqueAccessor);
    assert(m_configurationAccessor != nullptr);
    std::shared_ptr<CEConfigurationAccessor> configurationAccessor = std::static_pointer_cast<CEConfigurationAccessor>(m_configurationAccessor);
    configurationAccessor->loadComplexModelConfiguration(filename, complexModel);
    m_gameObjectsContainer.insert(complexModel);
    return complexModel;
}