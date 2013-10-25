//
//  CGUIGraph.cpp
//  indi2dEngine
//
//  Created by Sergey Sergeev on 5/14/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CGUIGraph.h"
#include "CSceneUpdateMgr.h"
#include "IGUIControl.h"
#include "CGUIContainer.h"
#include "CGUIButton.h"

CGUIGraph::CGUIGraph(void)
{

}

CGUIGraph::~CGUIGraph(void)
{
	m_controlsContainer.clear();
}

void CGUIGraph::_InsertGUIControl(const std::shared_ptr<IGUIControl>& _control)
{
    assert(m_sceneUpdateMgr != nullptr);
    assert(m_renderMgr != nullptr);
    
    _control->Set_SceneUpdateMgr(m_sceneUpdateMgr);
    _control->Set_RenderMgr(m_renderMgr);
    _control->ListenSceneUpdateMgr(true);
    _control->ListenRenderMgr(true);
    m_controlsContainer.insert(_control);
}

void CGUIGraph::_RemoveGUIControl(const std::shared_ptr<IGUIControl>& _control)
{
    assert(m_sceneUpdateMgr != nullptr);
    assert(m_renderMgr != nullptr);
    
    _control->Set_SceneUpdateMgr(nullptr);
    _control->Set_RenderMgr(nullptr);
    _control->ListenSceneUpdateMgr(false);
    _control->ListenRenderMgr(false);
    m_controlsContainer.erase(_control);
}

void CGUIGraph::InsertGUIContainer(const std::shared_ptr<CGUIContainer>& _container)
{
    CGUIGraph::_InsertGUIControl(_container);
}

void CGUIGraph::RemoveGUIContainer(const std::shared_ptr<CGUIContainer>& _container)
{
	CGUIGraph::_RemoveGUIControl(_container);
}

void CGUIGraph::InsertGUIButton(const std::shared_ptr<CGUIButton>& _button)
{
    //CGUIGraph::_InsertGUIControl(_button);
}

void CGUIGraph::RemoveGUIButton(const std::shared_ptr<CGUIButton>& _button)
{
    //CGUIGraph::_RemoveGUIControl(_button);
}
