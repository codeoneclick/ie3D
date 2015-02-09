// autogenerated: do not add any changes
#ifndef CConfigurationTexture_h
#define CConfigurationTexture_h
#include "IConfiguration.h"
class CConfigurationTexture : public IConfiguration
{
public:
CConfigurationTexture(void) = default;
~CConfigurationTexture(void) = default;
bool getCubemap(void) const;
#if defined(__EDITOR__)
void setCubemap(bool is_cubemap);
#endif
std::string getFilename(void) const;
#if defined(__EDITOR__)
void setFilename(std::string filename);
#endif
std::string getFilenamePositiveX(void) const;
#if defined(__EDITOR__)
void setFilenamePositiveX(std::string filename_x_positive);
#endif
std::string getFilenameNegativeX(void) const;
#if defined(__EDITOR__)
void setFilenameNegativeX(std::string filename_x_negative);
#endif
std::string getFilenamePositiveY(void) const;
#if defined(__EDITOR__)
void setFilenamePositiveY(std::string filename_y_positive);
#endif
std::string getFilenameNegativeY(void) const;
#if defined(__EDITOR__)
void setFilenameNegativeY(std::string filename_y_negative);
#endif
std::string getFilenamePositiveZ(void) const;
#if defined(__EDITOR__)
void setFilenamePositiveZ(std::string filename_z_positive);
#endif
std::string getFilenameNegativeZ(void) const;
#if defined(__EDITOR__)
void setFilenameNegativeZ(std::string filename_z_negative);
#endif
std::string getRenderTechniqueTextureName(void) const;
#if defined(__EDITOR__)
void setRenderTechniqueTextureName(std::string render_operation_name);
#endif
ui32 getSamplerIndex(void) const;
#if defined(__EDITOR__)
void setSamplerIndex(ui32 sampler_index);
#endif
GLenum getWrapMode(void) const;
#if defined(__EDITOR__)
void setWrapMode(GLenum wrap_mode);
#endif
GLenum getMagFilter(void) const;
#if defined(__EDITOR__)
void setMagFilter(GLenum mag_filter);
#endif
GLenum getMinFilter(void) const;
#if defined(__EDITOR__)
void setMinFilter(GLenum min_filter);
#endif
void serialize(pugi::xml_document& document, const std::string& path);
#if defined(__EDITOR__)
void deserialize(pugi::xml_node& node);
#endif
void serialize(pugi::xml_document& document, pugi::xpath_node& node);
};
#endif
