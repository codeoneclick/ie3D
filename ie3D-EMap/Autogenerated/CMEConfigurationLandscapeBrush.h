// autogenerated: do not add any changes
#ifndef CMEConfigurationLandscapeBrush_h
#define CMEConfigurationLandscapeBrush_h
#include "IConfiguration.h"
#include "CConfigurationMaterial.h"
class CMEConfigurationLandscapeBrush : public IConfigurationGameObject
{
public:
CMEConfigurationLandscapeBrush(void) = default;
~CMEConfigurationLandscapeBrush(void) = default;
f32 getgetSize(void) const;
#if defined(__EDITOR__)
void setgetSize(f32 size);
#endif
i32 getZOrder(void) const;
#if defined(__EDITOR__)
void setZOrder(i32 z_order);
#endif
std::vector<std::shared_ptr<IConfiguration>> getMaterialsConfigurations(void) const;
#if defined(__EDITOR__)
void addMaterialsConfigurations(const std::shared_ptr<CConfigurationMaterial>& material);
#endif
#if defined(__EDITOR__)
void setMaterialsConfigurations(const std::shared_ptr<CConfigurationMaterial>& material, i32 index);
#endif
void serialize(const std::string& filename);
#if defined(__EDITOR__)
void deserialize(const std::string& filename);
#endif
};
#endif
