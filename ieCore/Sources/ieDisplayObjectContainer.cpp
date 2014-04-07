//
//  ieDisplayObjectContainer.cpp
//  ieCore
//
//  Created by sergey.sergeev on 4/7/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#include "ieDisplayObjectContainer.h"
#include "ieEvent.h"

ieDisplayObjectContainer::ieDisplayObjectContainer(void) :
m_name(""),
m_parent(nullptr)
{
    m_description = "ieDisplayObjectContainer";
}

ieDisplayObjectContainer::~ieDisplayObjectContainer(void)
{
    m_childs.clear();
}

void ieDisplayObjectContainer::onUpdate(ieSharedEventRef event)
{
    std::for_each(m_childs.begin(), m_childs.end(), [event](std::shared_ptr<ieDisplayObjectContainer>& iterator){
        iterator->onUpdate(event);
    });
}

void ieDisplayObjectContainer::onDraw(ieSharedEventRef event)
{
    std::for_each(m_childs.begin(), m_childs.end(), [event](std::shared_ptr<ieDisplayObjectContainer>& iterator){
        iterator->onDraw(event);
    });
}

void ieDisplayObjectContainer::onAdded(ieSharedEventRef event)
{
    std::for_each(m_childs.begin(), m_childs.end(), [event](std::shared_ptr<ieDisplayObjectContainer>& iterator){
        iterator->onAdded(event);
    });
}

void ieDisplayObjectContainer::onRemoved(ieSharedEventRef event)
{
    std::for_each(m_childs.begin(), m_childs.end(), [event](std::shared_ptr<ieDisplayObjectContainer>& iterator){
        iterator->onRemoved(event);
    });
}

void ieDisplayObjectContainer::setName(const std::string& name)
{
    m_name = name;
}

std::string ieDisplayObjectContainer::getName(void) const
{
    return m_name;
}

void ieDisplayObjectContainer::addChild(ieSharedDisplayObjectContainerRef child)
{
    assert(m_childs.count(child) == 0);
    m_childs.insert(child);
    child->m_parent = std::static_pointer_cast<ieDisplayObjectContainer>(ieObject::shared_from_this());
    
    if(m_parent != nullptr)
    {
        ieSharedEvent eventOnStageAdded = std::make_shared<ieEvent>(kEVENT_ON_ADDED, child);
        eventOnStageAdded->addObject(m_resourceAccessor, "resourceAccessor");
        eventOnStageAdded->addObject(m_stage, "stage");
        eventOnStageAdded->addObject(m_camera, "camera");
        child->dispatchEvent(eventOnStageAdded);
    }
}

void ieDisplayObjectContainer::removeChild(ieSharedDisplayObjectContainerRef child)
{
    const auto& iterator = m_childs.find(child);
    if(iterator != m_childs.end())
    {
        m_childs.erase(child);
        if(m_parent != nullptr)
        {
            ieSharedEvent eventOnStageRemoved = std::make_shared<ieEvent>(kEVENT_ON_REMOVED, child);
            child->dispatchEvent(eventOnStageRemoved);
        }
    }
}

bool ieDisplayObjectContainer::contains(ieSharedDisplayObjectContainerRef child) const
{
    return m_childs.count(child) != 0;
}

ieSharedDisplayObjectContainer ieDisplayObjectContainer::getChildByName(const std::string& name) const
{
    return nullptr;
}