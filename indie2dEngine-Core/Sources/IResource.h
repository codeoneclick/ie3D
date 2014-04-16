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
#include "HDeclaration.h"

class IResourceLoadingHandler
{
public:
    
    typedef std::shared_ptr<std::function<void(ISharedResourceRef)>> RESOURCE_LOADING_HANDLER_FUNCTION;
    
private:
    
    friend class IResource;
    
protected:
    
    IResourceLoadingHandler(void);
    std::array<std::set<RESOURCE_LOADING_HANDLER_FUNCTION>, E_RESOURCE_CLASS_MAX> m_resourceLoadingHandlers;
    
    virtual void onResourceLoaded(ISharedResourceRef resource, bool success);
    
public:
    
    virtual ~IResourceLoadingHandler(void);
    
    void registerResourceLoadingHandler(const RESOURCE_LOADING_HANDLER_FUNCTION& handler, E_RESOURCE_CLASS resourceClass);
    void unregisterResourceLoadingHandler(const RESOURCE_LOADING_HANDLER_FUNCTION& handler, E_RESOURCE_CLASS resourceClass);
};

class IResourceData : public std::enable_shared_from_this<IResourceData>
{
private:
    
protected:
    
    E_RESOURCE_DATA_CLASS m_resourceDataClass;
    IResourceData(E_RESOURCE_DATA_CLASS resourceDataClass);
    
public:
    
    ~IResourceData(void);
    
    E_RESOURCE_DATA_CLASS getResourceDataClass(void) const;
};

class IResource : public std::enable_shared_from_this<IResource>
{
private:
    
    friend class IResourceCommiter;
    friend class IResourceSerializer;
    friend class IResourceLoadingOperation;
    
protected:
    
    std::string m_guid;
    E_RESOURCE_CLASS m_resourceClass;
    ui8 m_status;
    
    std::set<ISharedResourceLoadingHandler> m_handlers;
    
    IResource(E_RESOURCE_CLASS resourceClass,
              const std::string& guid);
    
    virtual void onResourceDataSerialized(ISharedResourceDataRef resourceData,
                                          E_RESOURCE_DATA_STATUS status) = 0;
    
    virtual void onResourceDataCommited(ISharedResourceDataRef resourceData,
                                        E_RESOURCE_DATA_STATUS status) = 0;
    
    void onResourceLoaded(void);
    
public:
    
    virtual ~IResource(void);
    
    const std::string& getGuid(void) const;
    E_RESOURCE_CLASS getResourceClass(void) const;
    
    virtual bool isLoaded(void) const;
    virtual bool isCommited(void) const;
    
    void registerLoadingHandler(ISharedResourceLoadingHandlerRef handler);
    void unregisterLoadingHandler(ISharedResourceLoadingHandlerRef handler);
};

#endif 


