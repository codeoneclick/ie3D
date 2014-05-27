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
#include "HDeclaration.h"

class CMapDragController;

class CEditableScene final : public IScene
{
private:
    
protected:
    
    std::shared_ptr<CMapDragController> m_mapDragController;
    CSharedSkyBox m_skyBox;
    CSharedLandscape m_landscape;
    
    std::vector<ISharedGameObject> colliders(void);
    void onCollision(const glm::vec3& position, ISharedGameObjectRef gameObject);
    
public:
    
    CEditableScene(IGameTransition* root);
    ~CEditableScene(void);
    
    void load(void);
    void update(f32 deltatime);
};

#endif
