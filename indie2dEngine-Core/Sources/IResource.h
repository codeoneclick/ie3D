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

class IResourceLoadingHandler;
class IResource : public std::enable_shared_from_this<IResource>
{
private:
    
protected:
    
    std::string m_guid;
    E_RESOURCE_CLASS m_class;
    ui8 m_status;
    
    std::set<std::shared_ptr<IResourceLoadingHandler>> m_handlers;
    
public:
    
    IResource(E_RESOURCE_CLASS _class, const std::string& _guid);
    virtual ~IResource(void);
    
    inline const std::string Get_Guid(void)
    {
        return m_guid;
    };
    
    inline const E_RESOURCE_CLASS Get_Class(void)
    {
        return m_class;
    };
    
    inline const bool IsLoaded(void)
    {
        return m_status & E_RESOURCE_STATUS_LOADED;
    };
    
    inline const bool IsLinked(void)
    {
        return m_status & E_RESOURCE_STATUS_COMMITED;
    };
    
    void RegisterResourceLoadingHandler(std::shared_ptr<IResourceLoadingHandler> _handler);
    void ExecuteResourceLoadingHandlers(void);
};

#endif 


