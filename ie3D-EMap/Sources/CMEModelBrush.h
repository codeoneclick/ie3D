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
    
    std::array<CESharedCustomModel, E_MODEL_BRUSH_ARROW_MAX> m_arrows;
    std::array<CESharedCustomModel, E_MODEL_BRUSH_PLANE_MAX> m_planes;
    
protected:
    
    void onSceneUpdate(f32 deltatime);
    
    void onResourceLoaded(ISharedResourceRef resource, bool success);
    void onConfigurationLoaded(ISharedConfigurationRef configuration, bool success);
    
    bool isInCameraFrustum(CSharedFrustumRef cameraFrustum);
    void onDraw(CSharedMaterialRef material);
    
    CESharedCustomModel createArrowModel(E_MODEL_BRUSH_ARROW arrow,
                                         SAttributeVertex *mainVertices, ui32 verticesOffset,
                                         ui16 *mainIndices, ui32 indicesOffset);
    CESharedCustomModel createPlaneModel(E_MODEL_BRUSH_PLANE plane,
                                         SAttributeVertex *mainVertices, ui32 verticesOffset,
                                         ui16 *mainIndices, ui32 indicesOffset);
    
public:
    
    CMEModelBrush(CSharedResourceAccessorRef resourceAccessor,
                  ISharedRenderTechniqueAccessorRef renderTechniqueAccessor);
    ~CMEModelBrush(void);
    
    void setPosition(const glm::vec3& position);
    void setRotation(const glm::vec3& rotation);
    void setScale(const glm::vec3& scale);
    
    void setCamera(CSharedCameraRef camera);
    void setCameraFrustum(CSharedFrustumRef frustum);
    void setGlobalLightSource(CSharedGlobalLightSourceRef lightSource);
    
    void onAddedToScene(ISharedRenderTechniqueImporterRef techniqueImporter,
                        CSharedSceneUpdateMgrRef sceneUpdateMgr);
    void onRemovedFromScene(void);
    
    void setLandscape(CSharedLandscapeRef landscape);
};


#endif
