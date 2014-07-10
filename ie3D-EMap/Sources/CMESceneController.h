//
//  CMESceneController.h
//  indieEngine-EMap
//
//  Created by Sergey Sergeev on 4/20/14.
//
//

#ifndef CMESceneController_h
#define CMESceneController_h

#include "IGameController.h"
#include "HDeclaration.h"

class CMESceneController : public IGameController
{
private:
    
protected:
    
public:
    
    CMESceneController(void) = default;
    ~CMESceneController(void) = default;
    
    std::shared_ptr<IGameTransition> createEditableSceneTransition(const std::string& filename,
                                                                   ISharedOGLWindowRef window);
};

#endif 
