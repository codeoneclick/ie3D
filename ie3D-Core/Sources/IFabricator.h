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
    ISharedRenderTechniqueAccessor m_renderTechniqueAccessor;
	
public:
    
    IFabricator(CSharedConfigurationAccessorRef configurationAccessor,
                CSharedResourceAccessorRef resourceAccessor,
                ISharedRenderTechniqueAccessorRef renderTechniqueAccessor);
    
    virtual ~IFabricator(void);
};

#endif
