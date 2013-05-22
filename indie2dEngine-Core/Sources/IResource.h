//
//  IResource.h
//  gEngine-Core
//
//  Created by Sergey Sergeev on 5/7/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef IResource_h
#define IResource_h

#include "HCommon.h"
#include "HEnums.h"

class IResource : public std::enable_shared_from_this<IResource>
{
private:
    
protected:
    
    std::string m_guid;
    E_RESOURCE_TYPE m_resourceType;
    bool m_isLinked;
    bool m_isLoaded;
    
public:
    
    IResource(E_RESOURCE_TYPE _resourceType, const std::string& _guid);
    virtual ~IResource(void);
    
    inline const std::string Get_Guid(void)
    {
        return m_guid;
    };
    
    inline const E_RESOURCE_TYPE Get_ResourceType(void)
    {
        return m_resourceType;
    };
    
    inline const bool IsLoaded(void)
    {
        return m_isLoaded;
    };
    
    inline const bool IsLinked(void)
    {
        return m_isLinked;
    };
};

#endif 


