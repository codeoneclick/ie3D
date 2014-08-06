//
//  CESceneFabricator.h
//  ie3D-Engine
//
//  Created by sergey.sergeev on 8/6/14.
//  Copyright (c) 2014 sergey.sergeev. All rights reserved.
//

#ifndef CESceneFabricator_h
#define CESceneFabricator_h

#include "CSceneFabricator.h"
#include "HEDeclaration.h"

class CESceneFabricator : public CSceneFabricator
{
private:
    
protected:
    
public:
    
    CESceneFabricator(CSharedConfigurationAccessorRef configurationAccessor,
                      CSharedResourceAccessorRef resourceAccessor,
                      ISharedRenderTechniqueAccessorRef renderTechniqueAccessor);
    
    virtual ~CESceneFabricator(void);
    
    CESharedComplexModel createComplexModel(const std::string& filename);
};


#endif
