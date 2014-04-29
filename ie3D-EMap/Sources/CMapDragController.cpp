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

CMapDragController::CMapDragController(CSharedCameraRef camera, f32 dragSpeed) :
m_camera(camera),
m_targetPosition(0),
m_currentPosition(0),
m_isPressed(false),
m_dragSpeed(dragSpeed)
{
    
}

CMapDragController::~CMapDragController(void)
{
    
}

void CMapDragController::onGestureRecognizerPressed(const glm::ivec2& point, bool isRightButton)
{
    m_isPressed = isRightButton;
    CCollisionMgr::isIntersected(m_camera, point, &m_targetPosition);
}

void CMapDragController::onGestureRecognizerMoved(const glm::ivec2& point)
{
    if(CCollisionMgr::isIntersected(m_camera, point, &m_currentPosition) && m_isPressed)
    {
        m_currentPosition = m_targetPosition - m_currentPosition + m_camera->Get_LookAt();
    }
}

void CMapDragController::onGestureRecognizerReleased(const glm::ivec2&, bool isRightButton)
{
    m_isPressed = !isRightButton;
}

void CMapDragController::update(f32)
{
    glm::vec3 position;
    position = glm::mix(m_camera->Get_LookAt(), m_currentPosition, m_dragSpeed);
    m_camera->Set_LookAt(position);
}
