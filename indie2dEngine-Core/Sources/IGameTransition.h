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

class IGraphicsContext;
class CResourceAccessor;
class CTemplateAccessor;

class IGameTransition :
public CSceneGraph,
public CSceneFabricator,
public ITemplateLoadingHandler,
public std::enable_shared_from_this<ITemplateLoadingHandler>
{
private:
    
protected:
    
    std::string m_guid;
    
    virtual void _OnRegistered(void);
    virtual void _OnUnregistered(void);
    
    virtual void _OnTemplateLoaded(std::shared_ptr<ITemplate> _template);
    
public:
    
    IGameTransition(const std::string& _filename, std::shared_ptr<IGraphicsContext> _graphicsContext, std::shared_ptr<CResourceAccessor> _resourceAccessor, std::shared_ptr<CTemplateAccessor> _templateAccessor);
    virtual ~IGameTransition(void);

    inline std::string Get_Guid(void)
    {
        return m_guid;
    };
};

#endif 
