//
//  CMEGameController.h
//  indieEngine-EMap
//
//  Created by Sergey Sergeev on 4/20/14.
//
//

#ifndef CMEGameController_h
#define CMEGameController_h

#include "IGameController.h"
#include "HDeclaration.h"

class CMEGameController : public IGameController
{
private:
    
protected:
    
public:
    
    CMEGameController(ISharedOGLWindowRef window);
    ~CMEGameController(void) = default;
    
    std::shared_ptr<IGameTransition> createMainSceneTransition(const std::string& filename,
                                                               ISharedOGLWindowRef window);
    
    std::shared_ptr<IGameTransition> createModelsSceneTransition(const std::string& filename,
                                                                 ISharedOGLWindowRef window);
};

#endif
