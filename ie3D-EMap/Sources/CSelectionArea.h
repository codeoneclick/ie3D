//
//  CSelectionArea.h
//  ie3D-EMap
//
//  Created by Sergey Sergeev on 4/30/14.
//
//

#ifndef CSelectionArea_h
#define CSelectionArea_h

#include "IGameObject.h"

class CSelectionArea : public IGameObject
{
private:
    
    ui32 m_sizeX;
    ui32 m_sizeZ;
    
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
    
    CSharedLandscape m_landscape;
    
public:
    
    CSelectionArea(CSharedResourceAccessorRef resourceAccessor,
                   ISharedScreenSpaceTextureAccessorRef screenSpaceTextureAccessor);
    ~CSelectionArea(void);
    
    void setLandscape(CSharedLandscapeRef landscape);
    
    void setPosition(const glm::vec3& position);
};

#endif
