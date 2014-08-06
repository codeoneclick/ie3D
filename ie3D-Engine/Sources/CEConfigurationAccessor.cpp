//
//  CEConfigurationAccessor.cpp
//  ie3D-Engine
//
//  Created by sergey.sergeev on 8/6/14.
//  Copyright (c) 2014 sergey.sergeev. All rights reserved.
//

#include "CEConfigurationAccessor.h"
#include "CEConfigurationGameObjects.h"
#include "CEConfigurationComplexModelLoadingOperation.h"

CEConfigurationAccessor::CEConfigurationAccessor(void)
{
    
}

CEConfigurationAccessor::~CEConfigurationAccessor(void)
{
    
}

void CEConfigurationAccessor::loadComplexModelConfiguration(const std::string& filename,
                                                            ISharedConfigurationLoadingHandlerRef handler)
{
    std::shared_ptr<CEConfigurationComplexModelLoadingOperation> operation = std::make_shared<CEConfigurationComplexModelLoadingOperation>();
    std::shared_ptr<CConfigurationModel> configuration = std::static_pointer_cast<CConfigurationModel>(operation->serialize(filename));
    assert(configuration != nullptr);
    assert(handler != nullptr);
    handler->onConfigurationLoaded(configuration, true);
}