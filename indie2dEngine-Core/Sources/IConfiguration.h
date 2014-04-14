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
    
public:
    
    CConfigurationAttribute(bool value)
    {
        m_scalarType = type_name<bool>::name();
        m_scalars = std::make_tuple(value, 0, 0, 0, 0, 0, 0, 0);
    };
    
    CConfigurationAttribute(i8 value)
    {
        m_scalarType = type_name<i8>::name();
        m_scalars = std::make_tuple(false, value, 0, 0, 0, 0, 0, 0);
    };
    
    CConfigurationAttribute(ui8 value)
    {
        m_scalarType = type_name<ui8>::name();
        m_scalars = std::make_tuple(false, 0, value, 0, 0, 0, 0, 0);
    };
    
    CConfigurationAttribute(i16 value)
    {
        m_scalarType = type_name<i16>::name();
        m_scalars = std::make_tuple(false, 0, 0, value, 0, 0, 0, 0);
    };
    
    CConfigurationAttribute(ui16 value)
    {
        m_scalarType = type_name<ui16>::name();
        m_scalars = std::make_tuple(false, 0, 0, 0, value, 0, 0, 0);
    };
    
    CConfigurationAttribute(i32 value)
    {
        m_scalarType = type_name<i32>::name();
        m_scalars = std::make_tuple(false, 0, 0, 0, 0, value, 0, 0);
    };
    
    CConfigurationAttribute(ui32 value)
    {
        m_scalarType = type_name<ui32>::name();
        m_scalars = std::make_tuple(false, 0, 0, 0, 0, 0, value, 0);
    };
    
    CConfigurationAttribute(f32 value)
    {
        m_scalarType = type_name<f32>::name();
        m_scalars = std::make_tuple(false, 0, 0, 0, 0, 0, 0, value);
    };
    
    CConfigurationAttribute(const std::string& string)
    {
        m_string = string;
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

class IConfiguration
{
private:
    
protected:
    
    std::unordered_map<std::string, std::vector<CSharedConfigurationAttribute>> m_attributes;
    std::unordered_map<std::string, std::vector<ISharedConfiguration>> m_configurations;
    
    IConfiguration(void);
    
public:
    
    ~IConfiguration(void);
    
    void setAttribute(const std::string& attributeName,
                      CSharedConfigurationAttributeRef attribute,
                      bool isReplacing = false,
                      ui32 replacingIndex = 0);
    
    void setConfiguration(const std::string& configurationName,
                          ISharedConfigurationRef configuration,
                          bool isReplacing = false,
                          ui32 replacingIndex = 0);
};

#endif
