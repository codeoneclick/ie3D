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
#include "HMEEnums.h"

class CMEPlacementModel : public IGameObject
{
private:
    
    CSharedLandscape m_landscape;
    CSharedModel m_model;
    
    std::array<CSharedMesh, E_PLACEMENT_MODEL_ARROW_MAX> m_arrows;
    
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
    
    CSharedMesh createArrowMesh(E_PLACEMENT_MODEL_ARROW arrow);
    
public:
    
    CMEPlacementModel(CSharedResourceAccessorRef resourceAccessor,
                     ISharedRenderTechniqueAccessorRef renderTechniqueAccessor);
    ~CMEPlacementModel(void);
    
    void setLandscape(CSharedLandscapeRef landscape);
};


#endif
