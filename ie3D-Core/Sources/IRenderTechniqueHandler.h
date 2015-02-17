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
    
    virtual bool isInCameraFrustum(CSharedFrustumRef cameraFrustum) = 0;
    virtual void onDraw(CSharedMaterialRef material) = 0;
    virtual CSharedComponentRendering getComponentRendering(void) const = 0;
};

#endif
