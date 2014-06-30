//
//  CEditableScene.h
//  indieEngine-EMap
//
//  Created by Sergey Sergeev on 4/20/14.
//
//

#ifndef CEditableScene_h
#define CEditableScene_h

#include "IScene.h"
#include "HMEDeclaration.h"

class CMapDragController;

class CEditableScene final :
public IScene,
public IGestureRecognizerHandler
{
private:
    
protected:
    
    std::shared_ptr<CMapDragController> m_mapDragController;
    CSharedSkyBox m_skyBox;
    CSharedLandscape m_landscape;
    CSharedModel m_model;
    CSharedSelectionArea m_selectionArea;
    glm::ivec2 m_previousDraggedPoint;
    ui32 m_editableRadius;
    
    std::vector<ISharedGameObject> colliders(void);
    void onCollision(const glm::vec3& position, ISharedGameObjectRef gameObject, E_INPUT_BUTTON inputButton);
    
    void onGestureRecognizerPressed(const glm::ivec2& point, E_INPUT_BUTTON inputButton);
    void onGestureRecognizerDragged(const glm::ivec2& point, E_INPUT_BUTTON inputButton);
    void onGestureRecognizerMoved(const glm::ivec2& point);
    void onGestureRecognizerReleased(const glm::ivec2& point, E_INPUT_BUTTON inputButton);
    void onGestureRecognizerWheelScroll(E_SCROLL_WHEEL_DIRECTION direction);
    
public:
    
    CEditableScene(IGameTransition* root);
    ~CEditableScene(void);
    
    void load(void);
    void update(f32 deltatime);
};

#endif
