//
//  CMEModelBrush.h
//  ie3D-EMap
//
//  Created by sergey.sergeev on 7/10/14.
//
//

#ifndef CMEModelBrush_h
#define CMEModelBrush_h

#include "IGameObject.h"
#include "HMEEnums.h"
#include "HMEDeclaration.h"

class CMEModelBrush : public IGameObject
{
private:
    
    CSharedLandscape m_landscape;
    CSharedModel m_model;
    
    std::array<CESharedCustomModel, E_PLACEMENT_MODEL_ARROW_MAX> m_arrows;
    
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
    
    CESharedCustomModel createArrowModel(E_PLACEMENT_MODEL_ARROW arrow);
    
public:
    
    CMEModelBrush(CSharedResourceAccessorRef resourceAccessor,
                  ISharedRenderTechniqueAccessorRef renderTechniqueAccessor);
    ~CMEModelBrush(void);
    
    void setPosition(const glm::vec3& position);
    void setRotation(const glm::vec3& rotation);
    void setScale(const glm::vec3& scale);
    
    void setCamera(CSharedCameraRef camera);
    void setGlobalLightSource(CSharedGlobalLightSourceRef lightSource);
    
    void setRenderTechniqueImporter(ISharedRenderTechniqueImporterRef techniqueImporter);
    void setRenderTechniqueAccessor(ISharedRenderTechniqueAccessorRef techniqueAccessor);
    void setSceneUpdateMgr(CSharedSceneUpdateMgrRef sceneUpdateMgr);
    
    void enableRender(bool value);
    void enableUpdate(bool value);
    
    void setLandscape(CSharedLandscapeRef landscape);
};


#endif