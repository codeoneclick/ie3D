// autogenerated: do not add any changes
#include "CConfigurationSSTechnique.h"
std::string CConfigurationSSTechnique::getGUID(void) const
{
const auto& iterator = m_attributes.find("/ss_technique/guid");
assert(iterator != m_attributes.end());
std::string value; iterator->second->get(&value);
return value;
}
#if defined(__EDITOR__)
void CConfigurationSSTechnique::setGUID(std::string guid)
{
IConfiguration::setAttribute("/ss_technique/guid", std::make_shared<CConfigurationAttribute>(guid));
}
#endif
ui32 CConfigurationSSTechnique::getScreenWidth(void) const
{
const auto& iterator = m_attributes.find("/ss_technique/screen_width");
assert(iterator != m_attributes.end());
ui32 value; iterator->second->get(&value);
return value;
}
#if defined(__EDITOR__)
void CConfigurationSSTechnique::setScreenWidth(ui32 screen_width)
{
IConfiguration::setAttribute("/ss_technique/screen_width", std::make_shared<CConfigurationAttribute>(screen_width));
}
#endif
ui32 CConfigurationSSTechnique::getScreenHeight(void) const
{
const auto& iterator = m_attributes.find("/ss_technique/screen_height");
assert(iterator != m_attributes.end());
ui32 value; iterator->second->get(&value);
return value;
}
#if defined(__EDITOR__)
void CConfigurationSSTechnique::setScreenHeight(ui32 screen_height)
{
IConfiguration::setAttribute("/ss_technique/screen_height", std::make_shared<CConfigurationAttribute>(screen_height));
}
#endif
std::shared_ptr<CConfigurationMaterial> CConfigurationSSTechnique::getConfigurationMaterial(void) const
{
const auto& iterator = m_configurations.find("/ss_technique/material");
if(iterator == m_configurations.end())
{
return nullptr;
}
assert(iterator != m_configurations.end());
assert(iterator->second.size() != 0);
return std::static_pointer_cast<CConfigurationMaterial>(iterator->second.at(0));
}
#if defined(__EDITOR__)
void CConfigurationSSTechnique::setConfigurationMaterial(const std::shared_ptr<CConfigurationMaterial>& material)
{
IConfiguration::setConfiguration("/ss_technique/material", material, 0);
}
#endif
void CConfigurationSSTechnique::serialize(const std::string& filename)
{
pugi::xml_document document;
pugi::xml_parse_result result = IConfiguration::openXMLDocument(document, filename);
assert(result.status == pugi::status_ok);
pugi::xpath_node node;
node = document.select_single_node("/ss_technique");
std::string guid = node.node().attribute("guid").as_string();
IConfiguration::setAttribute("/ss_technique/guid", std::make_shared<CConfigurationAttribute>(guid));
ui32 screen_width = node.node().attribute("screen_width").as_uint();
IConfiguration::setAttribute("/ss_technique/screen_width", std::make_shared<CConfigurationAttribute>(screen_width));
ui32 screen_height = node.node().attribute("screen_height").as_uint();
IConfiguration::setAttribute("/ss_technique/screen_height", std::make_shared<CConfigurationAttribute>(screen_height));
std::shared_ptr<CConfigurationMaterial> material = std::make_shared<CConfigurationMaterial>();
pugi::xpath_node material_node = document.select_single_node("/ss_technique/material");
material->serialize(material_node.node().attribute("filename").as_string());
IConfiguration::setConfiguration("/ss_technique/material", material);
}
#if defined(__EDITOR__)
void CConfigurationSSTechnique::deserialize(const std::string& filename)
{
pugi::xml_document document;
pugi::xml_parse_result result = document.load("");
assert(result.status == pugi::status_ok);
pugi::xml_node node = document.append_child("ss_technique");
pugi::xml_attribute attribute;
attribute = node.append_attribute("guid");
std::string guid = CConfigurationSSTechnique::getGUID();
attribute.set_value(guid.c_str());
attribute = node.append_attribute("screen_width");
ui32 screen_width = CConfigurationSSTechnique::getScreenWidth();
attribute.set_value(screen_width);
attribute = node.append_attribute("screen_height");
ui32 screen_height = CConfigurationSSTechnique::getScreenHeight();
attribute.set_value(screen_height);
document.save_file(filename.c_str());
}
#endif
