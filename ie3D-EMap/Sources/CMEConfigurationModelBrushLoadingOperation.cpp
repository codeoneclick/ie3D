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
#include "CEConfigurationGameObjects.h"
#include "CEConfigurationCustomModelLoadingOperation.h"

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
    
    std::vector<std::string> elementsConfigurationsFilenames = configuration->getElementsFilenames();
    for(const auto& iterator : elementsConfigurationsFilenames)
    {
        std::shared_ptr<CEConfigurationCustomModelLoadingOperation> elementLoadingOperation = std::make_shared<CEConfigurationCustomModelLoadingOperation>();
        std::shared_ptr<CEConfigurationCustomModel> elementConfiguration = std::static_pointer_cast<CEConfigurationCustomModel>(elementLoadingOperation->serialize(iterator));
        assert(elementConfiguration != nullptr);
        configuration->setConfiguration(getConfigurationAttributeKey(configuration->kModelBrushElementsConfigurationsNode,
                                                                     configuration->kModelBrushElementConfigurationNode,
                                                                     configuration->kModelBrushElementFilenameAttribute),
                                        elementConfiguration);
    }
    return configuration;
}