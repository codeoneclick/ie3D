//
//  ieEventDispatcher.cpp
//  ieCore
//
//  Created by sergey.sergeev on 4/7/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#include "ieEventDispatcher.h"
#include "ieEvent.h"

ieEventDispatcher::ieEventDispatcher(void)
{
    m_description = "ieEventDispatcher";
}

ieEventDispatcher::~ieEventDispatcher(void)
{
    for(auto iterator : m_functions)
    {
        iterator.second.clear();
    }
    m_functions.clear();
}

void ieEventDispatcher::addEventListener(const std::string& type,
                                         ieEventDispatcherFunctionSharedRef function)
{
    auto iterator = m_functions.find(type);
    if(iterator == m_functions.end())
    {
        std::set<ieEventDispatcherFunctionShared> functions;
        functions.insert(function);
        m_functions.insert(std::make_pair(type, functions));
        return;
    }
    iterator->second.insert(function);
}

void ieEventDispatcher::removeEventListener(const std::string& type,
                                            ieEventDispatcherFunctionSharedRef function)
{
    const auto& iterator = m_functions.find(type);
    if(iterator != m_functions.end())
    {
        iterator->second.erase(function);
    }
}

bool ieEventDispatcher::hasEventListener(const std::string& type)
{
    auto iterator = m_functions.find(type);
    return iterator != m_functions.end();
}

void ieEventDispatcher::dispatchEvent(ieSharedEventRef event)
{
    assert(event->getTarget() != nullptr);
    ieSharedEventDispatcher dispatcher = std::static_pointer_cast<ieEventDispatcher>(event->getTarget());
    if(dispatcher->hasEventListener(event->getType()))
    {
        const auto& iterator = dispatcher->m_functions.find(event->getType());
        for(const auto& function : iterator->second)
        {
            (*function)(event);
        }
    }
}
