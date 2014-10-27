//
//  CMEConfigurationPlacementModelLoadingOperation.cpp
//  ie3D-EMap
//
//  Created by sergey.sergeev on 10/27/14.
//
//

#include "CMEConfigurationPlacementModelLoadingOperation.h"
#include "CConfigurationMaterialLoadingOperation.h"
#include "CMEConfigurationPlacementModelSerializer.h"
#include "CMEConfigurationGameObjects.h"

CMEConfigurationPlacementModelLoadingOperation::CMEConfigurationPlacementModelLoadingOperation(void)
{
    
}

CMEConfigurationPlacementModelLoadingOperation::~CMEConfigurationPlacementModelLoadingOperation(void)
{
    
}

ISharedConfiguration CMEConfigurationPlacementModelLoadingOperation::serialize(const std::string& filename)
{
    std::shared_ptr<CMEConfigurationPlacementModelSerializer> placementModelSerializer = std::make_shared<CMEConfigurationPlacementModelSerializer>();
    std::shared_ptr<CMEConfigurationPlacementModel> placementModelConfiguration = std::static_pointer_cast<CMEConfigurationPlacementModel>(placementModelSerializer->serialize(filename));
    assert(placementModelConfiguration != nullptr);
    std::vector<std::string> materialsTemplatesFilenames = placementModelConfiguration->getMaterialsConfigurationsFilenames();
    for(const auto& iterator : materialsTemplatesFilenames)
    {
        std::shared_ptr<CConfigurationMaterialLoadingOperation> materialLoadingOperation = std::make_shared<CConfigurationMaterialLoadingOperation>();
        std::shared_ptr<CConfigurationMaterial> materialConfiguration = std::static_pointer_cast<CConfigurationMaterial>(materialLoadingOperation->serialize(iterator));
        assert(materialConfiguration != nullptr);
        placementModelConfiguration->setConfiguration(getConfigurationAttributeKey(placementModelConfiguration->kGameObjectMaterialsConfigurationsNode,
                                                                                   placementModelConfiguration->kGameObjectMaterialConfigurationNode,
                                                                                   placementModelConfiguration->kGameObjectMaterialFilenameAttribute),
                                                      materialConfiguration);
    }
    return placementModelConfiguration;
}