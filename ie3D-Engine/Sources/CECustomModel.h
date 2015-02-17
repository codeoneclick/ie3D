//
//  CCustomModel.h
//  ie3D-Engine
//
//  Created by sergey.sergeev on 10/27/14.
//  Copyright (c) 2014 sergey.sergeev. All rights reserved.
//

#ifndef CECustomModel_h
#define CECustomModel_h

#include "IGameObject.h"

class CECustomModel : public IGameObject
{
private:
    
    
protected:
    
    void onSceneUpdate(f32 deltatime);
    
    void onResourceLoaded(ISharedResourceRef resource, bool success);
    void onConfigurationLoaded(ISharedConfigurationRef configuration, bool success);
    
    void onDraw(CSharedMaterialRef material);
    
public:
    
    CECustomModel(CSharedResourceAccessorRef resourceAccessor,
                  ISharedRenderTechniqueAccessorRef renderTechniqueAccessor);
    ~CECustomModel(void);
    
    void setMesh(CSharedMeshRef mesh);
};

#endif
