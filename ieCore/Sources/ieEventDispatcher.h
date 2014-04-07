//
//  ieEventDispatcher.h
//  ieCore
//
//  Created by sergey.sergeev on 4/7/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#ifndef __ieCore__ieEventDispatcher__
#define __ieCore__ieEventDispatcher__

#include "ieObject.h"

typedef std::function<void(const std::shared_ptr<ieEvent>&)> ieEventDispatcherFunction;
typedef std::shared_ptr<ieEventDispatcherFunction> ieEventDispatcherFunctionShared;
typedef const ieEventDispatcherFunctionShared& ieEventDispatcherFunctionSharedRef;

class ieEventDispatcher :
public ieObject
{
private:
    
protected:
    
    std::unordered_map<std::string, std::set<ieEventDispatcherFunctionShared>> m_functions;
    
public:
    
    ieEventDispatcher(void);
    virtual ~ieEventDispatcher(void);
    
    void addEventListener(const std::string& type,
                          ieEventDispatcherFunctionSharedRef function);
    void removeEventListener(const std::string& type,
                             ieEventDispatcherFunctionSharedRef function);
    bool hasEventListener(const std::string& type);
    void dispatchEvent(ieSharedEventRef event);
    
};


#endif
