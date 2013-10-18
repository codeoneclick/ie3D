//
//  IGUIControl.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 10/10/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CGUIContainer.h"


CGUIContainer::CGUIContainer(const std::shared_ptr<CResourceAccessor>& _resourceAccessor) :
IGUIControl(_resourceAccessor)
{

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

}

void CGUIContainer::_OnTemplateLoaded(std::shared_ptr<ITemplate> _template)
{

}