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
    
    glm::vec3 m_targetPosition;
    glm::vec3 m_currentPosition;
    
    bool m_isPressed;
    f32 m_dragSpeed;
    
    void onGestureRecognizerPressed(const glm::ivec2& point, bool isRightButton = false);
    void onGestureRecognizerMoved(const glm::ivec2& point);
    void onGestureRecognizerReleased(const glm::ivec2& point, bool isRightButton = false);
    
public:
    
    void update(f32 deltatime);
    
    CMapDragController(CSharedCameraRef camera, f32 dragSpeed);
    ~CMapDragController(void);
};

#endif
