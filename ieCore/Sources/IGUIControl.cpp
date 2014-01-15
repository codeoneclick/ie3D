//
//  IGUIControl.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 10/10/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "IGUIControl.h"
#include "CQuad.h"
#include "CShader.h"
#include "CMaterial.h"
#include "CRenderMgr.h"
#include "CSceneUpdateMgr.h"

IGUIControl::IGUIControl(const std::shared_ptr<CResourceAccessor>& _resourceAccessor, const std::shared_ptr<IScreenSpaceTextureAccessor>& _screenSpaceTextureAccessor) :
m_resourceAccessor(_resourceAccessor),
m_screenSpaceTextureAccessor(_screenSpaceTextureAccessor),
m_position(glm::vec3(0.0f, 0.0f, 0.0f)),
m_scale(glm::vec3(1.0f, 1.0f, 1.0f)),
m_renderQueuePosition(0),
m_quad(nullptr),
m_renderMgr(nullptr),
m_sceneUpdateMgr(nullptr),
m_material(nullptr),
m_parent(nullptr),
m_isNeedToRender(false),
m_status(E_LOADING_STATUS_UNLOADED)
{

}

IGUIControl::~IGUIControl(void)
{
   
}

std::shared_ptr<CVertexBuffer> IGUIControl::Get_HardwareVertexBuffer(void)
{
	assert(m_quad != nullptr);
    assert(m_quad->Get_VertexBuffer() != nullptr);
    return m_quad->Get_VertexBuffer();
}

std::shared_ptr<CIndexBuffer> IGUIControl::Get_HardwareIndexBuffer(void)
{
    assert(m_quad != nullptr);
    assert(m_quad->Get_IndexBuffer() != nullptr);
    return m_quad->Get_IndexBuffer();
}

void IGUIControl::ListenRenderMgr(bool _value)
{
    if(_value)
	{
		assert(m_renderMgr != nullptr);
	}

	if(m_material != nullptr)
    {
		if(m_renderMgr != nullptr)
		{
			_value == true ? m_renderMgr->RegisterWorldSpaceRenderHandler(m_materialModeName, shared_from_this()) :
			m_renderMgr->UnregisterWorldSpaceRenderHandler(m_materialModeName, shared_from_this());
		}
    }

	m_isNeedToRender = _value;
}

void IGUIControl::ListenSceneUpdateMgr(bool _value)
{
    assert(m_sceneUpdateMgr != nullptr);
    _value == true ? m_sceneUpdateMgr->RegisterSceneUpdateHandler(shared_from_this()) :
    m_sceneUpdateMgr->UnregisterSceneUpdateHandler(shared_from_this());
}

void IGUIControl::_OnTemplateLoaded(std::shared_ptr<I_RO_TemplateCommon> _template)
{
    assert(false);
}

void IGUIControl::_OnResourceLoaded(std::shared_ptr<IResource> _resource, bool _success)
{
    switch (_resource->Get_Class())
    {
        case E_RESOURCE_CLASS_SHADER:
        {
            m_status |= E_LOADING_STATUS_SHADER_LOADED;
        }
            break;
        case E_RESOURCE_CLASS_MESH:
        {
            m_status |= E_LOADING_STATUS_MESH_LOADED;
        }
            break;
        case E_RESOURCE_CLASS_SKELETON:
        {
            m_status |= E_LOADING_STATUS_SKELETON_LOADED;
        }
            break;
        case E_RESOURCE_CLASS_SEQUENCE:
        {
            m_status |= E_LOADING_STATUS_SEQUENCE_LOADED;
        }
        default:
            break;
    }
}

void IGUIControl::_OnSceneUpdate(f32 _deltatime)
{
    m_matrixTranslation = glm::translate(glm::mat4(1.0f), m_position);
    m_matrixScale = glm::scale(glm::mat4(1.0f), m_scale);
    m_matrixWorld = m_matrixTranslation * m_matrixScale;
}

i32 IGUIControl::_OnQueuePosition(void)
{
	return 65535;
}

bool IGUIControl::_OnOcclusion(void)
{
    return false;
}

ui32 IGUIControl::_OnGet_NumTriangles(void)
{
    return 0;
}

void IGUIControl::_OnBatch(const std::string& _mode)
{
    
}

void IGUIControl::_OnBind(const std::string &_mode)
{
	assert(m_materialModeName == _mode);
	assert(m_material != nullptr);

    /*if(!m_material->Get_IsBatching())
    {
		assert(m_quad != nullptr);
        m_material->Bind();
        m_quad->Bind(m_material->Get_Shader()->Get_Attributes());
    }*/
}

void IGUIControl::_OnDraw(const std::string &_mode)
{
	assert(m_quad != nullptr);
    m_quad->Draw();
}

void IGUIControl::_OnUnbind(const std::string &_mode)
{
    assert(m_materialModeName == _mode);
	assert(m_material != nullptr);
    
    /*if(!m_material->Get_IsBatching())
    {
		assert(m_quad != nullptr);
        m_material->Unbind();
        m_quad->Unbind(m_material->Get_Shader()->Get_Attributes());
    }*/
}

void IGUIControl::_OnDebugDraw(const std::string &_mode)
{

}

