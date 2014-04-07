//
//  ieValue.h
//  ieCore
//
//  Created by sergey.sergeev on 4/7/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#ifndef __ieCore__ieValue__
#define __ieCore__ieValue__

#include "HCommon.h"
#include "HEnums.h"

template<typename T> struct type_name { static E_VALUE_TYPE name() { assert(false); } };
template<> struct type_name<bool> { static E_VALUE_TYPE name() {return E_VALUE_TYPE_BOOL;} };
template<> struct type_name<i8> { static E_VALUE_TYPE name() {return E_VALUE_TYPE_I8;} };
template<> struct type_name<ui8> { static E_VALUE_TYPE name() {return E_VALUE_TYPE_UI8;} };
template<> struct type_name<i16> { static E_VALUE_TYPE name() {return E_VALUE_TYPE_I16;} };
template<> struct type_name<ui16> { static E_VALUE_TYPE name() {return E_VALUE_TYPE_UI16;} };
template<> struct type_name<i32> { static E_VALUE_TYPE name() {return E_VALUE_TYPE_I32;} };
template<> struct type_name<ui32> { static E_VALUE_TYPE name() {return E_VALUE_TYPE_UI32;} };
template<> struct type_name<f32> { static E_VALUE_TYPE name() {return E_VALUE_TYPE_F32;} };

class ieValue
{
private:
    
protected:
    
    E_VALUE_TYPE m_type;
    std::tuple<bool, i8, ui8, i16, ui16, i32, ui32, f32> m_value;
    
public:
    
    ieValue(bool value)
    {
        m_type = type_name<bool>::name();
        m_value = std::make_tuple(value, 0, 0, 0, 0, 0, 0, 0);
    };
    
    ieValue(i8 value)
    {
        m_type = type_name<i8>::name();
        m_value = std::make_tuple(false, value, 0, 0, 0, 0, 0, 0);
    };
    
    ieValue(ui8 value)
    {
        m_type = type_name<ui8>::name();
        m_value = std::make_tuple(false, 0, value, 0, 0, 0, 0, 0);
    };
    
    ieValue(i16 value)
    {
        m_type = type_name<i16>::name();
        m_value = std::make_tuple(false, 0, 0, value, 0, 0, 0, 0);
    };
    
    ieValue(ui16 value)
    {
        m_type = type_name<ui16>::name();
        m_value = std::make_tuple(false, 0, 0, 0, value, 0, 0, 0);
    };
    
    ieValue(i32 value)
    {
        m_type = type_name<i32>::name();
        m_value = std::make_tuple(false, 0, 0, 0, 0, value, 0, 0);
    };
    
    ieValue(ui32 value)
    {
        m_type = type_name<ui32>::name();
        m_value = std::make_tuple(false, 0, 0, 0, 0, 0, value, 0);
    };
    
    ieValue(f32 value)
    {
        m_type = type_name<f32>::name();
        m_value = std::make_tuple(false, 0, 0, 0, 0, 0, 0, value);
    };
    
    template<class T_GET_VALUE>
    T_GET_VALUE getValue(void)
    {
        assert(type_name<T_GET_VALUE>::name() == m_type);
        switch (m_type) {
            case E_VALUE_TYPE_BOOL:
                return std::get<0>(m_value);
                break;
            case E_VALUE_TYPE_I8:
                return std::get<1>(m_value);
                break;
            case E_VALUE_TYPE_UI8:
                return std::get<2>(m_value);
                break;
            case E_VALUE_TYPE_I16:
                return std::get<3>(m_value);
                break;
            case E_VALUE_TYPE_UI16:
                return std::get<4>(m_value);
                break;
            case E_VALUE_TYPE_I32:
                return std::get<5>(m_value);
                break;
            case E_VALUE_TYPE_UI32:
                return std::get<6>(m_value);
                break;
            case E_VALUE_TYPE_F32:
                return std::get<7>(m_value);
                break;
                
            default:
                assert(false);
                break;
        }
        return 0;
    };
};

#endif
