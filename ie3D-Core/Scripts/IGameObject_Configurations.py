#!/usr/bin/python
import sys
import os
import re
from xml.etree import ElementTree
from re import sub, split, UNICODE
from os import walk, listdir
from os.path import isfile, join, basename

kConfigurationsPath = "../ie3D-Bundle/Configurations"

def get_filepathes(directory, extension):
    filepathes = [] 

    for root, directories, files in os.walk(directory):
        for filename in files:

            filepath = os.path.join(root, filename)
            if filename.endswith(extension) and "example" in filename:
            	filepathes.append(filepath)

    return filepathes

def parse_example_xml(filename):
	document = ElementTree.parse(filename)
	root = document.getroot()
	source_h_file = open(root.tag, 'w+')

	for prop in root.iter('property'):
		source_h_file.write("%s" % prop.get('name'))


def main():
	configuration_files = get_filepathes(kConfigurationsPath, ".xml")
	for filename in configuration_files:
		parse_example_xml(filename)

main()