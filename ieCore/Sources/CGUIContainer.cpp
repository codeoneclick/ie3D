//
//  IGUIControl.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 10/10/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CGUIContainer.h"
#include "CQuad.h"
#include "CMaterial.h"
#include "CShader.h"
#include "HEnums.h"
#include "CResourceAccessor.h"
#include "CRenderMgr.h"
#include "CBatchingMgr.h"
#include "CVertexBuffer.h"
#include "CCommonOS.h"

CGUIContainer::CGUIContainer(const std::shared_ptr<CResourceAccessor>& _resourceAccessor, const std::shared_ptr<IScreenSpaceTextureAccessor>& _screenSpaceTextureAccessor) :
IGUIControl(_resourceAccessor, _screenSpaceTextureAccessor)
{
	m_materialImposer = [this](std::shared_ptr<CMaterial> _material)
    {
    };
}

CGUIContainer::~CGUIContainer(void)
{

}

void CGUIContainer::RegisterControl(const std::shared_ptr<IGUIControl>& _control)
{
	m_controls.push_back(_control);
	_control->_Set_Parent(shared_from_this());
}

void CGUIContainer::UnregisterControl(const std::shared_ptr<IGUIControl>& _control)
{
	auto control = std::find(m_controls.begin(), m_controls.end(), _control);
	assert(control != m_controls.end());
	m_controls.erase(control);
	_control->_Set_Parent(nullptr);
}

void CGUIContainer::_OnResourceLoaded(std::shared_ptr<IResource> _resource, bool _success)
{
	IGUIControl::_OnResourceLoaded(_resource, _success);
}

void CGUIContainer::_OnTemplateLoaded(std::shared_ptr<I_RO_TemplateCommon> _template)
{
	/*std::shared_ptr<SGUIContainerTemplate> guicontainerTemplate = std::static_pointer_cast<SGUIContainerTemplate>(_template);
    assert(m_resourceAccessor != nullptr);
	m_quad = m_quad = std::make_shared<CQuad>();
    assert(m_quad != nullptr);
    
	m_size = glm::vec2(guicontainerTemplate->m_width, guicontainerTemplate->m_height);

	std::shared_ptr<CShader> shader = m_resourceAccessor->CreateShader(guicontainerTemplate->m_materialsTemplate->m_shaderTemplate->m_vsFilename,
                                                                       guicontainerTemplate->m_materialsTemplate->m_shaderTemplate->m_fsFilename);
    assert(shader != nullptr);
	m_material = std::make_shared<CMaterial>(shader, guicontainerTemplate->m_materialsTemplate->m_filename);
	m_material->Serialize(guicontainerTemplate->m_materialsTemplate, m_resourceAccessor, m_screenSpaceTextureAccessor, shared_from_this());
    m_material->Set_IsBatching(guicontainerTemplate->m_isBatching);
	m_materialModeName = guicontainerTemplate->m_materialsTemplate->m_renderMode;

	IGUIControl::ListenRenderMgr(m_isNeedToRender);
    m_status |= E_LOADING_STATUS_TEMPLATE_LOADED;*/
}

void CGUIContainer::_OnSceneUpdate(f32 _deltatime)
{
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
		IGUIControl::_OnSceneUpdate(_deltatime);

		/*if(m_material != nullptr && !m_material->Get_IsBatching())
		{
			SHardwareVertex* vertexData = m_quad->Get_VertexBuffer()->Lock();

			f32 screenWidth = Get_ScreenWidth();
			f32 screenHeight = Get_ScreenHeight();

			glm::vec4 frame(0.0f);
			frame.x = (m_position.x / screenWidth) * 2.0f - 1.0f;
			frame.y = ((screenHeight - m_position.y) / screenHeight) * 2.0f - 1.0f;
			frame.z = ((m_position.x + m_size.x) / screenWidth) * 2.0f - 1.0f;
			frame.w = ((screenHeight - (m_position.y + m_size.y)) / screenHeight) * 2.0f - 1.0f;

			vertexData[0].m_position = glm::vec3(frame.x, frame.y, 0.0f);
			vertexData[0].m_texcoord = CVertexBuffer::CompressVec2(glm::vec2(0.0f, 0.0f));
			vertexData[1].m_position = glm::vec3(frame.x, frame.w, 0.0f);
			vertexData[1].m_texcoord = CVertexBuffer::CompressVec2(glm::vec2(0.0f, 1.0f));
			vertexData[2].m_position = glm::vec3(frame.z, frame.y, 0.0f);
			vertexData[2].m_texcoord = CVertexBuffer::CompressVec2(glm::vec2(1.0f, 0.0f));
			vertexData[3].m_position = glm::vec3(frame.z, frame.w, 0.0f);
			vertexData[3].m_texcoord = CVertexBuffer::CompressVec2(glm::vec2(1.0f, 1.0f));
			m_quad->Get_VertexBuffer()->Unlock();
		}*/
    }
}

void CGUIContainer::_OnBatch(const std::string& _mode)
{
	assert(m_materialModeName == _mode);
    assert(m_material->Get_Shader() != nullptr);

    /*if(m_material->Get_IsBatching())
    {
		m_renderMgr->Get_BatchingMgr()->Batch(_mode, m_renderQueuePosition, m_quad, m_material, m_materialImposer, glm::vec2(m_position.x, m_position.y), m_size);
    }*/
}

void CGUIContainer::_OnBind(const std::string& _mode)
{
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        assert(m_materialModeName == _mode);
        IGUIControl::_OnBind(_mode);
    }
}

void CGUIContainer::_OnDraw(const std::string& _mode)
{
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        assert(m_materialModeName == _mode);
        assert(m_material->Get_Shader() != nullptr);
        
        /*if(!m_material->Get_IsBatching())
        {
            m_materialImposer(m_material);
            IGUIControl::_OnDraw(_mode);
        }*/
    }
}

void CGUIContainer::_OnUnbind(const std::string& _mode)
{
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        assert(m_materialModeName == _mode);
        IGUIControl::_OnUnbind(_mode);
    }
}