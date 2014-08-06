//
//  CEComplexModel.h
//  ie3D-Engine
//
//  Created by sergey.sergeev on 8/6/14.
//  Copyright (c) 2014 sergey.sergeev. All rights reserved.
//

#ifndef CEComplexModel_h
#define CEComplexModel_h

#include "IGameObject.h"

class CEComplexModel : public IGameObject
{
private:
    
    std::unordered_map<std::string, CSharedModel> m_parts;
    
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
    
    CEComplexModel(CSharedResourceAccessorRef resourceAccessor,
                   ISharedRenderTechniqueAccessorRef renderTechniqueAccessor);
    ~CEComplexModel(void);
    
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
