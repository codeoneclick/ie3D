//
//  ieEvent.cpp
//  ieCore
//
//  Created by sergey.sergeev on 4/7/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#include "ieEvent.h"

ieEvent::ieEvent(const std::string& type,
                 ieSharedEventDispatcherRef target):
m_type(type),
m_target(target)
{
    m_description = "ieEvent";
}

ieEvent::~ieEvent(void)
{
    
}

std::string ieEvent::getType(void) const
{
    return m_type;
}

ieSharedEventDispatcher ieEvent::getTarget(void) const
{
    return m_target;
}