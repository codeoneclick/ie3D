//
//  CGameViewController_win32.h
//  indi2dEngine-Core
//
//  Created by Sergey Sergeev on 5/3/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "HCommon.h"

#ifndef CGameViewController_win32_h
#define CGameViewController_win32_h

#if defined(__WIN32__)

class IOGLWindow;
class IGameWorkflow;
class CGameViewController_win32
{
private:

protected:

	IOGLWindow* m_window;
	IGameWorkflow* m_workflow;

public:

	CGameViewController_win32(void);
	~CGameViewController_win32(void);
};

#endif

#endif