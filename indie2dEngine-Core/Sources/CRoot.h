//
//  CRoot.h
//  indi2dEngine
//
//  Created by Sergey Sergeev on 5/14/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CRoot_h
#define CRoot_h

#include "IOGLContext.h"

class CRoot final
{
private:
    
    IOGLContext* m_glContext;
    
protected:
    
public:
    
    CRoot(void* _glWindow);
    ~CRoot(void);
};


#endif
