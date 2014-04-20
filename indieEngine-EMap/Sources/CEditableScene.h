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

class CEditableScene final : public IScene
{
private:
    
protected:
    
    void _OnCollision(const glm::vec3& position,
                      std::shared_ptr<IGameObject> collider);
    
public:
    
    CEditableScene(IGameTransition* root);
    ~CEditableScene(void);
    
    void load(void);
    void update(f32 deltatime);
};

#endif
