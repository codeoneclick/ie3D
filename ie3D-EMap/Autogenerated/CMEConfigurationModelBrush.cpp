// autogenerated: do not add any changes
#include "CMEConfigurationModelBrush.h"
std::vector<std::shared_ptr<IConfiguration>> CMEConfigurationModelBrush::getElementsConfigurations(void) const
{
const auto& iterator = m_configurations.find("/model_brush/elements/element");
if(iterator == m_configurations.end())
{
return std::vector<std::shared_ptr<IConfiguration>>();
}
assert(iterator != m_configurations.end());
return iterator->second;
}
#if defined(__EDITOR__)
void CMEConfigurationModelBrush::addElementsConfigurations(const std::shared_ptr<CMEConfigurationBrushElement>& element)
{
IConfiguration::setConfiguration("/model_brush/elements/element", element);
}
#endif
#if defined(__EDITOR__)
void CMEConfigurationModelBrush::setElementsConfigurations(const std::shared_ptr<CMEConfigurationBrushElement>& element, i32 index)
{
IConfiguration::setConfiguration("/model_brush/elements/element", element, index);
}
#endif
std::vector<std::shared_ptr<IConfiguration>> CMEConfigurationModelBrush::getMaterialsConfigurations(void) const
{
const auto& iterator = m_configurations.find("/model_brush/materials/material");
if(iterator == m_configurations.end())
{
return std::vector<std::shared_ptr<IConfiguration>>();
}
assert(iterator != m_configurations.end());
return iterator->second;
}
#if defined(__EDITOR__)
void CMEConfigurationModelBrush::addMaterialsConfigurations(const std::shared_ptr<CConfigurationMaterial>& material)
{
IConfiguration::setConfiguration("/model_brush/materials/material", material);
}
#endif
#if defined(__EDITOR__)
void CMEConfigurationModelBrush::setMaterialsConfigurations(const std::shared_ptr<CConfigurationMaterial>& material, i32 index)
{
IConfiguration::setConfiguration("/model_brush/materials/material", material, index);
}
#endif
void CMEConfigurationModelBrush::serialize(const std::string& filename)
{
pugi::xml_document document;
pugi::xml_parse_result result = IConfiguration::openXMLDocument(document, filename);
assert(result.status == pugi::status_ok);
pugi::xpath_node node;
node = document.select_single_node("/model_brush");
pugi::xpath_node_set element_nodes = document.select_nodes("/model_brush/elements/element");
for (pugi::xpath_node_set::const_iterator iterator = element_nodes.begin(); iterator != element_nodes.end(); ++iterator)
{
std::shared_ptr<CMEConfigurationBrushElement> element = std::make_shared<CMEConfigurationBrushElement>();
pugi::xpath_node node = (*iterator);
element->serialize(document, node);
IConfiguration::setConfiguration("/model_brush/elements/element", element);
}
pugi::xpath_node_set material_nodes = document.select_nodes("/model_brush/materials/material");
for (pugi::xpath_node_set::const_iterator iterator = material_nodes.begin(); iterator != material_nodes.end(); ++iterator)
{
std::shared_ptr<CConfigurationMaterial> material = std::make_shared<CConfigurationMaterial>();
material->serialize((*iterator).node().attribute("filename").as_string());
IConfiguration::setConfiguration("/model_brush/materials/material", material);
}
}
