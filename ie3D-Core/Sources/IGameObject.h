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
#include "IRenderTechniqueHandler.h"
#include "ISceneUpdateHandler.h"
#include "IOcclusionQueryHandler.h"

class IGameObject :
public std::enable_shared_from_this<IGameObject>,
public IConfigurationLoadingHandler,
public IResourceLoadingHandler,
public IRenderTechniqueHandler,
public ISceneUpdateHandler,
public IOcclusionQueryHandler
{
private:
    
protected:

    std::unordered_map<std::string, CSharedMaterial> m_materials;
    std::function<void(CSharedMaterialRef)> m_materialBindImposer;
    ISharedConfiguration m_configuration;
    
    CSharedMesh m_mesh;
    
    CSharedBoundingBox m_boundingBox;
    CSharedMaterial m_boundingBoxMaterial;
    std::function<void(CSharedMaterialRef)> m_boundingBoxMaterialBindImposer;
    
    CSharedCamera m_camera;
    CSharedFrustum m_cameraFrustum;
    CSharedGlobalLightSource m_globalLightSource;
    
    ISharedRenderTechniqueImporter m_renderTechniqueImporter;
    ISharedRenderTechniqueAccessor m_renderTechniqueAccessor;

    CSharedSceneUpdateMgr m_sceneUpdateMgr;
    
    CSharedResourceAccessor m_resourceAccessor;
    
    ui32 m_zOrder;
    
    bool m_occlusionQueryOngoing;
    bool m_occlusionQueryVisible;
    ui32 m_occlusionQueryHandler;
    
    ui8 m_status;
    
	bool m_isNeedToRender;
    bool m_isNeedToUpdate;
    bool m_isBatching;
    bool m_isNeedBoundingBox;
    
    virtual void onSceneUpdate(f32 deltatime);
    
    virtual void onResourceLoaded(ISharedResourceRef resource, bool success);
    virtual void onConfigurationLoaded(ISharedConfigurationRef configuration, bool success);
    
    virtual i32  zOrder(void);
    virtual bool checkOcclusion(void);
    virtual ui32 numTriangles(void);
    virtual void onDrawBoundingBox(void);
    virtual void onBind(const std::string& mode);
    virtual void onDraw(const std::string& mode);
    virtual void onUnbind(const std::string& mode);
    virtual void onBatch(const std::string& mode);
    
    virtual void onOcclusionQueryDraw(CSharedMaterialRef material);
    virtual void onOcclusionQueryUpdate(void);
    
    virtual void bindBaseShaderUniforms(CSharedMaterialRef material);
    virtual void bindCustomShaderUniforms(CSharedMaterialRef material);
    
    bool getBoundingBox(void);
    
    std::map<E_COMPONENT_CLASS, ISharedComponent> m_components;
    
public:
    
	IGameObject(CSharedResourceAccessorRef resourceAccessor,
                ISharedRenderTechniqueAccessorRef renderTechniqueAccessor);
    virtual ~IGameObject(void);
    
    void addComponent(ISharedComponentRef component);
    void removeComponent(ISharedComponentRef component);
    bool isComponentExist(E_COMPONENT_CLASS componentClass) const;
    ISharedComponent getComponent(E_COMPONENT_CLASS componentClass) const;
    
    virtual void setPosition(const glm::vec3& position);
    virtual void setRotation(const glm::vec3& rotation);
    virtual void setScale(const glm::vec3& scale);
    
    glm::vec3 getPosition(void) const;
    glm::vec3 getRotation(void) const;
    glm::vec3 getScale(void) const;
    
    glm::mat4 getTransformation(void) const;
    
    glm::vec3 getMaxBound(void) const;
    glm::vec3 getMinBound(void) const;
    
    virtual void setCamera(CSharedCameraRef camera);
    virtual void setCameraFrustum(CSharedFrustumRef frustum);
    virtual void setGlobalLightSource(CSharedGlobalLightSourceRef lightSource);
    
    CSharedVertexBuffer getVertexBuffer(void) const;
    CSharedIndexBuffer getIndexBuffer(void) const;
    
    CSharedMaterial getMaterial(const std::string& renderTechique) const;
    
    virtual void setCustomShaderUniform(const glm::mat4x4& matrix, const std::string& uniform, const std::string& renderTechnique = "");
    virtual void setCustomShaderUniform(const glm::mat3x3& matrix, const std::string& uniform, const std::string& renderTechnique = "");
    virtual void setCustomShaderUniform(const glm::vec4& vector, const std::string& uniform, const std::string& renderTechnique = "");
    virtual void setCustomShaderUniform(const glm::vec3& vector, const std::string& uniform, const std::string& renderTechnique = "");
    virtual void setCustomShaderUniform(const glm::vec2& vector, const std::string& uniform, const std::string& renderTechnique = "");
    virtual void setCustomShaderUniform(f32 value, const std::string& uniform, const std::string& renderTechnique = "");
    virtual void setCustomShaderUniform(i32 value, const std::string& uniform, const std::string& renderTechnique = "");
    
    virtual CSharedVertexBuffer getCollisionVertexBuffer(void) const;
    virtual CSharedIndexBuffer getCollisionIndexBuffer(void) const;
    
    virtual void setTexture(CSharedTextureRef texture,
                            E_SHADER_SAMPLER sampler,
                            const std::string& renderTechnique = "");
    
    virtual void setClippingPlane(const glm::vec4& clippingPlane,
                                  const std::string& renderTechnique);
    
    virtual void removeLoadingDependencies(void);
    
    virtual void setRenderTechniqueImporter(ISharedRenderTechniqueImporterRef techniqueImporter);
    virtual void setRenderTechniqueAccessor(ISharedRenderTechniqueAccessorRef techniqueAccessor);
    virtual void setSceneUpdateMgr(CSharedSceneUpdateMgrRef sceneUpdateMgr);
    
    virtual void enableRender(bool value);
    virtual void enableUpdate(bool value);
};

#endif 
