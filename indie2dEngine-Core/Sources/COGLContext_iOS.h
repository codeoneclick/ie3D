//
//  COGLContext_iOS.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/17/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef COGLContext_iOS_h
#define COGLContext_iOS_h

#include "IOGLContext.h"

#ifdef __APPLE__

#include <Foundation/Foundation.h>
#include <UIKit/UIKit.h>
#include <QuartzCore/QuartzCore.h>

class COGLContext_iOS final : public IOGLContext
{
private:
    
protected:
    
     EAGLContext* m_context;
    
public:
    
    COGLContext_iOS(const CAEAGLLayer* _iOSGLLayer);
    ~COGLContext_iOS(void);
    
    void Output(void) const;
};

#endif 
           
#endif