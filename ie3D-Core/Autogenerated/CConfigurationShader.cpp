// autogenerated: do not add any changes
#include "CConfigurationShader.h"
std::string CConfigurationShader::getVSFilename(void) const
{
const auto& iterator = m_attributes.find("/shader/vs_filename");
assert(iterator != m_attributes.end());
std::string value; iterator->second->get(&value);
return value;
}
#if defined(__EDITOR__)
void CConfigurationShader::setVSFilename(std::string vs_filename)
{
IConfiguration::setAttribute("/shader/vs_filename", std::make_shared<CConfigurationAttribute>(vs_filename));
}
#endif
std::string CConfigurationShader::getFSFilename(void) const
{
const auto& iterator = m_attributes.find("/shader/fs_filename");
assert(iterator != m_attributes.end());
std::string value; iterator->second->get(&value);
return value;
}
#if defined(__EDITOR__)
void CConfigurationShader::setFSFilename(std::string fs_filename)
{
IConfiguration::setAttribute("/shader/fs_filename", std::make_shared<CConfigurationAttribute>(fs_filename));
}
#endif
void CConfigurationShader::serialize(pugi::xml_document& document, const std::string& path)
{
pugi::xpath_node node;
node = document.select_single_node((path + "/shader").c_str());
std::string vs_filename = node.node().attribute("vs_filename").as_string();
IConfiguration::setAttribute("/shader/vs_filename", std::make_shared<CConfigurationAttribute>(vs_filename));
std::string fs_filename = node.node().attribute("fs_filename").as_string();
IConfiguration::setAttribute("/shader/fs_filename", std::make_shared<CConfigurationAttribute>(fs_filename));
}
#if defined(__EDITOR__)
void CConfigurationShader::deserialize(pugi::xml_node& node)
{
pugi::xml_attribute attribute;
attribute = node.append_attribute("vs_filename");
std::string vs_filename = CConfigurationShader::getVSFilename();
attribute.set_value(vs_filename.c_str());
attribute = node.append_attribute("fs_filename");
std::string fs_filename = CConfigurationShader::getFSFilename();
attribute.set_value(fs_filename.c_str());
}
#endif
void CConfigurationShader::serialize(pugi::xml_document& document, pugi::xpath_node& node)
{
std::string vs_filename = node.node().attribute("vs_filename").as_string();
IConfiguration::setAttribute("/shader/vs_filename", std::make_shared<CConfigurationAttribute>(vs_filename));
std::string fs_filename = node.node().attribute("fs_filename").as_string();
IConfiguration::setAttribute("/shader/fs_filename", std::make_shared<CConfigurationAttribute>(fs_filename));
}
