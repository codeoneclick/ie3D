//
//  CMEConfigurationAccessor.cpp
//  ie3D-Demo
//
//  Created by Sergey Sergeev on 6/7/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#include "CMEConfigurationAccessor.h"
#include "CMEConfigurationGameObjects.h"
#include "CMEConfigurationLandscapeBrushLoadingOperation.h"

CMEConfigurationAccessor::CMEConfigurationAccessor(void)
{
    
}

CMEConfigurationAccessor::~CMEConfigurationAccessor(void)
{
    
}

void CMEConfigurationAccessor::loadLandscapeBrushConfiguration(const std::string& filename,
                                                               ISharedConfigurationLoadingHandlerRef handler)
{
    std::shared_ptr<CMEConfigurationLandscapeBrushLoadingOperation> operation = std::make_shared<CMEConfigurationLandscapeBrushLoadingOperation>();
    std::shared_ptr<CConfigurationModel> configuration = std::static_pointer_cast<CConfigurationModel>(operation->serialize(filename));
    assert(configuration != nullptr);
    assert(handler != nullptr);
    handler->onConfigurationLoaded(configuration, true);
}
