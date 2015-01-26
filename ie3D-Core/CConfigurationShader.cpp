// autogenerated: do not add any changes
std::string CConfigurationShader::getVSFilename(void) const
{
const auto& iterator = m_attributes.find("/shader/vs_filename");
assert(iterator != m_attributes.end());
std::string value; iterator->second->get(&value);
return value
}
std::string CConfigurationShader::getFSFilename(void) const
{
const auto& iterator = m_attributes.find("/shader/fs_filename");
assert(iterator != m_attributes.end());
std::string value; iterator->second->get(&value);
return value
}