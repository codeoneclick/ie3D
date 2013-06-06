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

class IGameTransition;
class IGameWorkflow
{
private:
    
protected:
    
    std::map<std::string, std::shared_ptr<IGameTransition> > m_transitions;
    std::shared_ptr<IGameTransition> m_currentTransition;
    
public:
    
    IGameWorkflow(void);
    ~IGameWorkflow(void);
    
    void RegisterTransition(std::shared_ptr<IGameTransition> _transition);
    void UnregisterTransition(std::shared_ptr<IGameTransition> _transition);
    
    void EnableTransition(const std::string& _guid);
    void DisableTransition(const std::string& _guid);
    
    bool Forward(void);
    bool Backward(void);
};

#endif 
