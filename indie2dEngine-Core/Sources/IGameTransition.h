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
class IGameTransition final : public CSceneGraph, public CSceneFabricator
{
private:
    
protected:
    
    std::string m_guid;
    
public:
    IGameTransition(const std::string& _guid, const void* _hwnd);
    ~IGameTransition(void);

    
    inline std::string Get_Guid(void)
    {
        return m_guid;
    };
};

#endif 
