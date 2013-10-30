//
//  CIESAWorkflow.h
//  indieEngineShader
//
//  Created by Sergey Sergeev on 10/30/13.
//
//

#ifndef CIESAWorkflow_h
#define CIESAWorkflow_h

#include "HCommon.h"
#include "IGameWorkflow.h"

class CIESAWorkflow : public IGameWorkflow
{
private:
    
protected:
    
public:
    
    CIESAWorkflow(void);
    ~CIESAWorkflow(void);
    
    std::shared_ptr<IGameTransition> CreateIESAMainTransition(const std::string& _filename, void* _hwnd);
};

#endif
