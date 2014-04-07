//
//  ieResource.h
//  ieCore
//
//  Created by sergey.sergeev on 4/7/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#ifndef __ieCore__ieResource__
#define __ieCore__ieResource__

#include "ieEventDispatcher.h"
#include "HEnums.h"

class ieResource : public ieEventDispatcher
{
private:
    
protected:
    
    std::string m_guid;
    E_RESOURCE_CLASS m_resourceClass;
    std::set<ieSharedObject> m_owners;
    
    ieResource(E_RESOURCE_CLASS resourceClass, const std::string& guid);
    
public:
    
    virtual ~ieResource(void);
    
    void addOwner(ieSharedObjectRef owner);
    void removeOwner(ieSharedObjectRef owner);
    
    const ui32 getOwnersCount(void) const;
    
    std::string getGUID(void) const;
};

#endif
