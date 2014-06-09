//
//  CMapDragController.cpp
//  ie3D-EMap
//
//  Created by Sergey Sergeev on 4/23/14.
//
//

#include "CMapDragController.h"
#include "CCollisionMgr.h"
#include "CCamera.h"

CMapDragController::CMapDragController(CSharedCameraRef camera,
                                       f32 dragSpeed,
                                       const glm::vec3& maxBound,
                                       const glm::vec3& minBound) :
m_camera(camera),
m_positionStarting(0),
m_positionEnding(0),
m_maxBound(maxBound),
m_minBound(minBound),
m_isPressed(false),
m_dragSpeed(dragSpeed)
{
    
}

CMapDragController::~CMapDragController(void)
{
    
}

void CMapDragController::onGestureRecognizerPressed(const glm::ivec2& point, E_INPUT_BUTTON inputButton)
{
    if(inputButton == E_INPUT_BUTTON_MOUSE_RIGHT)
    {
        m_isPressed = true;
        CCollisionMgr::isIntersected(m_camera, point, &m_positionStarting);
    }
}

void CMapDragController::onGestureRecognizerMoved(const glm::ivec2&)
{
    
}

void CMapDragController::onGestureRecognizerDragged(const glm::ivec2& point, E_INPUT_BUTTON)
{
    glm::vec3 position;
    if(CCollisionMgr::isIntersected(m_camera, point, &position) && m_isPressed)
    {
        m_positionEnding = m_positionStarting - position + m_camera->Get_LookAt();
        m_positionEnding.x = glm::min(m_positionEnding.x, m_minBound.x);
        m_positionEnding.z = glm::min(m_positionEnding.z, m_minBound.z);
        m_positionEnding.x = glm::max(m_positionEnding.x, m_maxBound.x);
        m_positionEnding.z = glm::max(m_positionEnding.z, m_maxBound.z);
    }
}

void CMapDragController::onGestureRecognizerReleased(const glm::ivec2&, E_INPUT_BUTTON inputButton)
{
    if(inputButton == E_INPUT_BUTTON_MOUSE_RIGHT)
    {
        m_isPressed = false;
    }
}

void CMapDragController::update(f32)
{
    glm::vec3 position;
    position = glm::mix(m_camera->Get_LookAt(), m_positionEnding, m_dragSpeed);
    m_camera->Set_LookAt(position);
}

void CMapDragController::setMaxBound(const glm::vec3 &maxBound)
{
    m_maxBound = maxBound;
}

void CMapDragController::setMinBound(const glm::vec3 &minBound)
{
    m_minBound = minBound;
}
