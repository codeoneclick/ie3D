//
//  IRenderTechniqueOperationTextureHandler.h
//  ie3D-Core
//
//  Created by sergey.sergeev on 11/4/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#ifndef IRenderTechniqueOperationTextureHandler_h
#define IRenderTechniqueOperationTextureHandler_h

#include "HCommon.h"
#include "HDeclaration.h"

class IRenderTechniqueOperationTextureHandler
{
private:
    
protected:
    
public:
    
    IRenderTechniqueOperationTextureHandler(void);
    virtual ~IRenderTechniqueOperationTextureHandler(void);
    
    virtual void onTextureRendered(const std::string& techniqueName, const ui8 *rawdata, ui32 width, ui32 height) = 0;
};

#endif
