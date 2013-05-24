//
//  CGameObject.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/24/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CGameObject_h
#define CGameObject_h

#include "HCommon.h"
#include "HEnums.h"
#include "IRenderHandler.h"
#include "ISceneUpdateHandler.h"

class CCamera;
class CLight;
class CShape;
class CTexture;
class CMaterial;
class CRenderMgr;
class CSceneUpdateMgr;
class CResourceFabricatior;

class CGameObject :
public IRenderHandler,
public ISceneUpdateHandler
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
    std::shared_ptr<CShape> m_shape;
    
    std::shared_ptr<CCamera> m_camera;
    std::shared_ptr<CLight> m_light;
    
    std::shared_ptr<CRenderMgr> m_renderMgr;
    std::shared_ptr<CSceneUpdateMgr> m_sceneUpdateMgr;
    
    bool _IsBoundBoxInFrustum(void);
    
    virtual void _OnSceneUpdate(f32 _deltatime);
    
    virtual i32 _OnQueuePosition(void);
    virtual void _OnBind(const std::string& _renderMode);
    virtual void _OnDraw(const std::string& _renderMode);
    virtual void _OnUnbind(const std::string& _renderMode);
    
public:
    
    CGameObject(void);
    virtual ~CGameObject(void);
    
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
    
    virtual void Load(std::shared_ptr<CResourceFabricatior> _resourceFabricator, const std::string& _filename) = 0;
    
    glm::vec3 Get_MaxBound(void);
    glm::vec3 Get_MinBound(void);
    
    virtual inline void Set_Camera(std::shared_ptr<CCamera> _camera)
    {
        m_camera = _camera;
    };
    
    virtual inline void Set_Light(std::shared_ptr<CLight> _light)
    {
        m_light = _light;
    };
    
    void Set_Texture(std::shared_ptr<CTexture> _texture, E_SHADER_SAMPLER _sampler, const std::string& _renderMode);
    void Set_Clipping(const glm::vec4& _clipping, const std::string& _renderMode);
    
    virtual inline void Set_RenderMgr(std::shared_ptr<CRenderMgr> _renderMgr)
    {
        m_renderMgr = _renderMgr;
    };
    
    virtual inline void Set_SceneUpdateMgr(std::shared_ptr<CSceneUpdateMgr> _sceneUpdateMgr)
    {
        m_sceneUpdateMgr = _sceneUpdateMgr;
    };
    
    virtual void ListenRenderMgr(bool _value);
    virtual void ListenSceneUpdateMgr(bool _value);
};

#endif 
