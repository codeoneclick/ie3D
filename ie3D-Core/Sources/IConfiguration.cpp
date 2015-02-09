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

std::map<GLenum, std::string> g_enumGLToString = {
    {GL_FRONT, "GL_FRONT"},
    {GL_BACK, "GL_BACK"},
    {GL_SRC_ALPHA, "GL_SRC_ALPHA"},
    {GL_ONE, "GL_ONE"},
    {GL_ONE_MINUS_SRC_ALPHA, "GL_ONE_MINUS_SRC_ALPHA"},
    {GL_REPEAT, "GL_REPEAT"},
    {GL_CLAMP_TO_EDGE, "GL_CLAMP_TO_EDGE"},
    {GL_MIRRORED_REPEAT, "GL_MIRRORED_REPEAT"},
    {GL_NEAREST, "GL_NEAREST"},
    {GL_LINEAR, "GL_LINEAR"},
    {GL_LINEAR_MIPMAP_NEAREST, "GL_MIPMAP"}
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

IConfigurationLoadingHandler::IConfigurationLoadingHandler(void) :
m_configuration(nullptr)
{
    
}

IConfigurationLoadingHandler::~IConfigurationLoadingHandler(void)
{
    m_callbacks.clear();
}

void IConfigurationLoadingHandler::onConfigurationLoaded(ISharedConfigurationRef configuration, bool success)
{
    if(success)
    {
        assert(configuration != nullptr);
        m_configuration = configuration;
        
        std::for_each(m_callbacks.begin(), m_callbacks.end(), [configuration](CONFIGURATION_LOADED_CALLBACK callback){
            callback(configuration);
        });
    }
}
void IConfigurationLoadingHandler::addConfigurationLoadedCallback(const CONFIGURATION_LOADED_CALLBACK& callback)
{
    if(m_configuration == nullptr)
    {
        m_callbacks.push_back(callback);
    }
    else
    {
        callback(m_configuration);
    }
}
void IConfigurationLoadingHandler::removeConfigurationLoadedCallback(const CONFIGURATION_LOADED_CALLBACK& callback)
{
    /*const auto& iterator = std::find(m_callbacks.begin(), m_callbacks.end(), callback);
    m_callbacks.erase(iterator);*/
}

IConfiguration::~IConfiguration(void)
{
    m_attributes.clear();
    m_configurations.clear();
}

void IConfiguration::setAttribute(const std::string& attributeName,
                                  CSharedConfigurationAttributeRef attribute)
{
    m_attributes[attributeName] = attribute;
}

void IConfiguration::setConfiguration(const std::string& configurationName,
                                      ISharedConfigurationRef configuration,
                                      i32 index)
{
    const auto& iterator = m_configurations.find(configurationName);
    if(iterator != m_configurations.end())
    {
        if(index >= 0 && index < iterator->second.size())
        {
            iterator->second[index] = configuration;
        }
        else
        {
            iterator->second.push_back(configuration);
        }
    }
    else
    {
        std::vector<ISharedConfiguration> configurations;
        configurations.push_back(configuration);
        m_configurations[configurationName] = configurations;
    }
}

pugi::xml_parse_result IConfiguration::openXMLDocument(pugi::xml_document &document,
                                                       const std::string &filename)
{
    pugi::xml_parse_result result;
    result = document.load_file(bundlepath().append(filename).c_str());
    if(result.status == pugi::status_ok)
    {
        IConfiguration::setFilename(filename);
    }
    return result;
};

#if defined(__EDITOR__)

void IConfiguration::setFilename(const std::string& filename)
{
    m_filename = filename;
}

#endif

std::string IConfiguration::getFilename(void) const
{
    return m_filename;
}

