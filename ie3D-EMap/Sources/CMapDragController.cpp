//
//  CMapDragController.cpp
//  ie3D-EMap
//
//  Created by Sergey Sergeev on 4/23/14.
//
//

#include "CMapDragController.h"

CMapDragController::CMapDragController(CSharedCameraRef camera) :
m_camera(camera),
m_point(0)
{
    
}

CMapDragController::~CMapDragController(void)
{
    
}

void CMapDragController::onGestureRecognizerPressed(const glm::ivec2& point)
{

}

void CMapDragController::onGestureRecognizerMoved(const glm::ivec2& point)
{
    if(point.x < m_point.x && point.y < m_point.y)
    {
       
    }
    else if(point.x < m_point.x && point.y > m_point.y)
    {
        
    }
    else if(point.x > m_point.x && point.y < m_point.y)
    {
        
    }
    else if(point.x > m_point.x && point.y > m_point.y)
    {
        
    }
    else if(point.x < m_point.x)
    {
        
    }
    else if(point.x > m_point.x)
    {
        
    }
    else if(point.y < m_point.y)
    {
        
    }
    else if(point.y > m_point.y)
    {
        
    }
    else
    {
        
    }
    m_point = point;
}

void CMapDragController::onGestureRecognizerReleased(const glm::ivec2& point)
{

}