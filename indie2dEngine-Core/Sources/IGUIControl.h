//
//  IGUIControl.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 10/10/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef IGUIControl_h
#define IGUIControl_h

#include "HCommon.h"
#include "IRenderHandler.h"
#include "ISceneUpdateHandler.h"
#include "ITemplateLoadingHandler.h"
#include "IResourceLoadingHandler.h"

class CQuad;
class CVertexBuffer;
class CIndexBuffer;
class CTexture;
class CMaterial;
class CRenderMgr;
class CSceneUpdateMgr;
class CResourceAccessor;
class IScreenSpaceTextureAccessor;

class IGUIControl :
public std::enable_shared_from_this<IGUIControl>,
public IRenderHandler,
public ISceneUpdateHandler,
public ITemplateLoadingHandler,
public IResourceLoadingHandler
{
private:

	friend class CGUIContainer;

protected:

	glm::mat4x4 m_matrixScale;
    glm::mat4x4 m_matrixTranslation;
    glm::mat4x4 m_matrixWorld;

	std::shared_ptr<CQuad> m_quad;
	std::shared_ptr<CMaterial> m_material;
	std::string m_materialModeName;

	glm::vec3 m_position;
	glm::vec3 m_scale;
	glm::vec2 m_size;

	ui8 m_status;
    ui32 m_renderQueuePosition;
	std::function<void(std::shared_ptr<CMaterial>)> m_materialImposer;
	bool m_isNeedToRender;

	std::shared_ptr<IGUIControl> m_parent;

	inline void _Set_Parent(const std::shared_ptr<IGUIControl> _parent) 
	{
		m_parent = _parent;
	};

	std::shared_ptr<CRenderMgr> m_renderMgr;
    std::shared_ptr<CSceneUpdateMgr> m_sceneUpdateMgr;
    std::shared_ptr<CResourceAccessor> m_resourceAccessor;
	std::shared_ptr<IScreenSpaceTextureAccessor> m_screenSpaceTextureAccessor;

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

	IGUIControl(const std::shared_ptr<CResourceAccessor>& _resourceAccessor, const std::shared_ptr<IScreenSpaceTextureAccessor>& _screenSpaceTextureAccessor);
	virtual ~IGUIControl(void);

	inline void Set_Position(const glm::vec2& _position)
	{
		m_position.x = _position.x;
		m_position.y = _position.y;
		m_position.z = 0.0f;
	};

	inline glm::vec2 Get_Position(void)
	{
		return glm::vec2(m_position.x, m_position.y);
	};

	void Set_Size(const glm::vec2& _size);
	glm::vec2 Get_Size(void);

	inline glm::mat4x4 Get_WorldMatrix(void)
    {
        return m_matrixWorld;
    };

	inline std::shared_ptr<IGUIControl> Get_Parent(void)
	{
		return m_parent;
	};
    
    std::shared_ptr<CVertexBuffer> Get_HardwareVertexBuffer(void);
    std::shared_ptr<CIndexBuffer> Get_HardwareIndexBuffer(void);
    
    std::shared_ptr<CVertexBuffer> Get_BoundVertexBuffer(void);
    std::shared_ptr<CIndexBuffer> Get_BoundIndexBuffer(void);

	inline void Set_RenderMgr(std::shared_ptr<CRenderMgr> _renderMgr)
    {
		assert(_renderMgr != nullptr);
        m_renderMgr = _renderMgr;
    };
    
    inline void Set_SceneUpdateMgr(std::shared_ptr<CSceneUpdateMgr> _sceneUpdateMgr)
    {
		assert(_sceneUpdateMgr != nullptr);
        m_sceneUpdateMgr = _sceneUpdateMgr;
    };
    
    virtual void ListenRenderMgr(bool _value);
    virtual void ListenSceneUpdateMgr(bool _value);
};

#endif 
