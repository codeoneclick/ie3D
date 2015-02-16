//
//  IConfiguration.h
//  indieEngineCore
//
//  Created by Sergey Sergeev on 12/20/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef IConfiguration_h
#define IConfiguration_h

#include "HCommon.h"
#include "HEnums.h"
#include "HDeclaration.h"

class CConfigurationAttribute
{
private:
    
protected:
    
    std::tuple<std::shared_ptr<bool>,
    std::shared_ptr<i8>,
    std::shared_ptr<ui8>,
    std::shared_ptr<i16>,
    std::shared_ptr<ui16>,
    std::shared_ptr<i32>,
    std::shared_ptr<ui32>,
    std::shared_ptr<f32>,
    std::shared_ptr<std::string>
    > m_container;
    
public:
    
    CConfigurationAttribute(bool value)
    {
        m_container = std::make_tuple(std::make_shared<bool>(value),
                                      nullptr,
                                      nullptr,
                                      nullptr,
                                      nullptr,
                                      nullptr,
                                      nullptr,
                                      nullptr,
                                      nullptr);
    };
    
    CConfigurationAttribute(i8 value)
    {
        m_container = std::make_tuple(nullptr,
                                      std::make_shared<i8>(value),
                                      nullptr,
                                      nullptr,
                                      nullptr,
                                      nullptr,
                                      nullptr,
                                      nullptr,
                                      nullptr);
    };
    
    CConfigurationAttribute(ui8 value)
    {
        m_container = std::make_tuple(nullptr,
                                      nullptr,
                                      std::make_shared<ui8>(value),
                                      nullptr,
                                      nullptr,
                                      nullptr,
                                      nullptr,
                                      nullptr,
                                      nullptr);
    };
    
    CConfigurationAttribute(i16 value)
    {
        m_container = std::make_tuple(nullptr,
                                      nullptr,
                                      nullptr,
                                      std::make_shared<i16>(value),
                                      nullptr,
                                      nullptr,
                                      nullptr,
                                      nullptr,
                                      nullptr);
    };
    
    CConfigurationAttribute(ui16 value)
    {
        m_container = std::make_tuple(nullptr,
                                      nullptr,
                                      nullptr,
                                      nullptr,
                                      std::make_shared<ui16>(value),
                                      nullptr,
                                      nullptr,
                                      nullptr,
                                      nullptr);
    };
    
    CConfigurationAttribute(i32 value)
    {
        m_container = std::make_tuple(nullptr,
                                      nullptr,
                                      nullptr,
                                      nullptr,
                                      nullptr,
                                      std::make_shared<i32>(value),
                                      nullptr,
                                      nullptr,
                                      nullptr);
    };
    
    CConfigurationAttribute(ui32 value)
    {
        m_container = std::make_tuple(nullptr,
                                      nullptr,
                                      nullptr,
                                      nullptr,
                                      nullptr,
                                      nullptr,
                                      std::make_shared<ui32>(value),
                                      nullptr,
                                      nullptr);
    };
    
    CConfigurationAttribute(f32 value)
    {
        m_container = std::make_tuple(nullptr,
                                      nullptr,
                                      nullptr,
                                      nullptr,
                                      nullptr,
                                      nullptr,
                                      nullptr,
                                      std::make_shared<f32>(value),
                                      nullptr);
    };
    
    CConfigurationAttribute(const std::string& value)
    {
        m_container = std::make_tuple(nullptr,
                                      nullptr,
                                      nullptr,
                                      nullptr,
                                      nullptr,
                                      nullptr,
                                      nullptr,
                                      nullptr,
                                      std::make_shared<std::string>(value));
    };
    
    void get(bool *value) const
    {
        (*value) = (*std::get<0>(m_container).get());
    };
    
    void get(i8 *value) const
    {
        (*value) = (*std::get<1>(m_container).get());
    };
    
    void get(ui8 *value) const
    {
        (*value) = (*std::get<2>(m_container).get());
    };
    
    void get(i16 *value) const
    {
        (*value) = (*std::get<3>(m_container).get());
    };
    
    void get(ui16 *value) const
    {
        (*value) = (*std::get<4>(m_container).get());
    };
    
    void get(i32 *value) const
    {
        (*value) = (*std::get<5>(m_container).get());
    };
    
    void get(ui32 *value) const
    {
        (*value) = (*std::get<6>(m_container).get());
    };
    
    void get(f32 *value) const
    {
        (*value) = (*std::get<7>(m_container).get());
    };
    
    void get(std::string *value) const
    {
        (*value) = (*std::get<8>(m_container).get());
    };
};

#if defined(__IOS__) || defined(__OSX__) || defined(__NDK__)

extern std::map<std::string, GLenum> g_stringToGLenum;
extern std::map<GLenum, std::string> g_enumGLToString;

#elif defined(__WIN32__)

#endif

class IConfigurationLoadingHandler
{
public:
    
    typedef std::function<void(ISharedConfigurationRef)> CONFIGURATION_LOADED_CALLBACK;
    
private:
    
protected:
    
    IConfigurationLoadingHandler(void);
    ISharedConfiguration m_configuration;
    std::vector<CONFIGURATION_LOADED_CALLBACK> m_callbacks;

public:
    
    virtual ~IConfigurationLoadingHandler(void);
    virtual void onConfigurationLoaded(ISharedConfigurationRef configuration, bool success);
    
    void addConfigurationLoadedCallback(const CONFIGURATION_LOADED_CALLBACK& callback);
    void removeConfigurationLoadedCallback(const CONFIGURATION_LOADED_CALLBACK& callback);
};

class IConfiguration
{
private:
    
protected:
    
#if defined(__EDITOR__)
    
    bool m_isEnabled;
    
#endif

    std::string m_filename;
    std::unordered_map<std::string, CSharedConfigurationAttribute> m_attributes;
    std::unordered_map<std::string, std::vector<ISharedConfiguration>> m_configurations;
    
    IConfiguration(void);
    
public:
    
    virtual ~IConfiguration(void);
    
    void setAttribute(const std::string& attributeName,
                      CSharedConfigurationAttributeRef attribute);
    
    void setConfiguration(const std::string& configurationName,
                          ISharedConfigurationRef configuration,
                          i32 index = -1);
    
    pugi::xml_parse_result openXMLDocument(pugi::xml_document &document,
                                           const std::string &filename);
   
#if defined(__EDITOR__)
    
    void setFilename(const std::string& filename);
    void setEnabled(bool value);
    bool getEnabled(void) const;
    
#endif
    
    std::string getFilename(void) const;
    
};

class IConfigurationGameObject : public IConfiguration
{
private:
    
protected:
    
public:
    
    IConfigurationGameObject() = default;
    virtual ~IConfigurationGameObject(void) = default;
    virtual  std::vector<ISharedConfiguration> getMaterialsConfigurations(void) const = 0;
};

#endif
