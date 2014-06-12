//
//  IFabricator.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/28/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef IFabricator_h
#define IFabricator_h

#include "HCommon.h"
#include "HDeclaration.h"

class IFabricator
{
private:

protected:
    
    CSharedConfigurationAccessor m_configurationAccessor;
    CSharedResourceAccessor m_resourceAccessor;
    ISharedScreenSpaceRenderAccessor m_screenSpaceTextureAccessor;
	
public:
    
    IFabricator(CSharedConfigurationAccessorRef configurationAccessor,
                CSharedResourceAccessorRef resourceAccessor,
                ISharedScreenSpaceRenderAccessor screenSpaceTextureAccessor);
    
    virtual ~IFabricator(void);
};

#endif
