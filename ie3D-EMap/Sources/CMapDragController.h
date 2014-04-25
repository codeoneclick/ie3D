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
    
    glm::ivec2 m_point;
    
    void onGestureRecognizerPressed(const glm::ivec2& point);
    void onGestureRecognizerMoved(const glm::ivec2& point);
    void onGestureRecognizerReleased(const glm::ivec2& point);
    
public:
    
    CMapDragController(CSharedCameraRef camera);
    ~CMapDragController(void);
};

#endif
