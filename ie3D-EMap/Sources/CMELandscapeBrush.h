//
//  CMELandscapeBrush.h
//  ie3D-EMap
//
//  Created by Sergey Sergeev on 4/30/14.
//
//

#ifndef CMELandscapeBrush_h
#define CMELandscapeBrush_h

#include "IGameObject.h"

class CMELandscapeBrush : public IGameObject
{
private:
    
    f32 m_size;
    
    CSharedLandscape m_landscape;
    
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
    
    void createMesh(f32 radius);
    
public:
    
    CMELandscapeBrush(CSharedResourceAccessorRef resourceAccessor,
                   ISharedRenderTechniqueAccessorRef renderTechniqueAccessor);
    ~CMELandscapeBrush(void);
    
    void setLandscape(CSharedLandscapeRef landscape);
    
    void setPosition(const glm::vec3& position);
    
    void setSize(f32 size);
};

#endif
