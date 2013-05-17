//
//  CRoot.cpp
//  indi2dEngine
//
//  Created by Sergey Sergeev on 5/14/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CRoot.h"

#ifdef __APPLE__

#include "COGLContext_iOS.h"

#else

#include "CGLWindow_Win32.h"
#include "CGLContext_Win32.h"

#endif

CRoot::CRoot(void* _glWindow)
{
#ifdef __APPLE__
    
	UIView* glWindow = (__bridge UIView*)_glWindow;
    assert([[glWindow layer] isKindOfClass:[CAEAGLLayer class]]);
    m_glContext = new COGLContext_iOS(static_cast<CAEAGLLayer*>(glWindow.layer));
    
#else
    
#endif
    
    /*m_renderMgr = new CRenderMgr(m_glContext);
    m_updateMgr = new CSceneUpdateMgr();
    m_collisionMgr = new CCollisionMgr();
    
    m_shaderComposite = m_renderMgr->Get_ShaderComposite();
    
	m_uiMgr = new CUIMgr();
    m_uiMgr->Set_ShaderComposite(m_shaderComposite);
	m_uiMgr->Set_ResourceMgrFacade(m_resourceMgrsFacade);
    m_renderMgr->AddRenderPresentEventListener(m_uiMgr);
    m_inputContext->AddTapRecognizerListener(m_uiMgr);
    
    
    ConnectToMainLoop(m_renderMgr);
    ConnectToMainLoop(m_updateMgr);
    ConnectToMainLoop(m_collisionMgr);*/
}

CRoot::~CRoot(void)
{
    
}