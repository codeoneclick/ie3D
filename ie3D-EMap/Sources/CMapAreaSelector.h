//
//  CMapAreaSelector.h
//  ie3D-EMap
//
//  Created by Sergey Sergeev on 4/30/14.
//
//

#ifndef CMapAreaSelector_h
#define CMapAreaSelector_h

#include "IGameObject.h"

class CMapAreaSelector
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

};

#endif
