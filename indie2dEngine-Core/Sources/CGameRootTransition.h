//
//  CGameRootTransition.h
//  indi2dEngine
//
//  Created by Sergey Sergeev on 5/14/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CGameRootTransition_h
#define CGameRootTransition_h

#include "HCommon.h"
#include "CSceneGraph.h"
#include "CSceneFabricator.h"

class IGraphicsContext;
class CResourceAccessor;

class CGameRootTransition final : public CSceneGraph, public CSceneFabricator
{
private:
    
    std::shared_ptr<IGraphicsContext> m_glContext;
    
protected:
    
public:
    
    CGameRootTransition(const void* _glWindow);
    ~CGameRootTransition(void);
};


#endif
