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
    
    glm::vec3 m_positionStarting;
    glm::vec3 m_positionEnding;
    
    glm::vec3 m_maxBound;
    glm::vec3 m_minBound;
    
    bool m_isPressed;
    f32 m_dragSpeed;
    
    void onGestureRecognizerPressed(const glm::ivec2& point, bool isRightButton = false);
    void onGestureRecognizerMoved(const glm::ivec2& point);
    void onGestureRecognizerReleased(const glm::ivec2& point, bool isRightButton = false);
    
public:
    
    void update(f32 deltatime);
    
    CMapDragController(CSharedCameraRef camera,
                       f32 dragSpeed,
                       const glm::vec3& maxBound,
                       const glm::vec3& minBound);
    
    ~CMapDragController(void);
    
    void setMaxBound(const glm::vec3& maxBound);
    void setMinBound(const glm::vec3& minBound);
};

#endif