// autogenerated: do not add any changes
#include "CMEConfigurationBrushElement.h"
std::string CMEConfigurationBrushElement::getName(void) const
{
const auto& iterator = m_attributes.find("/brush_element/name");
assert(iterator != m_attributes.end());
std::string value; iterator->second->get(&value);
return value;
}
#if defined(__EDITOR__)
void CMEConfigurationBrushElement::setName(std::string name)
{
IConfiguration::setAttribute("/brush_element/name", std::make_shared<CConfigurationAttribute>(name));
}
#endif
ui8 CMEConfigurationBrushElement::getColorR(void) const
{
const auto& iterator = m_attributes.find("/brush_element/color_r");
assert(iterator != m_attributes.end());
ui8 value; iterator->second->get(&value);
return value;
}
#if defined(__EDITOR__)
void CMEConfigurationBrushElement::setColorR(ui8 color_r)
{
IConfiguration::setAttribute("/brush_element/color_r", std::make_shared<CConfigurationAttribute>(color_r));
}
#endif
ui8 CMEConfigurationBrushElement::getColorG(void) const
{
const auto& iterator = m_attributes.find("/brush_element/color_g");
assert(iterator != m_attributes.end());
ui8 value; iterator->second->get(&value);
return value;
}
#if defined(__EDITOR__)
void CMEConfigurationBrushElement::setColorG(ui8 color_g)
{
IConfiguration::setAttribute("/brush_element/color_g", std::make_shared<CConfigurationAttribute>(color_g));
}
#endif
ui8 CMEConfigurationBrushElement::getColorB(void) const
{
const auto& iterator = m_attributes.find("/brush_element/color_b");
assert(iterator != m_attributes.end());
ui8 value; iterator->second->get(&value);
return value;
}
#if defined(__EDITOR__)
void CMEConfigurationBrushElement::setColorB(ui8 color_b)
{
IConfiguration::setAttribute("/brush_element/color_b", std::make_shared<CConfigurationAttribute>(color_b));
}
#endif
ui8 CMEConfigurationBrushElement::getColorA(void) const
{
const auto& iterator = m_attributes.find("/brush_element/color_a");
assert(iterator != m_attributes.end());
ui8 value; iterator->second->get(&value);
return value;
}
#if defined(__EDITOR__)
void CMEConfigurationBrushElement::setColorA(ui8 color_a)
{
IConfiguration::setAttribute("/brush_element/color_a", std::make_shared<CConfigurationAttribute>(color_a));
}
#endif
f32 CMEConfigurationBrushElement::getWidth(void) const
{
const auto& iterator = m_attributes.find("/brush_element/width");
assert(iterator != m_attributes.end());
f32 value; iterator->second->get(&value);
return value;
}
#if defined(__EDITOR__)
void CMEConfigurationBrushElement::setWidth(f32 width)
{
IConfiguration::setAttribute("/brush_element/width", std::make_shared<CConfigurationAttribute>(width));
}
#endif
f32 CMEConfigurationBrushElement::getHeight(void) const
{
const auto& iterator = m_attributes.find("/brush_element/height");
assert(iterator != m_attributes.end());
f32 value; iterator->second->get(&value);
return value;
}
#if defined(__EDITOR__)
void CMEConfigurationBrushElement::setHeight(f32 height)
{
IConfiguration::setAttribute("/brush_element/height", std::make_shared<CConfigurationAttribute>(height));
}
#endif
void CMEConfigurationBrushElement::serialize(pugi::xml_document& document, const std::string& path)
{
pugi::xpath_node node;
node = document.select_single_node((path + "/brush_element").c_str());
std::string name = node.node().attribute("name").as_string();
IConfiguration::setAttribute("/brush_element/name", std::make_shared<CConfigurationAttribute>(name));
ui8 color_r = node.node().attribute("color_r").as_uint();
IConfiguration::setAttribute("/brush_element/color_r", std::make_shared<CConfigurationAttribute>(color_r));
ui8 color_g = node.node().attribute("color_g").as_uint();
IConfiguration::setAttribute("/brush_element/color_g", std::make_shared<CConfigurationAttribute>(color_g));
ui8 color_b = node.node().attribute("color_b").as_uint();
IConfiguration::setAttribute("/brush_element/color_b", std::make_shared<CConfigurationAttribute>(color_b));
ui8 color_a = node.node().attribute("color_a").as_uint();
IConfiguration::setAttribute("/brush_element/color_a", std::make_shared<CConfigurationAttribute>(color_a));
f32 width = node.node().attribute("width").as_float();
IConfiguration::setAttribute("/brush_element/width", std::make_shared<CConfigurationAttribute>(width));
f32 height = node.node().attribute("height").as_float();
IConfiguration::setAttribute("/brush_element/height", std::make_shared<CConfigurationAttribute>(height));
}
#if defined(__EDITOR__)
void CMEConfigurationBrushElement::deserialize(pugi::xml_node& node)
{
pugi::xml_attribute attribute;
attribute = node.append_attribute("name");
std::string name = CMEConfigurationBrushElement::getName();
attribute.set_value(name.c_str());
attribute = node.append_attribute("color_r");
ui8 color_r = CMEConfigurationBrushElement::getColorR();
attribute.set_value(color_r);
attribute = node.append_attribute("color_g");
ui8 color_g = CMEConfigurationBrushElement::getColorG();
attribute.set_value(color_g);
attribute = node.append_attribute("color_b");
ui8 color_b = CMEConfigurationBrushElement::getColorB();
attribute.set_value(color_b);
attribute = node.append_attribute("color_a");
ui8 color_a = CMEConfigurationBrushElement::getColorA();
attribute.set_value(color_a);
attribute = node.append_attribute("width");
f32 width = CMEConfigurationBrushElement::getWidth();
attribute.set_value(width);
attribute = node.append_attribute("height");
f32 height = CMEConfigurationBrushElement::getHeight();
attribute.set_value(height);
}
#endif
void CMEConfigurationBrushElement::serialize(pugi::xml_document& document, pugi::xpath_node& node)
{
std::string name = node.node().attribute("name").as_string();
IConfiguration::setAttribute("/brush_element/name", std::make_shared<CConfigurationAttribute>(name));
ui8 color_r = node.node().attribute("color_r").as_uint();
IConfiguration::setAttribute("/brush_element/color_r", std::make_shared<CConfigurationAttribute>(color_r));
ui8 color_g = node.node().attribute("color_g").as_uint();
IConfiguration::setAttribute("/brush_element/color_g", std::make_shared<CConfigurationAttribute>(color_g));
ui8 color_b = node.node().attribute("color_b").as_uint();
IConfiguration::setAttribute("/brush_element/color_b", std::make_shared<CConfigurationAttribute>(color_b));
ui8 color_a = node.node().attribute("color_a").as_uint();
IConfiguration::setAttribute("/brush_element/color_a", std::make_shared<CConfigurationAttribute>(color_a));
f32 width = node.node().attribute("width").as_float();
IConfiguration::setAttribute("/brush_element/width", std::make_shared<CConfigurationAttribute>(width));
f32 height = node.node().attribute("height").as_float();
IConfiguration::setAttribute("/brush_element/height", std::make_shared<CConfigurationAttribute>(height));
}
