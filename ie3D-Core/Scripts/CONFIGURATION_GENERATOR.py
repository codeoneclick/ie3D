#!/usr/bin/python
import sys
import os
import re
from xml.etree import ElementTree
from re import sub, split, UNICODE
from os import walk, listdir
from os.path import isfile, join, basename
from sets import Set

kConfiguratiomPath = "../../ie3D-Bundle/Configurations/"

kConfigurationAutogeneratedSourcesPath = "../Autogenerated/"
kConfigurationAccessorClassName = "CConfigurationAccessor"
kConfigurationDecrlarationsFilename = "HConfigurationDeclarations"

def get_configuration_samples(directory, extension):
    filepathes = [] 

    for root, directories, files in os.walk(directory):
        for filename in files:

            filepath = os.path.join(root, filename)
            if filename.endswith(extension) and "sample" in filename:
            	filepathes.append(filepath)

    return filepathes


def get_attribute_type_converter(attribute_type):
 	if attribute_type == 'bool':
 		return 'as_bool()'
 	elif attribute_type == 'i64' or attribute_type == 'i32' or attribute_type == 'i16' or attribute_type == 'i8':
 		return 'as_int()'
 	elif attribute_type == 'ui64' or attribute_type == 'ui32' or attribute_type == 'ui16' or attribute_type == 'ui8':
 		return 'as_uint()'
 	elif attribute_type == 'f32':
 		return 'as_float()'
 	elif attribute_type == 'std::string':
 		return 'as_string()'
 	raise Exception('unknown attribute type')


def write_attributes_serializer(source_cpp_file, attributes):
 	for attribute in attributes:

		source_cpp_file.write(attribute.get('type') + ' ' + attribute.get("attribute_name") + ' = node.node().attribute("'+ attribute.get("attribute_name") + '").' + get_attribute_type_converter(attribute.get('type')) +';\n')
		if attribute.get('is_convert_to_ogl_enum') == '1':

			source_cpp_file.write('assert(g_stringToGLenum.find('+ attribute.get("attribute_name") +') != g_stringToGLenum.end());\n')
			source_cpp_file.write('GLenum '+ attribute.get("attribute_name") + 'Enum' + ' = g_stringToGLenum.find('+ attribute.get("attribute_name") +')->second;\n')
			source_cpp_file.write('IConfiguration::setAttribute("' + attribute.get("path") + '/' + attribute.get("attribute_name") + '", std::make_shared<CConfigurationAttribute>(' + attribute.get("attribute_name") + 'Enum));\n')

		else:

			source_cpp_file.write('IConfiguration::setAttribute("' + attribute.get("path") + '/' + attribute.get("attribute_name") + '", std::make_shared<CConfigurationAttribute>(' + attribute.get("attribute_name") + '));\n')
			



def write_relationships_serializer(source_cpp_file, relationships):
	for relationship in relationships:

		if relationship.get("is_to_many") == '0':

			source_cpp_file.write('std::shared_ptr<' + relationship.get('type') + '> ' + relationship.get('relationship_name') + ' = std::make_shared<' + relationship.get('type') + '>();\n')

			if relationship.get("is_external") == '0':

				source_cpp_file.write(relationship.get('relationship_name') + '->serialize(document, "' + relationship.get('path') + '");\n')

			else:

				source_cpp_file.write('pugi::xpath_node ' + relationship.get('relationship_name') + '_node' + ' = document.select_single_node("' + relationship.get('path') + '/' + relationship.get('relationship_name') + '");\n')
				source_cpp_file.write(relationship.get('relationship_name') + '->serialize(' + relationship.get('relationship_name') + '_node' + '.node().attribute("filename").as_string());\n')

			source_cpp_file.write('IConfiguration::setConfiguration("' + relationship.get('path') + '/' + relationship.get("relationship_name") + '", ' + relationship.get('relationship_name') + ');\n')

		else:

			source_cpp_file.write('pugi::xpath_node_set ' + relationship.get('relationship_name') + '_nodes = document.select_nodes("' + relationship.get('path') + '/' + relationship.get('relationship_name') + '");\n')
			source_cpp_file.write('for (pugi::xpath_node_set::const_iterator iterator = ' + relationship.get('relationship_name') + '_nodes.begin(); iterator != ' + relationship.get('relationship_name') + '_nodes.end(); ++iterator)\n')
			source_cpp_file.write('{\n')
			source_cpp_file.write('std::shared_ptr<' + relationship.get('type') + '> ' + relationship.get('relationship_name') + ' = std::make_shared<' + relationship.get('type') + '>();\n')

			if relationship.get("is_external") == '0':

				source_cpp_file.write('pugi::xpath_node node = (*iterator);\n')
				source_cpp_file.write(relationship.get('relationship_name') + '->serialize(document, node);\n')

			else:

				source_cpp_file.write(relationship.get('relationship_name') + '->serialize((*iterator).node().attribute("filename").as_string());\n')

			source_cpp_file.write('IConfiguration::setConfiguration("' + relationship.get('path') + '/' + relationship.get("relationship_name") + '", ' + relationship.get('relationship_name') + ');\n')
			source_cpp_file.write('}\n')
			


def parse_xml(filename, accessor_class_source_h_file, accessor_class_source_cpp_file):
	document = ElementTree.parse(filename)
	root = document.getroot()

	class_name = root.get("class_name")
	base_class_name = root.get("base_class_name")
	is_external = root.get("is_external")

	source_h_file = open(kConfigurationAutogeneratedSourcesPath + class_name + '.h', 'w+')
	source_cpp_file = open(kConfigurationAutogeneratedSourcesPath + class_name + '.cpp', 'w+')
	source_cpp_file.write('// autogenerated: do not add any changes\n')
	source_cpp_file.write('#include "'+ class_name + '.h"\n')
	
	source_h_file.write('// autogenerated: do not add any changes\n')
	source_h_file.write('#ifndef ' + class_name + '_h\n')
	source_h_file.write('#define ' + class_name + '_h\n')
	source_h_file.write('#include "IConfiguration.h"\n')
	for relationship in root.iter('relationship'):
		source_h_file.write('#include "' + relationship.get("type") + '.h"\n')

	source_h_file.write('class ' + class_name + ' : public ' + base_class_name + '\n')
	source_h_file.write('{\n')
	
	source_h_file.write('public:\n')
	source_h_file.write(class_name + '(void) = default;\n')
	source_h_file.write('~' + class_name + '(void) = default;\n')

	for attribute in root.iter('attribute'):

		if attribute.get('is_convert_to_ogl_enum') == '0':

			source_h_file.write(attribute.get('type') + ' ' + attribute.get("getter") + '(void) const;\n')
			source_cpp_file.write(attribute.get('type') + ' ' + class_name + '::' + attribute.get("getter") + '(void) const\n')

		else:

			source_h_file.write('GLenum ' + attribute.get("getter") + '(void) const;\n')
			source_cpp_file.write('GLenum ' + class_name + '::' + attribute.get("getter") + '(void) const\n')

		source_cpp_file.write('{\n')
		source_cpp_file.write('const auto& iterator = m_attributes.find(\"' + attribute.get("path") + '/' + attribute.get("attribute_name") + '\");\n')
		source_cpp_file.write('assert(iterator != m_attributes.end());\n')
		if attribute.get('is_convert_to_ogl_enum') == '0':
			source_cpp_file.write(attribute.get('type') + ' value; iterator->second->get(&value);\n')
		else:
			source_cpp_file.write('GLenum value; iterator->second->get(&value);\n')
		source_cpp_file.write('return value;\n')
		source_cpp_file.write('}\n')

	for relationship in root.iter('relationship'):

		if relationship.get("is_to_many") == '0':

			source_h_file.write('std::shared_ptr<' + relationship.get("type") + '> ' + relationship.get("getter") + '(void) const;\n')
			source_cpp_file.write('std::shared_ptr<' + relationship.get("type") + '> ' + class_name + '::' + relationship.get("getter") + '(void) const\n')
			source_cpp_file.write('{\n')
			source_cpp_file.write('const auto& iterator = m_configurations.find(\"' + relationship.get("path") + '/' + relationship.get("relationship_name") + '\");\n')
			source_cpp_file.write('if(iterator == m_configurations.end())\n')
			source_cpp_file.write('{\n')
			source_cpp_file.write('return nullptr;\n')
			source_cpp_file.write('}\n')
			source_cpp_file.write('assert(iterator != m_configurations.end());\n')
			source_cpp_file.write('assert(iterator->second.size() != 0);\n')
			source_cpp_file.write('return std::static_pointer_cast<' + relationship.get("type") + '>(iterator->second.at(0));\n')
			source_cpp_file.write('}\n')

		else:

			source_h_file.write('std::vector<std::shared_ptr<IConfiguration>> ' + relationship.get("getter") + '(void) const;\n')
			source_cpp_file.write('std::vector<std::shared_ptr<IConfiguration>> ' + class_name + '::' + relationship.get("getter") + '(void) const\n')
			source_cpp_file.write('{\n')
			source_cpp_file.write('const auto& iterator = m_configurations.find(\"' + relationship.get("path") + '/' + relationship.get("relationship_name") + '\");\n')
			source_cpp_file.write('if(iterator == m_configurations.end())\n')
			source_cpp_file.write('{\n')
			source_cpp_file.write('return std::vector<std::shared_ptr<IConfiguration>>();\n')
			source_cpp_file.write('}\n')
			source_cpp_file.write('assert(iterator != m_configurations.end());\n')
			source_cpp_file.write('return iterator->second;\n')
			source_cpp_file.write('}\n')

	if is_external == "0":

		source_h_file.write('void serialize(pugi::xml_document& document, const std::string& path);\n')
		source_cpp_file.write('void ' + class_name + '::serialize(pugi::xml_document& document, const std::string& path)\n')
		source_cpp_file.write('{\n')
		source_cpp_file.write('pugi::xpath_node node;\n')
		source_cpp_file.write('node = document.select_single_node((path + "/' + root.tag + '").c_str());\n')

	else:

		source_h_file.write('void serialize(const std::string& filename);\n')
		source_cpp_file.write('void ' + class_name + '::serialize(const std::string& filename)\n')
		source_cpp_file.write('{\n')
		source_cpp_file.write('pugi::xml_document document;\n')
		source_cpp_file.write('pugi::xml_parse_result result = IConfiguration::openXMLDocument(document, filename);\n')
		source_cpp_file.write('assert(result.status == pugi::status_ok);\n')
		source_cpp_file.write('pugi::xpath_node node;\n')
		source_cpp_file.write('node = document.select_single_node("/' + root.tag + '");\n')

	write_attributes_serializer(source_cpp_file, root.iter('attribute'))
	write_relationships_serializer(source_cpp_file, root.iter('relationship'))

	source_cpp_file.write('}\n')

	if is_external == "0":

		source_h_file.write('void serialize(pugi::xml_document& document, pugi::xpath_node& node);\n')
		source_cpp_file.write('void ' + class_name + '::serialize(pugi::xml_document& document, pugi::xpath_node& node)\n')
		source_cpp_file.write('{\n')
		write_attributes_serializer(source_cpp_file, root.iter('attribute'))
		write_relationships_serializer(source_cpp_file, root.iter('relationship'))
		source_cpp_file.write('}\n')

	source_h_file.write('};\n')
	source_h_file.write('#endif\n')
	source_h_file.close()
	source_cpp_file.close()

def delete_character(string, n): 
    begin = string[:n]  
    end = string[n+1:] 
    return begin + end

def write_acessor_external_headers(configurations, accessor_class_source_h_file):
	
	for configuration in configurations:

		document = ElementTree.parse(configuration)
		root = document.getroot()
		class_name = root.get("class_name")
		is_external = root.get("is_external")
		if is_external == '1':
			accessor_class_source_h_file.write('#include "' + class_name + '.h"\n')


def write_acessor_header_getters(configurations, accessor_class_source_h_file):

	for configuration in configurations:

		document = ElementTree.parse(configuration)
		root = document.getroot()
		class_name = root.get("class_name")
		is_external = root.get("is_external")
		if is_external == '1':
			accessor_class_source_h_file.write('void get' + delete_character(class_name, 0) + '(const std::string& filename, const std::shared_ptr<IConfigurationLoadingHandler>& handler) const;\n')


def write_acessor_source_getters(configurations, accessor_class_source_cpp_file):

	for configuration in configurations:

		document = ElementTree.parse(configuration)
		root = document.getroot()
		class_name = root.get("class_name")
		is_external = root.get("is_external")
		if is_external == '1':

			accessor_class_source_cpp_file.write('void ' + kConfigurationAccessorClassName +'::get' + delete_character(class_name, 0) + '(const std::string& filename, const std::shared_ptr<IConfigurationLoadingHandler>& handler) const\n')
			accessor_class_source_cpp_file.write('{\n')
			accessor_class_source_cpp_file.write('std::shared_ptr<' + class_name + '> configuration = std::make_shared<' + class_name + '>();\n')
			accessor_class_source_cpp_file.write('configuration->serialize(filename);\n')
			accessor_class_source_cpp_file.write('assert(configuration != nullptr);\n')
			accessor_class_source_cpp_file.write('assert(handler != nullptr);\n')
			accessor_class_source_cpp_file.write('handler->onConfigurationLoaded(configuration, true);\n')
			accessor_class_source_cpp_file.write('}\n')

def write_classes_declarations(configurations, declaration_file):
	
	configurationClasses = Set([])
	for configuration in configurations:

		document = ElementTree.parse(configuration)
		root = document.getroot()
		class_name = root.get("class_name")
		if class_name not in configurationClasses:

			declaration_file.write('class ' + class_name + ';\n')
			declaration_file.write('typedef std::shared_ptr<' + class_name + '> CShared' + delete_character(class_name, 0) + ';\n')
			declaration_file.write('typedef const CShared' + delete_character(class_name, 0) +'& CShared' + delete_character(class_name, 0) + 'Ref;\n')
			configurationClasses.add(class_name)


def main():

	configurations = get_configuration_samples(kConfiguratiomPath, ".xml")

	declaration_file = open(kConfigurationAutogeneratedSourcesPath + kConfigurationDecrlarationsFilename + '.h', 'w+')
	declaration_file.write('#ifndef ' + kConfigurationDecrlarationsFilename + '_h\n')
	declaration_file.write('#define ' + kConfigurationDecrlarationsFilename + '_h\n')
	write_classes_declarations(configurations, declaration_file)
	declaration_file.write('#endif\n')
	declaration_file.close()

	accessor_class_source_h_file = open(kConfigurationAutogeneratedSourcesPath + kConfigurationAccessorClassName + '.h', 'w+')
	accessor_class_source_cpp_file = open(kConfigurationAutogeneratedSourcesPath + kConfigurationAccessorClassName + '.cpp', 'w+')

	accessor_class_source_h_file.write('// autogenerated: do not add any changes\n')
	accessor_class_source_h_file.write('#ifndef ' + kConfigurationAccessorClassName + '_h\n')
	accessor_class_source_h_file.write('#define ' + kConfigurationAccessorClassName + '_h\n')
	write_acessor_external_headers(configurations, accessor_class_source_h_file)
	accessor_class_source_h_file.write('class ' + kConfigurationAccessorClassName + '\n')
	accessor_class_source_h_file.write('{\n')
	accessor_class_source_h_file.write('public:\n')
	accessor_class_source_h_file.write(kConfigurationAccessorClassName + '(void) = default;\n')
	accessor_class_source_h_file.write('~' + kConfigurationAccessorClassName + '(void) = default;\n')
	write_acessor_header_getters(configurations, accessor_class_source_h_file)
	
	accessor_class_source_cpp_file.write('// autogenerated: do not add any changes\n')
	accessor_class_source_cpp_file.write('#include "'+ kConfigurationAccessorClassName + '.h"\n')
	write_acessor_source_getters(configurations, accessor_class_source_cpp_file)

	for configuration in configurations:
		parse_xml(configuration, accessor_class_source_h_file, accessor_class_source_cpp_file)

	accessor_class_source_h_file.write('};\n')
	accessor_class_source_h_file.write('#endif\n')

	accessor_class_source_h_file.close()
	accessor_class_source_cpp_file.close()

main()