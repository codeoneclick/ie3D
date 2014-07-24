//
//  CMETankComplex.h
//  ie3D-EMap
//
//  Created by sergey.sergeev on 7/23/14.
//
//

#ifndef CMETankComplex_h
#define CMETankComplex_h

#include "IGameObject.h"
#include "HMEEnums.h"

class CMETankComplex : public IGameObject
{
private:
    
    const std::string kTankPartBody;
    const std::string kTankPartTower;
    const std::string kTankPartLeftTrack;
    const std::string kTankPartRightTrack;
    std::array<CSharedModel, E_ME_TANK_PART_MAX> m_parts;
    
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
    
    CMETankComplex(CSharedResourceAccessorRef resourceAccessor,
                   ISharedRenderTechniqueAccessorRef renderTechniqueAccessor);
    ~CMETankComplex(void);
    
    void setPosition(const glm::vec3& position);
    void setRotation(const glm::vec3& rotation);
    void setScale(const glm::vec3& scale);
    
    void setCamera(CSharedCameraRef camera);
    
    void setRenderTechniqueImporter(ISharedRenderTechniqueImporterRef techniqueImporter);
    void setRenderTechniqueAccessor(ISharedRenderTechniqueAccessorRef techniqueAccessor);
    void setSceneUpdateMgr(CSharedSceneUpdateMgrRef sceneUpdateMgr);
    
    void enableRender(bool value);
    void enableUpdate(bool value);
};

#endif
