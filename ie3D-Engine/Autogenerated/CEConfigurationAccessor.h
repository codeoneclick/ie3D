// autogenerated: do not add any changes, added changes will be lost.
#ifndef CEConfigurationAccessor_h
#define CEConfigurationAccessor_h
#include "CConfigurationAccessor.h"
#include "CConfigurationCustomModel.h"
class CEConfigurationAccessor: public CConfigurationAccessor
{
public:
CEConfigurationAccessor(void) = default;
~CEConfigurationAccessor(void) = default;
void getConfigurationCustomModel(const std::string& filename, const std::shared_ptr<IConfigurationLoadingHandler>& handler) const;
};
#endif
