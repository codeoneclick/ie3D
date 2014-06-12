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
                         ISharedScreenSpaceRenderAccessor screenSpaceTextureAccessor) :
m_configurationAccessor(configurationAccessor),
m_resourceAccessor(resourceAccessor),
m_screenSpaceTextureAccessor(screenSpaceTextureAccessor)
{

}

IFabricator::~IFabricator(void)
{

}

