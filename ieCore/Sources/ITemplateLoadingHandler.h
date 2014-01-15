//
//  ITemplateLoadingHandler.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/29/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef ITemplateLoadingHandler_h
#define ITemplateLoadingHandler_h

#include "HCommon.h"

class I_RO_TemplateCommon;
class CTemplateLoadingCommands final
{
private:
    
    friend class ITemplateLoadingHandler;
    friend class CModelTemplateLoader;
    friend class CParticleEmitterTemplateLoader;
    friend class CGameTransitionTemplateLoader;
    friend class COceanTemplateLoader;
    friend class CLandscapeTemplateLoader;
	friend class CGUITemplateLoader;
    
    typedef std::function<void(std::shared_ptr<I_RO_TemplateCommon> _template)> __TEMPLATE_LOADED_COMMAND;
    
    __TEMPLATE_LOADED_COMMAND m_templateLoadedCommand;
    
protected:
    
    CTemplateLoadingCommands(void);
    
    void _ConnectTemplateLoadedCommand(const __TEMPLATE_LOADED_COMMAND& _command);
    void _ExecuteTemplateLoadedCommand(const std::shared_ptr<I_RO_TemplateCommon>& _template);
    
public:
    
    ~CTemplateLoadingCommands(void);
};

class ITemplateLoadingHandler
{
public:
    
    typedef std::shared_ptr<std::function<void(const std::shared_ptr<I_RO_TemplateCommon>&)>> TEMPLATE_LOADING_HANDLER;
    
private:
    
    CTemplateLoadingCommands m_commands;
    
    friend class CModelTemplateLoader;
    friend class CParticleEmitterTemplateLoader;
    friend class CGameTransitionTemplateLoader;
    friend class COceanTemplateLoader;
    friend class CLandscapeTemplateLoader;
	friend class CGUITemplateLoader;
    
    void _ConnectCommands(void);
    inline CTemplateLoadingCommands _Get_Commands(void)
    {
        return m_commands;
    };
    
protected:
    
    ITemplateLoadingHandler(void);
    
    std::set<TEMPLATE_LOADING_HANDLER> m_templateLoadingHandlers;
    
    virtual void _OnTemplateLoaded(std::shared_ptr<I_RO_TemplateCommon> _template) = 0;
    
public:
    
    virtual ~ITemplateLoadingHandler(void);
    
    void RegisterTemplateLoadingHandler(const TEMPLATE_LOADING_HANDLER& _handler);
    void UnregisterTemplateLoadingHandler(const TEMPLATE_LOADING_HANDLER& _handler);
};

#endif
