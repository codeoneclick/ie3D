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

class CCamera;
class CLight;
class CShape;
class CMesh;
class CTexture;
class CMaterial;
class CRenderMgr;
class CSceneUpdateMgr;
class CResourceAccessor;

class IGameObject :
public std::enable_shared_from_this<IGameObject>,
public IRenderHandler,
public ISceneUpdateHandler,
public ITemplateLoadingHandler
{
private:
    
protected:
    
    glm::mat4x4 m_matrixScale;
    glm::mat4x4 m_matrixRotation;
    glm::mat4x4 m_matrixTranslation;
    glm::mat4x4 m_matrixWorld;
    glm::mat4x4 m_matrixWVP;
    
    glm::vec3 m_position;
    glm::vec3 m_rotation;
    glm::vec3 m_scale;
    glm::vec2 m_texcoordDisplacement;
    
    std::map<std::string, std::shared_ptr<CMaterial> > m_materials;
    std::shared_ptr<CMesh> m_mesh;
    
    std::shared_ptr<CCamera> m_camera;
    std::shared_ptr<CLight> m_light;
    
    std::shared_ptr<CRenderMgr> m_renderMgr;
    std::shared_ptr<CSceneUpdateMgr> m_sceneUpdateMgr;
    std::shared_ptr<CResourceAccessor> m_resourceFabricator;
    
    bool m_isLoaded;
    bool m_isLazyListening;
    
    bool _IsBoundBoxInFrustum(void);
    
    virtual void _OnSceneUpdate(f32 _deltatime);
    
    virtual i32 _OnQueuePosition(void);
    virtual void _OnBind(const std::string& _renderMode);
    virtual void _OnDraw(const std::string& _renderMode);
    virtual void _OnUnbind(const std::string& _renderMode);
    
    virtual void _OnTemplateLoaded(std::shared_ptr<ITemplate> _template);
    
    virtual void _LazyListenRenderMgr(void);
    
public:
    
    IGameObject(std::shared_ptr<CResourceAccessor> _resourceFabricator);
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
    
    inline void Set_TexcoordDisplacement(const glm::vec2& _texcoordDisplacement)
    {
        m_texcoordDisplacement = _texcoordDisplacement;
    };
    
    inline glm::vec2 Get_TexcoordDisplacement(void)
    {
        return m_texcoordDisplacement;
    };
    
    glm::vec3 Get_MaxBound(void);
    glm::vec3 Get_MinBound(void);
    
    inline void Set_Camera(std::shared_ptr<CCamera> _camera)
    {
        m_camera = _camera;
    };
    
    virtual inline void Set_Light(std::shared_ptr<CLight> _light)
    {
        m_light = _light;
    };
    
    inline std::shared_ptr<CMesh> Get_Mesh(void)
    {
        assert(m_mesh != nullptr);
        return m_mesh;
    };
    
    inline glm::mat4x4 Get_WorldMatrix(void)
    {
        return m_matrixWorld;
    };
    
    void Set_Texture(std::shared_ptr<CTexture> _texture, E_SHADER_SAMPLER _sampler, const std::string& _renderMode);
    void Set_Clipping(const glm::vec4& _clipping, const std::string& _renderMode);
    
    inline void Set_RenderMgr(std::shared_ptr<CRenderMgr> _renderMgr)
    {
        m_renderMgr = _renderMgr;
    };
    
    inline void Set_SceneUpdateMgr(std::shared_ptr<CSceneUpdateMgr> _sceneUpdateMgr)
    {
        m_sceneUpdateMgr = _sceneUpdateMgr;
    };
    
    virtual void ListenRenderMgr(bool _value);
    virtual void ListenSceneUpdateMgr(bool _value);
};

#endif 
