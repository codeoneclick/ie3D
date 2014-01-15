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
#include "IRenderHandler.h"
#include "ISceneUpdateHandler.h"
#include "ITemplateLoadingHandler.h"
#include "IResourceLoadingHandler.h"

class CCamera;
class CLight;
class CMesh;
class CVertexBuffer;
class CIndexBuffer;
class CTexture;
class CMaterial;
class CRenderMgr;
class CSceneUpdateMgr;
class CResourceAccessor;
class IScreenSpaceTextureAccessor;
class CAABoundBox;

class IGameObject :
public std::enable_shared_from_this<IGameObject>,
public IRenderHandler,
public ISceneUpdateHandler,
public ITemplateLoadingHandler,
public IResourceLoadingHandler
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
    
    std::map<std::string, std::shared_ptr<CMaterial> > m_materials;
    std::shared_ptr<I_RO_TemplateCommon> m_template;
    std::shared_ptr<CMaterial> m_debugBoundBoxMaterial;
    std::shared_ptr<CMesh> m_mesh;
    
    std::shared_ptr<CCamera> m_camera;
    std::array<std::shared_ptr<CLight>, E_LIGHT_MAX> m_lights;
    std::shared_ptr<CAABoundBox> m_boundBox;
    
    std::shared_ptr<CRenderMgr> m_renderMgr;
    std::shared_ptr<CSceneUpdateMgr> m_sceneUpdateMgr;
    std::shared_ptr<CResourceAccessor> m_resourceAccessor;
	std::shared_ptr<IScreenSpaceTextureAccessor> m_screenSpaceTextureAccessor;
    
    ui8 m_status;
    ui32 m_renderQueuePosition;
    std::function<void(std::shared_ptr<CMaterial>)> m_materialImposer;
	bool m_isNeedToRender;
    bool m_isNeedToUpdate;
    bool m_isBatching;
    
    bool _IsBoundBoxInFrustum(void);
    
    virtual void _OnSceneUpdate(f32 _deltatime);
    
    virtual void _OnResourceLoaded(std::shared_ptr<IResource> _resource, bool _success);
    virtual void _OnTemplateLoaded(std::shared_ptr<I_RO_TemplateCommon> _template);
    
    virtual i32 _OnQueuePosition(void);
    virtual bool _OnOcclusion(void);
    virtual ui32 _OnGet_NumTriangles(void);
    virtual void _OnBind(const std::string& _mode);
    virtual void _OnDraw(const std::string& _mode);
    virtual void _OnUnbind(const std::string& _mode);
    virtual void _OnDebugDraw(const std::string& _mode);
    virtual void _OnBatch(const std::string& _mode);
    
public:
    
	IGameObject(const std::shared_ptr<CResourceAccessor>& _resourceAccessor, const std::shared_ptr<IScreenSpaceTextureAccessor>& _screenSpaceTextureAccessor);
    virtual ~IGameObject(void);
    
    inline void Set_Position(const glm::vec3& _position)
    {
        m_position = _position;
    };
    
    inline glm::vec3 Get_Position(void)
    {
        return m_position;
    };
    
    inline void Set_Rotation(const glm::vec3& _rotation)
    {
        m_rotation = _rotation;
    };
    
    inline glm::vec3 Get_Rotation(void)
    {
        return m_rotation;
    };
    
    inline void Set_Scale(const glm::vec3& _scale)
    {
        m_scale = _scale;
    };
    
    inline glm::vec3 Get_Scale(void)
    {
        return m_scale;
    };
    
    inline glm::mat4x4 Get_WorldMatrix(void)
    {
        return m_matrixWorld;
    };
    
    glm::vec3 Get_MaxBound(void);
    glm::vec3 Get_MinBound(void);
    
    virtual void Set_Camera(std::shared_ptr<CCamera> _camera);
    
    virtual inline void Set_Light(std::shared_ptr<CLight> _light, E_LIGHTS _id)
    {
        m_lights[_id] = _light;
    };
    
    std::shared_ptr<CVertexBuffer> Get_HardwareVertexBuffer(void);
    std::shared_ptr<CIndexBuffer> Get_HardwareIndexBuffer(void);
    
    std::shared_ptr<CVertexBuffer> Get_BoundVertexBuffer(void);
    std::shared_ptr<CIndexBuffer> Get_BoundIndexBuffer(void);
    
    void Set_Texture(std::shared_ptr<CTexture> _texture, E_SHADER_SAMPLER _sampler, const std::string& _renderMode);
    void Set_ClippingPlane(const glm::vec4& _clippingPlane, const std::string& _renderMode);
    
    virtual inline void Set_RenderMgr(std::shared_ptr<CRenderMgr> _renderMgr)
    {
        m_renderMgr = _renderMgr;
    };
    
    virtual inline void Set_SceneUpdateMgr(std::shared_ptr<CSceneUpdateMgr> _sceneUpdateMgr)
    {
        m_sceneUpdateMgr = _sceneUpdateMgr;
    };
    
    virtual ui32 Get_NumTriangles(void);
    
    virtual void ListenRenderMgr(bool _value);
    virtual void ListenSceneUpdateMgr(bool _value);
    
};

#endif 
