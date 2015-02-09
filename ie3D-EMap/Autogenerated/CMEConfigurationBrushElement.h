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
#if defined(__EDITOR__)
void setName(std::string name);
#endif
ui8 getColorR(void) const;
#if defined(__EDITOR__)
void setColorR(ui8 color_r);
#endif
ui8 getColorG(void) const;
#if defined(__EDITOR__)
void setColorG(ui8 color_g);
#endif
ui8 getColorB(void) const;
#if defined(__EDITOR__)
void setColorB(ui8 color_b);
#endif
ui8 getColorA(void) const;
#if defined(__EDITOR__)
void setColorA(ui8 color_a);
#endif
f32 getWidth(void) const;
#if defined(__EDITOR__)
void setWidth(f32 width);
#endif
f32 getHeight(void) const;
#if defined(__EDITOR__)
void setHeight(f32 height);
#endif
void serialize(pugi::xml_document& document, const std::string& path);
void serialize(pugi::xml_document& document, pugi::xpath_node& node);
};
#endif
