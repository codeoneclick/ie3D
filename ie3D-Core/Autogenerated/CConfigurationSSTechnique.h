// autogenerated: do not add any changes
#ifndef CConfigurationSSTechnique_h
#define CConfigurationSSTechnique_h
#include "IConfiguration.h"
#include "CConfigurationMaterial.h"
class CConfigurationSSTechnique : public IConfiguration
{
public:
CConfigurationSSTechnique(void) = default;
~CConfigurationSSTechnique(void) = default;
std::string getGUID(void) const;
#if defined(__EDITOR__)
void setGUID(std::string guid);
#endif
ui32 getScreenWidth(void) const;
#if defined(__EDITOR__)
void setScreenWidth(ui32 screen_width);
#endif
ui32 getScreenHeight(void) const;
#if defined(__EDITOR__)
void setScreenHeight(ui32 screen_height);
#endif
std::shared_ptr<CConfigurationMaterial> getConfigurationMaterial(void) const;
#if defined(__EDITOR__)
void setConfigurationMaterial(const std::shared_ptr<CConfigurationMaterial>& material);
#endif
void serialize(const std::string& filename);
#if defined(__EDITOR__)
void deserialize(const std::string& filename);
#endif
};
#endif
