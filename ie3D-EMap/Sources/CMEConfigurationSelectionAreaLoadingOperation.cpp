//
//  CMEConfigurationSelectionAreaLoadingOperation.cpp
//  ie3D-Demo
//
//  Created by Sergey Sergeev on 6/7/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#include "CMEConfigurationSelectionAreaLoadingOperation.h"
#include "CConfigurationMaterialLoadingOperation.h"
#include "CMEConfigurationSelectionAreaSerializer.h"
#include "CMEConfigurationGameObjects.h"

CMEConfigurationSelectionAreaLoadingOperation::CMEConfigurationSelectionAreaLoadingOperation(void)
{
    
}

CMEConfigurationSelectionAreaLoadingOperation::~CMEConfigurationSelectionAreaLoadingOperation(void)
{
    
}

ISharedConfiguration CMEConfigurationSelectionAreaLoadingOperation::serialize(const std::string& filename)
{
    std::shared_ptr<CMEConfigurationSelectionAreaSerializer> selectionAreaSerializer = std::make_shared<CMEConfigurationSelectionAreaSerializer>();
    std::shared_ptr<CMEConfigurationSelectionArea> selectionAreaConfiguration = std::static_pointer_cast<CMEConfigurationSelectionArea>(selectionAreaSerializer->serialize(filename));
    assert(selectionAreaConfiguration != nullptr);
    std::vector<std::string> materialsTemplatesFilenames = selectionAreaConfiguration->getMaterialsConfigurationsFilenames();
    for(const auto& iterator : materialsTemplatesFilenames)
    {
        std::shared_ptr<CConfigurationMaterialLoadingOperation> materialLoadingOperation = std::make_shared<CConfigurationMaterialLoadingOperation>();
        std::shared_ptr<CConfigurationMaterial> materialConfiguration = std::static_pointer_cast<CConfigurationMaterial>(materialLoadingOperation->serialize(iterator));
        assert(materialConfiguration != nullptr);
        selectionAreaConfiguration->setConfiguration(getConfigurationAttributeKey(selectionAreaConfiguration->kGameObjectMaterialsConfigurationsNode,
                                                                                  selectionAreaConfiguration->kGameObjectMaterialConfigurationNode,
                                                                                  selectionAreaConfiguration->kGameObjectMaterialFilenameAttribute),
                                                     materialConfiguration);
    }
    return selectionAreaConfiguration;
}