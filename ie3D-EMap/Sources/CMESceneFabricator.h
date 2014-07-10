//
//  CMESceneFabricator.h
//  ie3D-EMap
//
//  Created by sergey.sergeev on 6/9/14.
//
//

#ifndef CMESceneFabricator_h
#define CMESceneFabricator_h

#include "CSceneFabricator.h"
#include "HMEDeclaration.h"

class CMESceneFabricator : public CSceneFabricator
{
private:
    
protected:
    
public:
    
    CMESceneFabricator(CSharedConfigurationAccessorRef configurationAccessor,
                       CSharedResourceAccessorRef resourceAccessor,
                       ISharedRenderTechniqueAccessorRef renderTechniqueAccessor);
    
    virtual ~CMESceneFabricator(void);
    
    CSharedMELandscapeBrush createLandscapeBrush(const std::string& filename);
    void deleteLandscapeBrush(CSharedMELandscapeBrushRef editableBrush);
};


#endif
