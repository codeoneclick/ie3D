//
//  IResourceLoadingHandler.h
//  gEngine-Core
//
//  Created by Sergey Sergeev on 5/7/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef IResourceLoadingHandler_h
#define IResourceLoadingHandler_h

#include "HCommon.h"
#include "HEnums.h"

class IResource;
class CResourceLoadingCommands final
{
private:
    
    friend class IResource;
    friend class IResourceLoadingHandler;
    
    typedef std::function<void(std::shared_ptr<IResource> _resource, bool _success)> _RESOURCE_LOADED_COMMAND;
    
    _RESOURCE_LOADED_COMMAND m_resourceLoadedCommand;
    
protected:
    
    CResourceLoadingCommands(void);
    
    void _ConnectLoadedResourceCommand(const _RESOURCE_LOADED_COMMAND& _command);
    void _ExecuteLoadedResourceCommand(std::shared_ptr<IResource> _resource, bool _success);
    
public:
    
    ~CResourceLoadingCommands(void);
};

class IResourceLoadingHandler
{
public:
    
    typedef std::shared_ptr<std::function<void(const std::shared_ptr<IResource>&)>> RESOURCE_LOADING_HANDLER;
    
private:
    
    friend class IResource;
    
    CResourceLoadingCommands m_commands;
    
    void _ConnectCommands(void);
    inline CResourceLoadingCommands _Get_Commands(void)
    {
        return m_commands;
    };
    
protected:
    
    IResourceLoadingHandler(void);
    std::array<std::set<RESOURCE_LOADING_HANDLER>, E_RESOURCE_CLASS_MAX> m_resourceLoadingHandlers;
    
    virtual void _OnResourceLoaded(std::shared_ptr<IResource> _resource, bool _success) = 0;
    
public:
    
    virtual ~IResourceLoadingHandler(void);
    
    void RegisterResourceLoadingHandler(const RESOURCE_LOADING_HANDLER& _handler, E_RESOURCE_CLASS _class);
    void UnregisterResourceLoadingHandler(const RESOURCE_LOADING_HANDLER& _handler, E_RESOURCE_CLASS _class);
};

#endif 
