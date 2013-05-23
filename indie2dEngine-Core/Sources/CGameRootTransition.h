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

class IOGLContext;
class CResourceFabricator;

class CGameRootTransition final
{
private:
    
    std::shared_ptr<IOGLContext> m_glContext;
    std::shared_ptr<CResourceFabricator> m_resourceFabricator;
    
protected:
    
public:
    
    CGameRootTransition(const void* _glWindow);
    ~CGameRootTransition(void);
};


#endif
