//
//  CGraphicsContext_iOS.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/17/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CGraphicsContext_iOS_h
#define CGraphicsContext_iOS_h

#include "IGraphicsContext.h"

#ifdef __APPLE__

#include <Foundation/Foundation.h>
#include <UIKit/UIKit.h>
#include <QuartzCore/QuartzCore.h>

class CGraphicsContext_iOS final : public IGraphicsContext
{
private:
    
protected:
    
     EAGLContext* m_context;
    
public:
    
    CGraphicsContext_iOS(const CAEAGLLayer* _iOSGLLayer);
    ~CGraphicsContext_iOS(void);
    
    void Output(void) const;
};

#endif 
           
#endif