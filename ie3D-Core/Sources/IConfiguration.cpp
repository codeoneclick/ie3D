//
//  IConfiguration.cpp
//  indieEngineCore
//
//  Created by Sergey Sergeev on 12/20/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "IConfiguration.h"
#include "CCommonOS.h"

#if defined(__IOS__) || defined(__OSX__) || defined(__NDK__)

std::map<std::string, GLenum> g_stringToGLenum = {
    {"GL_FRONT", GL_FRONT},
    {"GL_BACK", GL_BACK},
    {"GL_SRC_ALPHA", GL_SRC_ALPHA},
    {"GL_ONE", GL_ONE},
    {"GL_ONE_MINUS_SRC_ALPHA", GL_ONE_MINUS_SRC_ALPHA},
    {"GL_REPEAT", GL_REPEAT},
    {"GL_CLAMP_TO_EDGE", GL_CLAMP_TO_EDGE },
    {"GL_MIRRORED_REPEAT", GL_MIRRORED_REPEAT },
    {"GL_NEAREST", GL_NEAREST },
    {"GL_LINEAR", GL_LINEAR },
    {"GL_MIPMAP", GL_LINEAR_MIPMAP_NEAREST }
};

std::map<GLenum, std::string> g_glenumToString = {
    {GL_FRONT, "GL_FRONT"},
    {GL_BACK, "GL_BACK"},
    {GL_SRC_ALPHA, "GL_SRC_ALPHA"},
    {GL_ONE, "GL_ONE"},
    {GL_ONE_MINUS_SRC_ALPHA, "GL_ONE_MINUS_SRC_ALPHA"},
    {GL_REPEAT, "GL_REPEAT"},
    {GL_CLAMP_TO_EDGE, "GL_CLAMP_TO_EDGE"},
    {GL_MIRRORED_REPEAT, "GL_MIRRORED_REPEAT"}
};

#elif defined(__WIN32__)

static std::map<std::string, GLenum> g_glenumToString(void)
{
    static std::map<std::string, GLenum> container;
    if(container.size() == 0)
    {
        container.insert(std::make_pair("GL_FRONT", GL_FRONT));
        container.insert(std::make_pair("GL_BACK", GL_BACK));
        container.insert(std::make_pair("GL_SRC_ALPHA", GL_SRC_ALPHA));
        container.insert(std::make_pair("GL_ONE", GL_ONE));
        container.insert(std::make_pair("GL_ONE_MINUS_SRC_ALPHA", GL_ONE_MINUS_SRC_ALPHA));
        container.insert(std::make_pair("GL_REPEAT", GL_REPEAT));
        container.insert(std::make_pair("GL_CLAMP_TO_EDGE", GL_CLAMP_TO_EDGE));
        container.insert(std::make_pair("GL_MIRRORED_REPEAT", GL_MIRRORED_REPEAT));
    }
    return container;
};

#endif

IConfiguration::~IConfiguration(void)
{
    m_attributes.clear();
    m_configurations.clear();
}

void IConfiguration::setAttribute(const std::string& attributeName,
                                  CSharedConfigurationAttributeRef attribute)
{
    m_attributes.insert(std::make_pair(attributeName, attribute));
}

void IConfiguration::setConfiguration(const std::string &configurationName,
                                      ISharedConfigurationRef configuration,
                                      bool isReplacing,
                                      ui32 replacingIndex)
{
    const auto& iterator = m_configurations.find(configurationName);
    if(iterator != m_configurations.end())
    {
        if(isReplacing && replacingIndex < iterator->second.size())
        {
            iterator->second[replacingIndex] = configuration;
        } else {
            iterator->second.push_back(configuration);
        }
    } else {
        std::vector<ISharedConfiguration> configurations;
        configurations.push_back(configuration);
        m_configurations.insert(std::make_pair(configurationName, configurations));
    }
}

pugi::xml_parse_result IConfiguration::openXMLDocument(pugi::xml_document &document,
                                                       const std::string &filename)
{
    pugi::xml_parse_result result;
    result = document.load_file(bundlepath().append(filename).c_str());
    return result;
};

