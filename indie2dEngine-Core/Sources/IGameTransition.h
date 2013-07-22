//
//  IGameTransition.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 6/6/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef IGameTransition_h
#define IGameTransition_h

#include "HCommon.h"
#include "CSceneGraph.h"
#include "CSceneFabricator.h"
#include "ITemplateLoadingHandler.h"
#include "IGameLoopHandler.h"

class IGraphicsContext;
class IInputContext;
class CResourceAccessor;
class CTemplateAccessor;

class IGameTransition :
public CSceneGraph,
public CSceneFabricator,
public ITemplateLoadingHandler,
public IGameLoopHandler
{
private:
    
protected:
    
    friend class IGameWorkflow;
    
    std::string m_guid;
    bool m_isLoaded;
    
    virtual void _OnRegistered(void);
    virtual void _OnUnregistered(void);
    
    virtual void _OnActivate(void);
    virtual void _OnDeactivate(void);
    
    virtual void _OnTemplateLoaded(std::shared_ptr<ITemplate> _template);
    virtual void _OnLoaded(void);
    
    virtual void _OnGameLoopUpdate(f32 _deltatime);
    
public:
    
    IGameTransition(const std::string& _filename, std::shared_ptr<IGraphicsContext> _graphicsContext, std::shared_ptr<IInputContext> _inputContext, std::shared_ptr<CResourceAccessor> _resourceAccessor, std::shared_ptr<CTemplateAccessor> _templateAccessor);
    virtual ~IGameTransition(void);

    inline std::string Get_Guid(void)
    {
        return m_guid;
    };
};

#endif 
