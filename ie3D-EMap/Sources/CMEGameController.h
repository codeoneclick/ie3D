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
    
    CMEGameController(void) = default;
    ~CMEGameController(void) = default;
    
    std::shared_ptr<IGameTransition> createEditableGameTransition(const std::string& filename,
                                                                  ISharedOGLWindowRef window);
};

#endif
