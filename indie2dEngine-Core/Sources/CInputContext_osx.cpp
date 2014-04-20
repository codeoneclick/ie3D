//
//  CInputMgr.cpp
//  iGaia-CartoonPanzers
//
//  Created by Sergey Sergeev on 3/27/13.
//
//

#include "IInputContext.h"

#if defined(__OSX__)

class IInputContext_osx : public IInputContext
{
private:
    
protected:
    
public:
    
    IInputContext_osx(ISharedOGLWindowRef window);
    ~IInputContext_osx(void);
};

std::shared_ptr<IInputContext> createInputContext_osx(ISharedOGLWindowRef window)
{
    return std::make_shared<IInputContext_osx>(window);
};

IInputContext_osx::IInputContext_osx(ISharedOGLWindowRef window)
{

}

IInputContext_osx::~IInputContext_osx(void)
{
    
}

#endif