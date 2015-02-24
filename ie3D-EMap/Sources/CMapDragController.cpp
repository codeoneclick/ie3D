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
#include "CLandscape.h"

#define SPACE_BUTTON 32
#define ALT_BUTTON 33

CMapDragController::CMapDragController(CSharedCameraRef camera,
                                       CSharedLandscapeRef landscape,
                                       f32 dragSpeed,
                                       const glm::vec3& maxBound,
                                       const glm::vec3& minBound) :
m_camera(camera),
m_landscape(landscape),
m_positionStarting(0),
m_positionEnding(0),
m_maxBound(maxBound),
m_minBound(minBound),
m_isMouseRightButtonPressed(false),
m_isSpaceButtonPressed(false),
m_dragSpeed(dragSpeed),
m_cameraLeftRightRotationSpeed(2.0),
m_cameraUpDownRotationSpeed(2.0),
m_cameraDistanceChangeSpeed(2.0)
{
    m_cameraPrecomputedRotationY = glm::degrees(m_camera->getRotation());
    m_cameraPrecomputedDistance = m_camera->getDistanceToLookAt();
}

CMapDragController::~CMapDragController(void)
{
    
}

void CMapDragController::onGestureRecognizerPressed(const glm::ivec2& point, E_INPUT_BUTTON inputButton)
{
    if(inputButton == E_INPUT_BUTTON_MOUSE_RIGHT)
    {
        m_isMouseRightButtonPressed = true;
        std::vector<std::tuple<glm::vec3, glm::vec3, glm::vec3>> triangles;
        triangles.push_back(std::make_tuple(glm::vec3(-4096.0, 0.0, -4096.0),
                                            glm::vec3( 4096.0, 0.0, -4096.0),
                                            glm::vec3(-4096.0, 0.0,  4096.0)));
        
        triangles.push_back(std::make_tuple(glm::vec3( 4096.0, 0.0,  4096.0),
                                            glm::vec3( 4096.0, 0.0, -4096.0),
                                            glm::vec3(-4096.0, 0.0,  4096.0)));
        
        CCollisionMgr::isTrianglesIntersected(m_camera, triangles, point, &m_positionStarting);
    }
    m_mouseLastPosition = point;
}

void CMapDragController::onGestureRecognizerMoved(const glm::ivec2&)
{
    
}

void CMapDragController::onGestureRecognizerDragged(const glm::ivec2& point, E_INPUT_BUTTON)
{
    std::vector<std::tuple<glm::vec3, glm::vec3, glm::vec3>> triangles;
    triangles.push_back(std::make_tuple(glm::vec3(-4096.0, 0.0, -4096.0),
                                        glm::vec3( 4096.0, 0.0, -4096.0),
                                        glm::vec3(-4096.0, 0.0,  4096.0)));
    
    triangles.push_back(std::make_tuple(glm::vec3( 4096.0, 0.0,  4096.0),
                                        glm::vec3( 4096.0, 0.0, -4096.0),
                                        glm::vec3(-4096.0, 0.0,  4096.0)));
    
    glm::vec3 position;
    if(CCollisionMgr::isTrianglesIntersected(m_camera, triangles, point, &position) && m_isMouseRightButtonPressed && !m_isSpaceButtonPressed)
    {
        m_positionEnding = m_positionStarting - position + m_camera->getLookAt();
        m_positionEnding.x = glm::min(m_positionEnding.x, m_minBound.x);
        m_positionEnding.z = glm::min(m_positionEnding.z, m_minBound.z);
        m_positionEnding.x = glm::max(m_positionEnding.x, m_maxBound.x);
        m_positionEnding.z = glm::max(m_positionEnding.z, m_maxBound.z);
    }
    else if(m_isMouseRightButtonPressed && m_isSpaceButtonPressed)
    {
        glm::ivec2 delta = m_mouseLastPosition - point;
        
        if(abs(delta.x) > abs(delta.y))
        {
            if(point.x < m_mouseLastPosition.x)
            {
                m_cameraPrecomputedRotationY -= m_cameraLeftRightRotationSpeed;
            }
            else if(point.x > m_mouseLastPosition.x)
            {
                m_cameraPrecomputedRotationY += m_cameraLeftRightRotationSpeed;
            }
        }
        else
        {
            if(point.y < m_mouseLastPosition.y)
            {
                m_cameraPrecomputedDistance.y -= m_cameraUpDownRotationSpeed;
            }
            else if(point.y > m_mouseLastPosition.y)
            {
                m_cameraPrecomputedDistance.y += m_cameraUpDownRotationSpeed;
            }
            m_cameraPrecomputedDistance.y = glm::clamp(m_cameraPrecomputedDistance.y, 2.0f, 128.0f);
        }
    }
    m_mouseLastPosition = point;
}

void CMapDragController::onGestureRecognizerReleased(const glm::ivec2&, E_INPUT_BUTTON inputButton)
{
    if(inputButton == E_INPUT_BUTTON_MOUSE_RIGHT)
    {
        m_isMouseRightButtonPressed = false;
    }
}

void CMapDragController::onGestureRecognizerWheelScroll(E_SCROLL_WHEEL_DIRECTION direction)
{
    if(m_isSpaceButtonPressed)
    {
        if(direction == E_SCROLL_WHEEL_DIRECTION_FORWARD)
        {
            m_cameraPrecomputedDistance.x -= m_cameraDistanceChangeSpeed;
            m_cameraPrecomputedDistance.z -= m_cameraDistanceChangeSpeed;
        }
        else if(direction == E_SCROLL_WHEEL_DIRECTION_BACKWARD)
        {
            m_cameraPrecomputedDistance.x += m_cameraDistanceChangeSpeed;
            m_cameraPrecomputedDistance.z += m_cameraDistanceChangeSpeed;
        }
        m_cameraPrecomputedDistance = glm::clamp(m_cameraPrecomputedDistance, 2.0f, 128.0f);
    }
}

void CMapDragController::onKeyUp(i32 key)
{
    switch (key)
    {
        case SPACE_BUTTON:
        {
            m_isSpaceButtonPressed = false;
        }
            break;
        case ALT_BUTTON:
        {
            
        }
            break;
        default:
            break;
    }
}

void CMapDragController::onKeyDown(i32 key)
{
    switch (key)
    {
        case SPACE_BUTTON:
        {
            m_isSpaceButtonPressed = true;
        }
            break;
            
        default:
            break;
    }
}

void CMapDragController::update(f32)
{
    glm::vec3 cameraLookAt;
    cameraLookAt = glm::mix(m_camera->getLookAt(), m_positionEnding, m_dragSpeed);
    m_camera->setLookAt(cameraLookAt);
    
    glm::vec3 cameraPosition = m_camera->getPosition();
    if(cameraPosition.x < m_landscape->getHeightmapSize().x &&
       cameraPosition.z < m_landscape->getHeightmapSize().y &&
       cameraPosition.x >= 0.0 &&
       cameraPosition.z >= 0.0)
    {
        f32 landscapeHeight = m_landscape->getHeight(cameraPosition);
        if(landscapeHeight > m_camera->getDistanceToLookAt().y + m_camera->getLookAt().y)
        {
            m_cameraPrecomputedDistance.y = landscapeHeight + 2.0f;
        }
    }
    
    f32 currentCameraRotation = glm::mix(glm::degrees(m_camera->getRotation()), m_cameraPrecomputedRotationY, 0.1);
    m_camera->setRotation(glm::radians(currentCameraRotation));
    
    glm::vec3 currentCameraDistanceToLookAt = glm::mix(m_camera->getDistanceToLookAt(), m_cameraPrecomputedDistance, 0.1);
    m_camera->setDistanceToLookAt(currentCameraDistanceToLookAt);
}

void CMapDragController::setMaxBound(const glm::vec3 &maxBound)
{
    m_maxBound = maxBound;
}

void CMapDragController::setMinBound(const glm::vec3 &minBound)
{
    m_minBound = minBound;
}
