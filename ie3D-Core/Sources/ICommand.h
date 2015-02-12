//
//  ICommand.h
//  ie3D-Core
//
//  Created by sergey.sergeev on 2/12/15.
//  Copyright (c) 2015 Sergey Sergeev. All rights reserved.
//

#ifndef ICommand_h
#define ICommand_h

#include "HCommon.h"

class ICommand
{
private:
    
protected:
    
    ICommand(void) = default;
    
public:
    
    virtual ~ICommand(void) = default;
};

template<class FUNCTION>
class CCommand : public ICommand
{
    
private:
    
    FUNCTION m_function;
    
protected:
    
public:
    
    CCommand(FUNCTION function)
    {
        m_function = function;
    };
    
    ~CCommand(void) = default;
    
    template<typename... ARGS>
    void execute(ARGS... args)
    {
        assert(m_function != nullptr);
        m_function(args...);
    };
};

#endif
