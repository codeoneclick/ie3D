//
//  CMEPlacementModel.h
//  ie3D-EMap
//
//  Created by sergey.sergeev on 7/10/14.
//
//

#ifndef CMEPlacementModel_h
#define CMEPlacementModel_h

#include "IGameObject.h"

class CMEPlacementMesh : public IGameObject
{
private:
    
    CSharedLandscape m_landscape;
    CSharedModel m_model;
    
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
    
    CMEPlacementMesh(CSharedResourceAccessorRef resourceAccessor,
                   ISharedRenderTechniqueAccessorRef renderTechniqueAccessor);
    ~CMEPlacementMesh(void);
    
    void setLandscape(CSharedLandscapeRef landscape);
    
    void setPosition(const glm::vec3& position);
    
    void setSize(f32 size);
};


#endif /* defined(__ie3D_EMap__CMEPlacementMesh__) */
