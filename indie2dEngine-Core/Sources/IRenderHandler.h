//
//  IRenderHandler.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/17/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef IRenderHandler_h
#define IRenderHandler_h

#include "HCommon.h"

class IRenderHandler
{
private:
    
    friend class CRenderMgr;
    friend class CRenderOperationWorldSpace;
    
protected:
    
    IRenderHandler(void);
    
    virtual i32  zOrder(void) = 0;
    virtual bool checkOcclusion(void) = 0;
    virtual ui32 numTriangles(void) = 0;
    virtual void onBind(const std::string& mode) = 0;
    virtual void onDraw(const std::string& mode) = 0;
    virtual void onUnbind(const std::string& mode) = 0;
    virtual void onBatch(const std::string& mode) = 0;
    
public:
    
    virtual ~IRenderHandler(void);
};


#endif 
