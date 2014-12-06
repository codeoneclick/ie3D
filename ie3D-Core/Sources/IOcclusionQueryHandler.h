//
//  IOcclusionQueryHandler.h
//  ie3D-Core
//
//  Created by sergey.sergeev on 11/24/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#ifndef IOcclusionQueryHandler_h
#define IOcclusionQueryHandler_h

#include "HCommon.h"
#include "HDeclaration.h"
#include "HEnums.h"

class IOcclusionQueryHandler
{
private:
    
protected:
    
    IOcclusionQueryHandler(void);
    
public:
    
    virtual ~IOcclusionQueryHandler(void);
    
    virtual void onOcclusionQueryDraw(CSharedMaterialRef material) = 0;
    virtual void onOcclusionQueryUpdate(void) = 0;
};

#endif
