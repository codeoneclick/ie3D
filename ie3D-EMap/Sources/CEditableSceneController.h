//
//  CEditableSceneController.h
//  indieEngine-EMap
//
//  Created by Sergey Sergeev on 4/20/14.
//
//

#ifndef CEditableSceneController_h
#define CEditableSceneController_h

#include "IGameController.h"
#include "HDeclaration.h"

class CEditableSceneController : public IGameController
{
private:
    
protected:
    
public:
    
    CEditableSceneController(void) = default;
    ~CEditableSceneController(void) = default;
    
    std::shared_ptr<IGameTransition> createEditableSceneTransition(const std::string& filename,
                                                                   ISharedOGLWindowRef window);
};

#endif 
