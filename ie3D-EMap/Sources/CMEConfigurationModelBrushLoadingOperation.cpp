//
//  CMEConfigurationModelBrushLoadingOperation.cpp
//  ie3D-EMap
//
//  Created by sergey.sergeev on 10/27/14.
//
//

#include "CMEConfigurationModelBrushLoadingOperation.h"
#include "CConfigurationMaterialLoadingOperation.h"
#include "CMEConfigurationModelBrushSerializer.h"
#include "CMEConfigurationGameObjects.h"

CMEConfigurationModelBrushLoadingOperation::CMEConfigurationModelBrushLoadingOperation(void)
{
    
}

CMEConfigurationModelBrushLoadingOperation::~CMEConfigurationModelBrushLoadingOperation(void)
{
    
}

ISharedConfiguration CMEConfigurationModelBrushLoadingOperation::serialize(const std::string& filename)
{
    std::shared_ptr<CMEConfigurationModelBrushSerializer> serializer = std::make_shared<CMEConfigurationModelBrushSerializer>();
    std::shared_ptr<CMEConfigurationModelBrush> configuration = std::static_pointer_cast<CMEConfigurationModelBrush>(serializer->serialize(filename));
    assert(configuration != nullptr);
    std::vector<std::string> materialsConfigurationsFilenames = configuration->getMaterialsConfigurationsFilenames();
    for(const auto& iterator : materialsConfigurationsFilenames)
    {
        std::shared_ptr<CConfigurationMaterialLoadingOperation> materialLoadingOperation = std::make_shared<CConfigurationMaterialLoadingOperation>();
        std::shared_ptr<CConfigurationMaterial> materialConfiguration = std::static_pointer_cast<CConfigurationMaterial>(materialLoadingOperation->serialize(iterator));
        assert(materialConfiguration != nullptr);
        configuration->setConfiguration(getConfigurationAttributeKey(configuration->kGameObjectMaterialsConfigurationsNode,
                                                                     configuration->kGameObjectMaterialConfigurationNode,
                                                                     configuration->kGameObjectMaterialFilenameAttribute),
                                        materialConfiguration);
    }
    return configuration;
}