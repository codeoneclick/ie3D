//
//  CSkyBox.h
//  ie3D-Core
//
//  Created by Sergey Sergeev on 5/6/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#ifndef CSkyBox_h
#define CSkyBox_h

#include "IGameObject.h"

class CSkyBox: public IGameObject
{
private:
    
protected:
    
    void onSceneUpdate(f32 deltatime);
    
    void onResourceLoaded(ISharedResourceRef resource, bool success);
    void onConfigurationLoaded(ISharedConfigurationRef configuration, bool success);
    
    bool isInCameraFrustum(CSharedFrustumRef cameraFrustum);
    void onDraw(CSharedMaterialRef material);
    void onBind(CSharedMaterialRef material);
    
public:
    
    CSkyBox(CSharedResourceAccessorRef resourceAccessor,
            ISharedRenderTechniqueAccessorRef renderTechniqueAccessor);
    ~CSkyBox(void);
};

#endif
