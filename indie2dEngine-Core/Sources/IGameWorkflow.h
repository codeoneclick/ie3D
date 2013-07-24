//
//  IGameWorkflow.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 6/6/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef IGameWorkflow_h
#define IGameWorkflow_h

#include "HCommon.h"

class CTemplateAccessor;
class CResourceAccessor;
class IGameTransition;
class IGameWorkflow
{
private:
    
protected:
    
    std::map<std::string, std::shared_ptr<IGameTransition> > m_transitions;
    std::shared_ptr<IGameTransition> m_currentTransition;
    
    std::shared_ptr<CTemplateAccessor> m_templateAccessor;
    std::shared_ptr<CResourceAccessor> m_resourceAccessor;
    
public:
    
    IGameWorkflow(void);
    virtual ~IGameWorkflow(void);
    
    void RegisterTransition(std::shared_ptr<IGameTransition> _transition);
    void UnregisterTransition(std::shared_ptr<IGameTransition> _transition);
    
    void GoToTransition(const std::string& _guid);
    
    ui32 Get_CurrentNumTriagles(void);
    ui32 Get_TotalNumTriangles(void);
};

extern ui32 Get_CurrentNumTriagles(void);
extern ui32 Get_TotalNumTriangles(void);

#endif 
