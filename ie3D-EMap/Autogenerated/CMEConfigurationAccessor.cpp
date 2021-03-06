// autogenerated: do not add any changes, added changes will be lost.
#include "CMEConfigurationAccessor.h"
void CMEConfigurationAccessor::getMEConfigurationLandscapeBrush(const std::string& filename, const std::shared_ptr<IConfigurationLoadingHandler>& handler) const
{
std::shared_ptr<CMEConfigurationLandscapeBrush> configuration = std::make_shared<CMEConfigurationLandscapeBrush>();
configuration->serialize(filename);
assert(configuration != nullptr);
assert(handler != nullptr);
handler->onConfigurationLoaded(configuration, true);
}
void CMEConfigurationAccessor::getMEConfigurationModelBrush(const std::string& filename, const std::shared_ptr<IConfigurationLoadingHandler>& handler) const
{
std::shared_ptr<CMEConfigurationModelBrush> configuration = std::make_shared<CMEConfigurationModelBrush>();
configuration->serialize(filename);
assert(configuration != nullptr);
assert(handler != nullptr);
handler->onConfigurationLoaded(configuration, true);
}
void CMEConfigurationAccessor::getMEConfigurationSceneStage(const std::string& filename, const std::shared_ptr<IConfigurationLoadingHandler>& handler) const
{
std::shared_ptr<CMEConfigurationSceneStage> configuration = std::make_shared<CMEConfigurationSceneStage>();
configuration->serialize(filename);
assert(configuration != nullptr);
assert(handler != nullptr);
handler->onConfigurationLoaded(configuration, true);
}
