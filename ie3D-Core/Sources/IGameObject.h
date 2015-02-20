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
#include "CComponentRendering.h"

class IGameObject :
public std::enable_shared_from_this<IGameObject>,
public IConfigurationLoadingHandler,
public IResourceLoadingHandler,
public ISceneUpdateHandler,
public IRenderTechniqueHandler
{
private:
    
protected:

    std::function<void(CSharedMaterialRef)> m_materialBindImposer;
    
    CSharedMesh m_mesh;
    
    CSharedCamera m_camera;
    CSharedFrustum m_cameraFrustum;
    CSharedGlobalLightSource m_globalLightSource;
    
    ISharedRenderTechniqueImporter m_renderTechniqueImporter;
    ISharedRenderTechniqueAccessor m_renderTechniqueAccessor;

    CSharedSceneUpdateMgr m_sceneUpdateMgr;
    
    CSharedResourceAccessor m_resourceAccessor;
    
    ui8 m_status;
    
    virtual void onSceneUpdate(f32 deltatime);
    
    virtual void onResourceLoaded(ISharedResourceRef resource, bool success);
    virtual void onConfigurationLoaded(ISharedConfigurationRef configuration, bool success);
    
    virtual bool isInCameraFrustum(CSharedFrustumRef cameraFrustum);
    virtual void onDraw(CSharedMaterialRef material);
    CSharedComponentRendering getComponentRendering(void) const;
    CSharedComponentDebugRendering getComponentDebugRendering(void) const;
    
    virtual void onBind(CSharedMaterialRef material);
    virtual void onUnbind(CSharedMaterialRef material);
    
    virtual void bindBaseShaderUniforms(CSharedMaterialRef material);
    virtual void bindCustomShaderUniforms(CSharedMaterialRef material);
    
    std::map<E_COMPONENT_CLASS, ISharedComponent> m_components;
    
    void addComponentRendering(void);
    void removeComponentRendering(void);
    
    void addComponentDebugRendering(void);
    void removeComponentDebugRendering(void);
    
public:
    
	IGameObject(CSharedResourceAccessorRef resourceAccessor,
                ISharedRenderTechniqueAccessorRef renderTechniqueAccessor);
    virtual ~IGameObject(void);
    
    void addComponent(ISharedComponentRef component);
    void removeComponent(ISharedComponentRef component);
    void removeComponents(void);
    bool isComponentExist(E_COMPONENT_CLASS componentClass) const;
    ISharedComponent getComponent(E_COMPONENT_CLASS componentClass) const;
    
    virtual void setPosition(const glm::vec3& position);
    virtual void setRotation(const glm::vec3& rotation);
    virtual void setScale(const glm::vec3& scale);
    
    glm::vec3 getPosition(void) const;
    glm::vec3 getRotation(void) const;
    glm::vec3 getScale(void) const;
    
    glm::mat4 getMMatrix(void) const;
    glm::mat4 getMVPMatrix(void) const;
    glm::mat4 getIMVPMatrix(void) const;
    
    glm::vec3 getMaxBound(void) const;
    glm::vec3 getMinBound(void) const;
    
    virtual void setCamera(CSharedCameraRef camera);
    virtual void setCameraFrustum(CSharedFrustumRef cameraFrustum);
    virtual void setGlobalLightSource(CSharedGlobalLightSourceRef globalLightSource);
    
    CSharedVertexBuffer getVertexBuffer(void) const;
    CSharedIndexBuffer getIndexBuffer(void) const;
    
    CSharedMaterial getMaterial(const std::string& renderTechique) const;
    
    template<typename T_VALUE>
    void setCustomShaderUniform(T_VALUE value, const std::string& uniform, const std::string& techniqueName)
    {
        if (IGameObject::isComponentExist(E_COMPONENT_CLASS_RENDERING))
        {
            CSharedComponentRendering componentRendering = std::static_pointer_cast<CComponentRendering>(IGameObject::getComponent(E_COMPONENT_CLASS_RENDERING));
            componentRendering->setCustomShaderUniform(value, uniform, techniqueName);
        }
        else
        {
            assert(false);
        }
    };
    
    virtual CSharedVertexBuffer getCollisionVertexBuffer(void) const;
    virtual CSharedIndexBuffer getCollisionIndexBuffer(void) const;
    
    void setTexture(CSharedTextureRef texture, E_SHADER_SAMPLER sampler, const std::string& techniqueName = "");
    
    virtual void removeLoadingDependencies(void);
    
    virtual void setRenderTechniqueImporter(ISharedRenderTechniqueImporterRef techniqueImporter);
    virtual void setRenderTechniqueAccessor(ISharedRenderTechniqueAccessorRef techniqueAccessor);
    virtual void setSceneUpdateMgr(CSharedSceneUpdateMgrRef sceneUpdateMgr);
};

#endif 
