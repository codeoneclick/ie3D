//
//  CLandscapeSeams.h
//  ie3D-Core
//
//  Created by sergey.sergeev on 10/17/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#ifndef CLandscapeSeams_h
#define CLandscapeSeams_h

#include "IGameObject.h"

class CLandscapeSeams : public IGameObject
{
private:
    

protected:

    void onSceneUpdate(f32 deltatime);
    
    void onResourceLoaded(ISharedResourceRef resource, bool success);
    void onConfigurationLoaded(ISharedConfigurationRef configuration, bool success);
    
    i32  zOrder(void);
    bool checkOcclusion(void);
    ui32 numTriangles(void);
    void onBind(const std::string& mode);
    void onDraw(const std::string& mode);
    void onUnbind(const std::string& mode);
    void onBatch(const std::string& mode);
    
public:
    
    CLandscapeSeams(CSharedResourceAccessorRef resourceAccessor,
                    ISharedRenderTechniqueAccessorRef renderTechniqueAccessor);
    ~CLandscapeSeams(void);
};

#endif
