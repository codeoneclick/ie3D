//
//  CMoveController.cpp
//  indieEngineDemo
//
//  Created by Sergey Sergeev on 12/18/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CMoveController.h"
#include "HKOTHEnums.h"

CMoveController::CMoveController(void)
{

}

CMoveController::~CMoveController(void)
{
    m_handlers.clear();
}

void CMoveController::addMoveControllerHandler(ISharedMoveControllerHandlerRef handler)
{
    m_handlers.insert(handler);
}

void CMoveController::removeMoveControllerHandler(ISharedMoveControllerHandlerRef handler)
{
    m_handlers.erase(handler);
}

void CMoveController::update(ui32 direction)
{
    for(const auto& handler : m_handlers)
    {
        handler->onMoveControllerUpdate(direction);
    }
}

void CMoveController::onGestureRecognizerPressed(const glm::ivec2& point)
{
    CMoveController::update(E_MOVE_CONTROLLER_DIRECTION_NONE);
    m_point = point;
}

void CMoveController::onGestureRecognizerMoved(const glm::ivec2& point)
{
    if(point.x < m_point.x && point.y < m_point.y)
    {
        CMoveController::update(E_MOVE_CONTROLLER_DIRECTION_NORTH_EAST);
    }
    else if(point.x < m_point.x && point.y > m_point.y)
    {
        CMoveController::update(E_MOVE_CONTROLLER_DIRECTION_SOUTH_WEST);
    }
    else if(point.x > m_point.x && point.y < m_point.y)
    {
        CMoveController::update(E_MOVE_CONTROLLER_DIRECTION_NORTH_WEST);
    }
    else if(point.x > m_point.x && point.y > m_point.y)
    {
        CMoveController::update(E_MOVE_CONTROLLER_DIRECTION_SOUTH_EAST);
    }
    else if(point.x < m_point.x)
    {
        CMoveController::update(E_MOVE_CONTROLLER_DIRECTION_WEST);
    }
    else if(point.x > m_point.x)
    {
        CMoveController::update(E_MOVE_CONTROLLER_DIRECTION_EAST);
    }
    else if(point.y < m_point.y)
    {
        CMoveController::update(E_MOVE_CONTROLLER_DIRECTION_NORTH);
    }
    else if(point.y > m_point.y)
    {
        CMoveController::update(E_MOVE_CONTROLLER_DIRECTION_SOUTH);
    }
    else
    {
        CMoveController::update(E_MOVE_CONTROLLER_DIRECTION_NONE);
    }
}

void CMoveController::onGestureRecognizerReleased(const glm::ivec2& point)
{
    CMoveController::update(E_MOVE_CONTROLLER_DIRECTION_NONE);
}