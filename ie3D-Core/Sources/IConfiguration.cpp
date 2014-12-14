//
//  IConfiguration.cpp
//  indieEngineCore
//
//  Created by Sergey Sergeev on 12/20/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "IConfiguration.h"

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

IConfigurationLoadingHandler::IConfigurationLoadingHandler(void)
{
    
}

IConfigurationLoadingHandler::~IConfigurationLoadingHandler(void)
{

}

void IConfigurationLoadingHandler::onConfigurationLoaded(ISharedConfigurationRef configuration, bool success)
{
    if(success)
    {
        m_configurations.insert(configuration);
        std::for_each(m_commands.begin(), m_commands.end(), [configuration](CONFIGURATION_LOADING_COMMAND command){
            command(configuration);
        });
    }
}

void IConfigurationLoadingHandler::addConfigurationLoadingCommand(const CONFIGURATION_LOADING_COMMAND& command)
{
    std::for_each(m_configurations.begin(), m_configurations.end(), [command](ISharedConfiguration configuration){
        command(configuration);
    });
    m_commands.push_back(command);
}

void IConfigurationLoadingHandler::removeConfigurationLoadingCommand(const CONFIGURATION_LOADING_COMMAND& command)
{
    /*const auto& iterator = std::find(m_commands.begin(), m_commands.end(), command);
    m_commands.erase(iterator);*/
}

IConfiguration::IConfiguration(E_CONFIGURATION_CLASS configurationClass) :
m_configurationClass(configurationClass)
{
    
}

IConfiguration::~IConfiguration(void)
{
    m_attributes.clear();
    m_configurations.clear();
}

E_CONFIGURATION_CLASS IConfiguration::getConfigurationClass(void) const
{
    return m_configurationClass;
}

void IConfiguration::setAttribute(const std::string& attributeName,
                                  CSharedConfigurationAttributeRef attribute,
                                  bool isReplacing,
                                  ui32 replacingIndex)
{
    const auto& iterator = m_attributes.find(attributeName);
    if(iterator != m_attributes.end())
    {
        if(isReplacing && replacingIndex < iterator->second.size())
        {
            iterator->second[replacingIndex] = attribute;
        } else {
            iterator->second.push_back(attribute);
        }
    } else {
        std::vector<CSharedConfigurationAttribute> attributes;
        attributes.push_back(attribute);
        m_attributes.insert(std::make_pair(attributeName, attributes));
    }
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

