//
//  IGameObject.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/24/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef IGameObject_h
#define IGameObject_h

#include "HCommon.h"
#include "HEnums.h"
#include "HDeclaration.h"
#include "IResource.h"
#include "IConfiguration.h"
#include "IRenderHandler.h"
#include "ISceneUpdateHandler.h"

class IGameObject :
public std::enable_shared_from_this<IGameObject>,
public IConfigurationLoadingHandler,
public IResourceLoadingHandler,
public IRenderHandler,
public ISceneUpdateHandler
{
private:
    
protected:
    
    glm::mat4x4 m_matrixScale;
    glm::mat4x4 m_matrixRotation;
    glm::mat4x4 m_matrixTranslation;
    glm::mat4x4 m_matrixWorld;
    
    glm::vec3 m_position;
    glm::vec3 m_rotation;
    glm::vec3 m_scale;
    
    std::unordered_map<std::string, CSharedMaterial> m_materials;
    std::function<void(CSharedMaterialRef)> m_materialBindImposer;
    ISharedConfiguration m_configuration;
    
    CSharedMesh m_mesh;
    CSharedBoundBox m_boundBox;
    
    CSharedCamera m_camera;
    std::array<CSharedLightSource, E_LIGHT_SOURCE_MAX> m_lightSources;
    
    CSharedRenderMgr m_renderMgr;
    CSharedSceneUpdateMgr m_sceneUpdateMgr;
    CSharedResourceAccessor m_resourceAccessor;
	ISharedScreenSpaceTextureAccessor m_screenSpaceTextureAccessor;
    ui32 m_zOrder;
    
    ui8 m_status;
    
	bool m_isNeedToRender;
    bool m_isNeedToUpdate;
    bool m_isBatching;
    
    virtual void onSceneUpdate(f32 deltatime);
    
    virtual void onResourceLoaded(ISharedResourceRef resource, bool success);
    virtual void onConfigurationLoaded(ISharedConfigurationRef configuration, bool success);
    
    virtual i32  zOrder(void);
    virtual bool checkOcclusion(void);
    virtual ui32 numTriangles(void);
    virtual void onBind(const std::string& mode);
    virtual void onDraw(const std::string& mode);
    virtual void onUnbind(const std::string& mode);
    virtual void onBatch(const std::string& mode);
    
public:
    
	IGameObject(CSharedResourceAccessorRef resourceAccessor,
                ISharedScreenSpaceTextureAccessorRef screenSpaceTextureAccessor);
    virtual ~IGameObject(void);
    
    void setPosition(const glm::vec3& position);
    glm::vec3 getPosition(void) const;
    
    void setRotation(const glm::vec3& rotation);
    glm::vec3 getRotation(void) const;
    
    void setScale(const glm::vec3& scale);
    glm::vec3 getScale(void) const;
    
    glm::mat4x4 getWorldMatrix(void) const;
    
    glm::vec3 getMaxBound(void) const;
    glm::vec3 getMinBound(void) const;
    
    virtual void setCamera(CSharedCameraRef camera);
    
    virtual void setLightSource(CSharedLightSourceRef lightSource,
                                E_LIGHT_SOURCE index);
    
    CSharedVertexBuffer getVertexBuffer(void) const;
    CSharedIndexBuffer getIndexBuffer(void) const;
    
    virtual CSharedVertexBuffer getCollisionVertexBuffer(void) const;
    virtual CSharedIndexBuffer getCollisionIndexBuffer(void) const;
    
    void setTexture(CSharedTextureRef texture,
                    E_SHADER_SAMPLER sampler,
                    const std::string& mode);
    
    void setClippingPlane(const glm::vec4& clippingPlane,
                          const std::string& mode);
    
    virtual void setRenderMgr(CSharedRenderMgrRef renderMgr);
    virtual void setSceneUpdateMgr(CSharedSceneUpdateMgrRef sceneUpdateMgr);
    
    virtual void listenRenderMgr(bool value);
    virtual void listenSceneUpdateMgr(bool value);
};

#endif 
