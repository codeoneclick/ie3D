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

template<typename T> struct type_name { static E_CONFIGURATION_ATTRIBUTE_SCALAR name() { assert(false); } };
template<> struct type_name<bool> { static E_CONFIGURATION_ATTRIBUTE_SCALAR name() { return E_CONFIGURATION_ATTRIBUTE_SCALAR_BOOL; } };
template<> struct type_name<i8> { static E_CONFIGURATION_ATTRIBUTE_SCALAR name() { return E_CONFIGURATION_ATTRIBUTE_SCALAR_I8; } };
template<> struct type_name<ui8> { static E_CONFIGURATION_ATTRIBUTE_SCALAR name() { return E_CONFIGURATION_ATTRIBUTE_SCALAR_UI8; } };
template<> struct type_name<i16> { static E_CONFIGURATION_ATTRIBUTE_SCALAR name() { return E_CONFIGURATION_ATTRIBUTE_SCALAR_I16; } };
template<> struct type_name<ui16> { static E_CONFIGURATION_ATTRIBUTE_SCALAR name() { return E_CONFIGURATION_ATTRIBUTE_SCALAR_UI16; } };
template<> struct type_name<i32> { static E_CONFIGURATION_ATTRIBUTE_SCALAR name() { return E_CONFIGURATION_ATTRIBUTE_SCALAR_I32; } };
template<> struct type_name<ui32> { static E_CONFIGURATION_ATTRIBUTE_SCALAR name() { return E_CONFIGURATION_ATTRIBUTE_SCALAR_UI32; } };
template<> struct type_name<f32> { static E_CONFIGURATION_ATTRIBUTE_SCALAR name() { return E_CONFIGURATION_ATTRIBUTE_SCALAR_F32; } };

class CConfigurationAttribute
{
private:
    
protected:
    
    E_CONFIGURATION_ATTRIBUTE_SCALAR m_scalarType;
    std::tuple<bool, i8, ui8, i16, ui16, i32, ui32, f32> m_scalars;
    std::string m_string;
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
        m_scalarType = type_name<bool>::name();
        m_scalars = std::make_tuple(value, 0, 0, 0, 0, 0, 0, 0);
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
        m_scalarType = type_name<i8>::name();
        m_scalars = std::make_tuple(false, value, 0, 0, 0, 0, 0, 0);
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
        m_scalarType = type_name<ui8>::name();
        m_scalars = std::make_tuple(false, 0, value, 0, 0, 0, 0, 0);
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
        m_scalarType = type_name<i16>::name();
        m_scalars = std::make_tuple(false, 0, 0, value, 0, 0, 0, 0);
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
        m_scalarType = type_name<ui16>::name();
        m_scalars = std::make_tuple(false, 0, 0, 0, value, 0, 0, 0);
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
        m_scalarType = type_name<i32>::name();
        m_scalars = std::make_tuple(false, 0, 0, 0, 0, value, 0, 0);
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
        m_scalarType = type_name<ui32>::name();
        m_scalars = std::make_tuple(false, 0, 0, 0, 0, 0, value, 0);
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
        m_scalarType = type_name<f32>::name();
        m_scalars = std::make_tuple(false, 0, 0, 0, 0, 0, 0, value);
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
        m_string = value;
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
    
    template<class T_GET_VALUE>
    T_GET_VALUE getScalar(void) const
    {
        assert(type_name<T_GET_VALUE>::name() == m_scalarType);
        switch (m_scalarType) {
            case E_CONFIGURATION_ATTRIBUTE_SCALAR_BOOL:
                return std::get<0>(m_scalars);
                break;
            case E_CONFIGURATION_ATTRIBUTE_SCALAR_I8:
                return std::get<1>(m_scalars);
                break;
            case E_CONFIGURATION_ATTRIBUTE_SCALAR_UI8:
                return std::get<2>(m_scalars);
                break;
            case E_CONFIGURATION_ATTRIBUTE_SCALAR_I16:
                return std::get<3>(m_scalars);
                break;
            case E_CONFIGURATION_ATTRIBUTE_SCALAR_UI16:
                return std::get<4>(m_scalars);
                break;
            case E_CONFIGURATION_ATTRIBUTE_SCALAR_I32:
                return std::get<5>(m_scalars);
                break;
            case E_CONFIGURATION_ATTRIBUTE_SCALAR_UI32:
                return std::get<6>(m_scalars);
                break;
            case E_CONFIGURATION_ATTRIBUTE_SCALAR_F32:
                return std::get<7>(m_scalars);
                break;
            default:
                assert(false);
                break;
        }
        return 0;
    };
    
    std::string getString(void) const
    {
        return m_string;
    };
};

#if defined(__IOS__) || defined(__OSX__) || defined(__NDK__)

extern std::map<std::string, GLenum> g_stringToGLenum;
extern std::map<GLenum, std::string> g_glenumToString;

#elif defined(__WIN32__)

#endif

template <class ...Args>
std::string getConfigurationAttributeKey(const Args&... args)
{
    std::vector<std::string> keys = {args...};
    assert(keys.size() != 0);
    std::string key = "";
    for (ui32 i = 0; i < (keys.size() - 1); ++i)
    {
        key.append(keys[i] + ":");
    }
    key.append(keys[keys.size() - 1]);
    assert(key.length() != 0);
    return key;
}

class IConfigurationLoadingHandler
{
public:
    
    typedef std::function<void(ISharedConfigurationRef)> CONFIGURATION_LOADING_COMMAND;
    
private:
    
protected:
    
    IConfigurationLoadingHandler(void);
    std::vector<CONFIGURATION_LOADING_COMMAND> m_commands;
    std::set<ISharedConfiguration> m_configurations;

public:
    
    virtual ~IConfigurationLoadingHandler(void);
    
    virtual void onConfigurationLoaded(ISharedConfigurationRef configuration, bool success);
    
    void addConfigurationLoadingCommand(const CONFIGURATION_LOADING_COMMAND& command);
    void removeConfigurationLoadingCommand(const CONFIGURATION_LOADING_COMMAND& command);
};

class IConfiguration
{
private:
    
protected:
    
    E_CONFIGURATION_CLASS m_configurationClass;
    std::unordered_map<std::string, CSharedConfigurationAttribute> m_attributes;
    std::unordered_map<std::string, std::vector<ISharedConfiguration>> m_configurations;
    
    IConfiguration(E_CONFIGURATION_CLASS configurationClass);
    
public:
    
    IConfiguration(void) = default;
    
    ~IConfiguration(void);
    
    E_CONFIGURATION_CLASS getConfigurationClass(void) const;
    
    void setAttribute(const std::string& attributeName,
                      CSharedConfigurationAttributeRef attribute);
    
    void setConfiguration(const std::string& configurationName,
                          ISharedConfigurationRef configuration,
                          bool isReplacing = false,
                          ui32 replacingIndex = 0);
    
    pugi::xml_parse_result openXMLDocument(pugi::xml_document &document,
                                           const std::string &filename)
    {
        pugi::xml_parse_result result;
        return result;
    };
};

#endif
