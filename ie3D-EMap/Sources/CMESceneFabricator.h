//
//  CMESceneFabricator.h
//  ie3D-EMap
//
//  Created by sergey.sergeev on 6/9/14.
//
//

#ifndef CMESceneFabricator_h
#define CMESceneFabricator_h

#include "CESceneFabricator.h"
#include "HMEDeclaration.h"

class CMESceneFabricator : public CESceneFabricator
{
private:
    
protected:
    
public:
    
    CMESceneFabricator(CSharedConfigurationAccessorRef configurationAccessor,
                       CSharedResourceAccessorRef resourceAccessor,
                       ISharedRenderTechniqueAccessorRef renderTechniqueAccessor);
    
    virtual ~CMESceneFabricator(void);
    
    CMESharedLandscapeBrush createLandscapeBrush(const std::string& filename);
    CMESharedModelBrush createModelBrush(const std::string& filename);
    CMESharedSceneStage createSceneStage(const std::string& filename);
};


#endif
