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
    
    typedef std::function<void(ISharedResourceRef)> RESOURCE_LOADING_COMMAND;
    
private:
    
protected:
    
    IResourceLoadingHandler(void);
    std::vector<RESOURCE_LOADING_COMMAND> m_commands;
    std::set<ISharedResource> m_resources;
    
public:
    
    virtual ~IResourceLoadingHandler(void);
    
    virtual void onResourceLoaded(ISharedResourceRef resource, bool success);
    void addResourceLoadingCommand(const RESOURCE_LOADING_COMMAND& command);
    void removeResourceLoadingCommand(const RESOURCE_LOADING_COMMAND& command);
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
    
    virtual void onResourceDataSerializationFinished(ISharedResourceDataRef resourceData) = 0;
    
    virtual void onResourceDataCommitFinished(ISharedResourceDataRef resourceData) = 0;
    
    void onResourceLoaded(void);
    
public:
    
    virtual ~IResource(void);
    
    const std::string& getGUID(void) const;
    E_RESOURCE_CLASS getResourceClass(void) const;
    
    virtual bool isLoaded(void) const;
    virtual bool isCommited(void) const;
    
    void addLoadingHandler(ISharedResourceLoadingHandlerRef handler);
    void removeLoadingHandler(ISharedResourceLoadingHandlerRef handler);
};

#endif 


