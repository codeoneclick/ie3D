//
//  ieObject.cpp
//  ieCore
//
//  Created by sergey.sergeev on 4/7/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#include "ieObject.h"

ieObject::ieObject(void)
{
    m_description = "ieObject";
}

ieObject::~ieObject(void)
{
    m_objects.clear();
}

std::string ieObject::getDescription(void) const
{
    return m_description;
}

void ieObject::addObject(ieSharedObjectRef object, const std::string& key)
{
    const auto& iterator = m_objects.find(key);
    assert(iterator == m_objects.end());
    m_objects.insert(std::make_pair(key, object));
}

void ieObject::removeObject(const std::string& key)
{
    const auto& iterator = m_objects.find(key);
    assert(iterator != m_objects.end());
    m_objects.erase(iterator);
}

void ieObject::setObject(ieSharedObjectRef object, const std::string& key)
{
    const auto& iterator = m_objects.find(key);
    assert(iterator != m_objects.end());
    iterator->second = object;
}

ieSharedObject ieObject::getObject(const std::string& key) const
{
    auto iterator = m_objects.find(key);
    return iterator != m_objects.end() ? iterator->second : nullptr;
}

void ieObject::addValue(ieSharedValueRef value, const std::string& key)
{
    const auto& iterator = m_values.find(key);
    assert(iterator == m_values.end());
    m_values.insert(std::make_pair(key, value));
}

void ieObject::removeValue(const std::string& key)
{
    const auto& iterator = m_values.find(key);
    assert(iterator != m_values.end());
    m_values.erase(iterator);
}

void ieObject::setValue(ieSharedValueRef value, const std::string& key)
{
    const auto& iterator = m_values.find(key);
    assert(iterator != m_values.end());
    iterator->second = value;
}

ieSharedValue ieObject::getValue(const std::string& key) const
{
    auto iterator = m_values.find(key);
    return iterator != m_values.end() ? iterator->second : nullptr;
}
