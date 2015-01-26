// autogenerated: do not add any changes
#ifndef CConfigurationMaterial_h
#define CConfigurationMaterial_h
class CConfigurationMaterial : public IConfiguration
{
public:
CConfigurationMaterial(void) = default
~CConfigurationMaterial(void) = default
std::string getTechniqueName(void) const;
bool isDepthTest(void) const;
bool isDepthMask(void) const;
std::shared_ptr<CConfigurationShader> getConfigurationShader(void) const;
std::vector<std::shared_ptr<CConfigurationTexture>> getConfigurationsTextures(void) const;
};
