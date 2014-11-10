//
//  IRenderTechniqueHandler.h
//  ie3D-Core
//
//  Created by sergey.sergeev on 7/2/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#ifndef IRenderTechniqueHandler_h
#define IRenderTechniqueHandler_h

#include "HCommon.h"
#include "HDeclaration.h"

class IRenderTechniqueHandler
{
private:
    
protected:
    
    IRenderTechniqueHandler(void);
    
public:

    virtual ~IRenderTechniqueHandler(void);
    
    virtual i32  zOrder(void) = 0;
    virtual bool checkOcclusion(void) = 0;
    virtual ui32 numTriangles(void) = 0;
    virtual void onDrawBoundingBox(void) = 0;
    virtual void onBind(const std::string& techniqueName) = 0;
    virtual void onDraw(const std::string& techniqueName) = 0;
    virtual void onUnbind(const std::string& techniqueName) = 0;
    virtual void onBatch(const std::string& techniqueName) = 0;
};

#endif
