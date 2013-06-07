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

class IOGLContext;
class CResourceAccessor;
class CTemplateAccessor;
class IGameTransition final :public CSceneGraph,public CSceneFabricator
{
private:
    
protected:
    
    std::string m_guid;
    
    virtual void _OnRegistered(void);
    virtual void _OnUnregistered(void);
    
public:
    
    IGameTransition(const std::string& _guid, std::shared_ptr<IOGLContext> _graphicsContext, std::shared_ptr<CResourceAccessor> _resourceAccessor, std::shared_ptr<CTemplateAccessor> _templateAccessor);
    virtual ~IGameTransition(void);

    inline std::string Get_Guid(void)
    {
        return m_guid;
    };
};

#endif 
