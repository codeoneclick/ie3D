// autogenerated: do not add any changes
#ifndef CMEConfigurationBrushElement_h
#define CMEConfigurationBrushElement_h
#include "IConfiguration.h"
class CMEConfigurationBrushElement : public IConfiguration
{
public:
CMEConfigurationBrushElement(void) = default;
~CMEConfigurationBrushElement(void) = default;
std::string getName(void) const;
ui8 getColorR(void) const;
ui8 getColorG(void) const;
ui8 getColorB(void) const;
ui8 getColorA(void) const;
f32 getWidth(void) const;
f32 getHeight(void) const;
void serialize(pugi::xml_document& document, const std::string& path);
void serialize(pugi::xml_document& document, pugi::xpath_node& node);
};
#endif