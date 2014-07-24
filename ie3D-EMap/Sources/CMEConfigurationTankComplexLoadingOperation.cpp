//
//  CMEConfigurationTankComplexLoadingOperation.cpp
//  ie3D-EMap
//
//  Created by sergey.sergeev on 7/23/14.
//
//

#include "CMEConfigurationTankComplexLoadingOperation.h"
#include "CConfigurationModelLoadingOperation.h"
#include "CMEConfigurationTankComplexSerializer.h"
#include "CMEConfigurationGameObjects.h"

CMEConfigurationTankComplexLoadingOperation::CMEConfigurationTankComplexLoadingOperation(void)
{
    
}

CMEConfigurationTankComplexLoadingOperation::~CMEConfigurationTankComplexLoadingOperation(void)
{
    
}

ISharedConfiguration CMEConfigurationTankComplexLoadingOperation::serialize(const std::string& filename)
{
    std::shared_ptr<CMEConfigurationTankComplexSerializer> tankComplexSerializer = std::make_shared<CMEConfigurationTankComplexSerializer>();
    std::shared_ptr<CMEConfigurationTankComplex> tankComplexConfiguration = std::static_pointer_cast<CMEConfigurationTankComplex>(tankComplexSerializer->serialize(filename));
    assert(tankComplexConfiguration != nullptr);
    std::vector<std::string> modelsConfigurationsFilenames = tankComplexConfiguration->getModelsConfigurationsFilenames();
    for(const auto& iterator : modelsConfigurationsFilenames)
    {
        std::shared_ptr<CConfigurationModelLoadingOperation> modelLoadingOperation = std::make_shared<CConfigurationModelLoadingOperation>();
        std::shared_ptr<CConfigurationModel> modelConfiguration = std::static_pointer_cast<CConfigurationModel>(modelLoadingOperation->serialize(iterator));
        assert(modelConfiguration != nullptr);
        tankComplexConfiguration->setConfiguration(getConfigurationAttributeKey(tankComplexConfiguration->kTankComplexModelsConfigurationsNode,
                                                                                tankComplexConfiguration->kTankComplexModelConfigurationsNode,
                                                                                tankComplexConfiguration->kTankComplexModelFilenameAttribute),
                                                   modelConfiguration);
    }
    return tankComplexConfiguration;
}