//
//  CMapDragController.h
//  ie3D-EMap
//
//  Created by Sergey Sergeev on 4/23/14.
//
//

#ifndef CMapDragController_h
#define CMapDragController_h

#include "HCommon.h"
#include "HDeclaration.h"
#include "IInputContext.h"

class CMapDragController : public IGestureRecognizerHandler
{
private:
    
protected:
    
    CSharedCamera m_camera;
    CSharedLandscape m_landscape;
    
    glm::vec3 m_positionStarting;
    glm::vec3 m_positionEnding;
    
    glm::vec3 m_maxBound;
    glm::vec3 m_minBound;
    std::vector<std::tuple<glm::vec3, glm::vec3, glm::vec3>> m_triangles;
    
    bool m_isPressed;
    f32 m_dragSpeed;
    
    void onGestureRecognizerPressed(const glm::ivec2& point, E_INPUT_BUTTON inputButton);
    void onGestureRecognizerMoved(const glm::ivec2& point);
    void onGestureRecognizerDragged(const glm::ivec2& point, E_INPUT_BUTTON inputButton);
    void onGestureRecognizerReleased(const glm::ivec2& point, E_INPUT_BUTTON inputButton);
    void onGestureRecognizerWheelScroll(E_SCROLL_WHEEL_DIRECTION);
    
    void onKeyUp(i32 key);
    void onKeyDown(i32 key);
    
public:
    
    void update(f32 deltatime);
    
    CMapDragController(CSharedCameraRef camera,
                       CSharedLandscapeRef landscape,
                       f32 dragSpeed,
                       const glm::vec3& maxBound,
                       const glm::vec3& minBound);
    
    ~CMapDragController(void);
    
    void setMaxBound(const glm::vec3& maxBound);
    void setMinBound(const glm::vec3& minBound);
};

#endif
