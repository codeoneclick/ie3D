//
//  IGameController.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 6/6/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef IGameController_h
#define IGameController_h

#include "HCommon.h"
#include "HDeclaration.h"

class IGameController
{
private:
    
protected:
    
    std::map<std::string, ISharedGameTransition> m_transitions;
    ISharedGameTransition m_currentTransition;
    
    std::shared_ptr<IGraphicsContext> m_graphicsContext;
    std::shared_ptr<IInputContext> m_gestureRecognizerContext;
    
    std::shared_ptr<CConfigurationAccessor> m_configurationAccessor;
    std::shared_ptr<CResourceAccessor> m_resourceAccessor;
    
public:
    
    IGameController(ISharedOGLWindowRef window);
    virtual ~IGameController(void);
    
    void addTransition(ISharedGameTransitionRef transition);
    void removeTransition(ISharedGameTransitionRef transition);
    
    void gotoTransition(const std::string& guid);
    
    ui32 getCurrentNumTriagles(void);
    ui32 getTotalNumTriangles(void);
};

#endif 
