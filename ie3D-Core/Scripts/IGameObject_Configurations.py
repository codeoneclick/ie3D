#!/usr/bin/python
import sys
import os
import re
from xml.etree import ElementTree
from re import sub, split, UNICODE
from os import walk, listdir
from os.path import isfile, join, basename

kConfigurationsPath = "../ie3D-Bundle/Configurations"
kShaderConfiguratiomSample = "../ie3D-Bundle/Configurations/example.shader.xml"
kMaterialConfigurationSample = "../ie3D-Bundle/Configurations/example.material.xml"

def get_filepathes(directory, extension):
    filepathes = [] 

    for root, directories, files in os.walk(directory):
        for filename in files:

            filepath = os.path.join(root, filename)
            if filename.endswith(extension) and "example" in filename:
            	filepathes.append(filepath)

    return filepathes

def parse_xml(filename):
	document = ElementTree.parse(filename)
	root = document.getroot()

	class_name = root.get("class_name")
	base_class_name = root.get("base_class_name")
	is_external = root.get("is_external")

	source_h_file = open(class_name + '.h', 'w+')
	source_cpp_file = open(class_name + '.cpp', 'w+')
	source_cpp_file.write('// autogenerated: do not add any changes\n')
	
	source_h_file.write('// autogenerated: do not add any changes\n')
	source_h_file.write('#ifndef ' + class_name + '_h\n')
	source_h_file.write('#define ' + class_name + '_h\n')
	source_h_file.write('class ' + class_name + ' : public ' + base_class_name + '\n')
	source_h_file.write('{\n')
	
	source_h_file.write('public:\n')
	source_h_file.write(class_name + '(void) = default\n')
	source_h_file.write('~' + class_name + '(void) = default\n')

	for attribute in root.iter('attribute'):

		source_h_file.write(attribute.get('type') + ' ' + attribute.get("getter") + '(void) const;\n')
		source_cpp_file.write(attribute.get('type') + ' ' + class_name + '::' + attribute.get("getter") + '(void) const\n')
		source_cpp_file.write('{\n')
		source_cpp_file.write('const auto& iterator = m_attributes.find(\"' + attribute.get("path") + '/' + attribute.get("attribute_name") + '\");\n')
		source_cpp_file.write('assert(iterator != m_attributes.end());\n')
		source_cpp_file.write(attribute.get('type') + ' value; iterator->second->get(&value);\n')
		source_cpp_file.write('return value\n')
		source_cpp_file.write('}\n')

	for relationship in root.iter('relationship'):

		if relationship.get("is_to_many") == '0':

			source_h_file.write('std::shared_ptr<' + relationship.get("type") + '> ' + relationship.get("getter") + '(void) const;\n')
			source_cpp_file.write('std::shared_ptr<' + relationship.get("type") + '> ' + class_name + '::' + relationship.get("getter") + '(void) const\n')
			source_cpp_file.write('{\n')
			if relationship.get("is_external") == '0':
				source_cpp_file.write('const auto& iterator = m_configurations.find(\"' + relationship.get("path") + '/' + relationship.get("relationship_name") + '\");\n')
			else:
				source_cpp_file.write('const auto& iterator = m_configurations.find(\"' + relationship.get("filename") + '/' + relationship.get("relationship_name") + '\");\n')

			source_cpp_file.write('assert(iterator != m_configurations.end());\n')
			source_cpp_file.write('assert(iterator->second.size() != 0;\n')
			source_cpp_file.write('return std::static_pointer_cast<' + relationship.get("type") + '>(iterator->second.at(0));\n')

			source_cpp_file.write('}\n')

		else:

			source_h_file.write('std::vector<std::shared_ptr<' + relationship.get("type") + '>> ' + relationship.get("getter") + '(void) const;\n')
			source_cpp_file.write('std::vector<std::shared_ptr<' + relationship.get("type") + '>> ' + class_name + '::' + relationship.get("getter") + '(void) const\n')
			source_cpp_file.write('{\n')
			if relationship.get("is_external") == '0':
				source_cpp_file.write('const auto& iterator = m_configurations.find(\"' + relationship.get("path") + '/' + relationship.get("relationship_name") + '\");\n')
			else:
				source_cpp_file.write('const auto& iterator = m_configurations.find(\"' + relationship.get("filename") + '/' + relationship.get("relationship_name") + '\");\n')

			source_cpp_file.write('assert(iterator != m_configurations.end());\n')
			source_cpp_file.write('return iterator->second;\n')

			source_cpp_file.write('}\n')


	source_h_file.write('};\n')
	source_h_file.close()
	source_cpp_file.close()

def main():
	parse_xml(kShaderConfiguratiomSample)
	parse_xml(kMaterialConfigurationSample)

main()