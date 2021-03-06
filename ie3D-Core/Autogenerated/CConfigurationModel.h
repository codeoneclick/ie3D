// autogenerated: do not add any changes
#ifndef CConfigurationModel_h
#define CConfigurationModel_h
#include "IConfiguration.h"
#include "CConfigurationMaterial.h"
#include "CConfigurationAnimation.h"
class CConfigurationModel : public IConfigurationGameObject
{
public:
CConfigurationModel(void) = default;
~CConfigurationModel(void) = default;
std::string getMeshFilename(void) const;
#if defined(__EDITOR__)
void setMeshFilename(std::string mesh_filename);
#endif
i32 getZOrder(void) const;
#if defined(__EDITOR__)
void setZOrder(i32 z_order);
#endif
bool getBatching(void) const;
#if defined(__EDITOR__)
void setBatching(bool is_batching);
#endif
std::vector<std::shared_ptr<IConfiguration>> getMaterialsConfigurations(void) const;
#if defined(__EDITOR__)
void addMaterialsConfigurations(const std::shared_ptr<CConfigurationMaterial>& material);
#endif
#if defined(__EDITOR__)
void setMaterialsConfigurations(const std::shared_ptr<CConfigurationMaterial>& material, i32 index);
#endif
std::vector<std::shared_ptr<IConfiguration>> getAnimationsConfigurations(void) const;
#if defined(__EDITOR__)
void addAnimationsConfigurations(const std::shared_ptr<CConfigurationAnimation>& animation);
#endif
#if defined(__EDITOR__)
void setAnimationsConfigurations(const std::shared_ptr<CConfigurationAnimation>& animation, i32 index);
#endif
void serialize(const std::string& filename);
#if defined(__EDITOR__)
void deserialize(const std::string& filename);
#endif
};
#endif
