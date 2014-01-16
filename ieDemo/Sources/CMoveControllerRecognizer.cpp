//
//  CMoveControllerRecognizer.cpp
//  indieEngineDemo
//
//  Created by Sergey Sergeev on 12/18/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CMoveControllerRecognizer.h"
#include "IMoveControllerHandler.h"
#include "HKOTHEnums.h"

CMoveControllerRecognizer::CMoveControllerRecognizer(void)
{

}

CMoveControllerRecognizer::~CMoveControllerRecognizer(void)
{
    
}

void CMoveControllerRecognizer::RegisterMoveControllerHandler(std::shared_ptr<IMoveControllerHandler> _handler)
{
    m_handlers.insert(_handler);
}

void CMoveControllerRecognizer::UnregisterMoveControllerHandler(std::shared_ptr<IMoveControllerHandler> _handler)
{
    m_handlers.erase(_handler);
}

void CMoveControllerRecognizer::_OnUpdate(ui32 _direction)
{
    for(const auto& handler : m_handlers)
    {
        handler->_Get_Commands()._ExecuteMoveControllerUpdateCommand(_direction);
    }
}

void CMoveControllerRecognizer::_OnInputTapRecognizerPressed(const glm::ivec2& _point)
{
    CMoveControllerRecognizer::_OnUpdate(E_MOVE_CONTROLLER_DIRECTION_NONE);
    m_point = _point;
}

void CMoveControllerRecognizer::_OnInputTapRecognizerMoved(const glm::ivec2& _point)
{
    if(_point.x < m_point.x && _point.y < m_point.y)
    {
        CMoveControllerRecognizer::_OnUpdate(E_MOVE_CONTROLLER_DIRECTION_NORTH_EAST);
    }
    else if(_point.x < m_point.x && _point.y > m_point.y)
    {
        CMoveControllerRecognizer::_OnUpdate(E_MOVE_CONTROLLER_DIRECTION_SOUTH_WEST);
    }
    else if(_point.x > m_point.x && _point.y < m_point.y)
    {
        CMoveControllerRecognizer::_OnUpdate(E_MOVE_CONTROLLER_DIRECTION_NORTH_WEST);
    }
    else if(_point.x > m_point.x && _point.y > m_point.y)
    {
        CMoveControllerRecognizer::_OnUpdate(E_MOVE_CONTROLLER_DIRECTION_SOUTH_EAST);
    }
    else if(_point.x < m_point.x)
    {
        CMoveControllerRecognizer::_OnUpdate(E_MOVE_CONTROLLER_DIRECTION_WEST);
    }
    else if(_point.x > m_point.x)
    {
        CMoveControllerRecognizer::_OnUpdate(E_MOVE_CONTROLLER_DIRECTION_EAST);
    }
    else if(_point.y < m_point.y)
    {
        CMoveControllerRecognizer::_OnUpdate(E_MOVE_CONTROLLER_DIRECTION_NORTH);
    }
    else if(_point.y > m_point.y)
    {
        CMoveControllerRecognizer::_OnUpdate(E_MOVE_CONTROLLER_DIRECTION_SOUTH);
    }
    else
    {
        CMoveControllerRecognizer::_OnUpdate(E_MOVE_CONTROLLER_DIRECTION_NONE);
    }
}

void CMoveControllerRecognizer::_OnInputTapRecognizerReleased(const glm::ivec2& _point)
{
    CMoveControllerRecognizer::_OnUpdate(E_MOVE_CONTROLLER_DIRECTION_NONE);
}