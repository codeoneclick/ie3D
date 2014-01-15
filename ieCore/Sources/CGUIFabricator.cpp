//
//  CGUIFabricator.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 10/10/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CGUIFabricator.h"
#include "CTemplateAccessor.h"
#include "CResourceAccessor.h"
#include "CGUIContainer.h"
#include "CGUIButton.h"

CGUIFabricator::CGUIFabricator(const std::shared_ptr<CTemplateAccessor>& _templateAccessor, const std::shared_ptr<CResourceAccessor>& _resourceAccessor) :
IFabricator(_templateAccessor, _resourceAccessor)
{

}

CGUIFabricator::~CGUIFabricator(void)
{
	m_controlsContainer.clear();
}

std::shared_ptr<CGUIContainer> CGUIFabricator::CreateGUIContainer(const std::string& _filename)
{
	assert(m_resourceAccessor != nullptr);
	assert(m_screenSpaceTextureAccessor != nullptr);
    std::shared_ptr<CGUIContainer> container = std::make_shared<CGUIContainer>(m_resourceAccessor, m_screenSpaceTextureAccessor);
    assert(m_templateAccessor != nullptr);
	m_templateAccessor->LoadGUITemplate(_filename, container, E_GUI_LOADING_SCHEME_CONTAINER);
    m_controlsContainer.insert(container);
    return container;
}

void CGUIFabricator::DeleteGUIContainer(const std::shared_ptr<CGUIContainer>& _control)
{

}
    
std::shared_ptr<CGUIButton> CGUIFabricator::CreateGUIButton(const std::string& _filename)
{
	return nullptr;
}

void CGUIFabricator::DeleteGUIButton(const std::shared_ptr<CGUIButton>& _control)
{

}