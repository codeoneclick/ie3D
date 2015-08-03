//
//  CMESceneStage.h
//  ie3D-EMap-OSX
//
//  Created by sergey.sergeev on 8/3/15.
//
//

#ifndef CMESceneStage_h
#define CMESceneStage_h

#include "IGameObject.h"

class CMESceneStage : public IGameObject
{
private:
    
    glm::vec3 m_size;
    
protected:
    
    void onSceneUpdate(f32 deltatime);
    
    void onResourceLoaded(ISharedResourceRef resource, bool success);
    void onConfigurationLoaded(ISharedConfigurationRef configuration, bool success);
    
public:
    
    CMESceneStage(CSharedResourceAccessorRef resourceAccessor,
                      ISharedRenderTechniqueAccessorRef renderTechniqueAccessor);
    ~CMESceneStage(void);
};

#endif
