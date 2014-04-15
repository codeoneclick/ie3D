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

class CConfigurationAccessor;
class CResourceAccessor;
class IGameTransition;
class IGameController
{
private:
    
protected:
    
    std::map<std::string, std::shared_ptr<IGameTransition> > m_transitions;
    std::shared_ptr<IGameTransition> m_currentTransition;
    
    std::shared_ptr<CConfigurationAccessor> m_templateAccessor;
    std::shared_ptr<CResourceAccessor> m_resourceAccessor;
    
public:
    
    IGameController(void);
    virtual ~IGameController(void);
    
    void RegisterTransition(std::shared_ptr<IGameTransition> _transition);
    void UnregisterTransition(std::shared_ptr<IGameTransition> _transition);
    
    void GoToTransition(const std::string& _guid);
    
    ui32 Get_CurrentNumTriagles(void);
    ui32 Get_TotalNumTriangles(void);
};

extern ui32 Get_CurrentNumTriagles(void);
extern ui32 Get_TotalNumTriangles(void);

#endif 
