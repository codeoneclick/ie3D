//
//  ieEvent.h
//  ieCore
//
//  Created by sergey.sergeev on 4/7/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#ifndef __ieCore__ieEvent__
#define __ieCore__ieEvent__

#include "ieObject.h"

static const std::string kEVENT_ON_UPDATE = "EVENT_ON_UPDATE";
static const std::string kEVENT_ON_DRAW = "EVENT_ON_DRAW";

static const std::string kEVENT_ON_ADDED = "EVENT_ON_ADDED";
static const std::string kEVENT_ON_REMOVED = "EVENT_ON_REMOVED";

class ieEvent : public ieObject
{
private:
    
protected:
    
    std::string m_type;
    ieSharedEventDispatcher m_target;
    
public:
    
    ieEvent(const std::string& type,
            ieSharedEventDispatcherRef target);
    virtual ~ieEvent(void);
    
    std::string getType(void) const;
    ieSharedEventDispatcher getTarget(void) const;
};

#endif
