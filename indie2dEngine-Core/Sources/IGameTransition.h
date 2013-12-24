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
#include "CGUIFabricator.h"
#include "CGUIGraph.h"
#include "ITemplateLoadingHandler.h"
#include "IGameLoopHandler.h"

class IGraphicsContext;
class IInputContext;
class CResourceAccessor;
class CTemplateAccessor;
class IScene;

class IGameTransition :
public CSceneGraph,
public CSceneFabricator,
public CGUIFabricator,
public CGUIGraph,
public ITemplateLoadingHandler,
public IGameLoopHandler
{
private:
    
protected:
    
    friend class IGameWorkflow;
    
    std::string m_guid;
    std::shared_ptr<IScene> m_scene;
    bool m_isLoaded;
    
    virtual void _OnRegistered(void);
    virtual void _OnUnregistered(void);
    
    virtual void _OnActivate(void);
    virtual void _OnDeactivate(void);
    
    virtual void _OnTemplateLoaded(std::shared_ptr<I_RO_TemplateCommon> _template);
    virtual void _OnLoaded(void);
    
    virtual void _OnGameLoopUpdate(f32 _deltatime);
    
public:
    
    IGameTransition(const std::string& _filename, std::shared_ptr<IGraphicsContext> _graphicsContext, std::shared_ptr<IInputContext> _inputContext, std::shared_ptr<CResourceAccessor> _resourceAccessor, std::shared_ptr<CTemplateAccessor> _templateAccessor);
    virtual ~IGameTransition(void);

    inline std::string Get_Guid(void)
    {
        return m_guid;
    };
    
    ui32 Get_CurrentNumTriangles(void);
    ui32 Get_TotalNumTriangles(void);
};

#endif 
