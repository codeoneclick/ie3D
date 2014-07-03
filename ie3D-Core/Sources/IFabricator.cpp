//
//  IFabricator.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/28/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "IFabricator.h"

IFabricator::IFabricator(CSharedConfigurationAccessorRef configurationAccessor,
                         CSharedResourceAccessorRef resourceAccessor,
                         ISharedRenderTechniqueAccessorRef renderTechniqueAccessor) :
m_configurationAccessor(configurationAccessor),
m_resourceAccessor(resourceAccessor),
m_renderTechniqueAccessor(renderTechniqueAccessor)
{

}

IFabricator::~IFabricator(void)
{

}

