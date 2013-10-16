//
//  IOGLWindow.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/17/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef IOGLWindow_h
#define IOGLWindow_h

#include "HCommon.h"

#if defined(__APPLE__)

#include <Foundation/Foundation.h>
#include <UIKit/UIKit.h>
#include <QuartzCore/QuartzCore.h>

@interface IOGLWindow : UIView

@end

#elif defined(__WIN32__)

class IOGLWindow
{
private:

	HWND m_hWnd;
	HDC	m_hDC;

protected:

public:

	IOGLWindow(void);
	~IOGLWindow(void);

	inline HWND Get_HWND(void)
	{
		return m_hWnd;
	};

	inline HDC Get_HDC(void)
	{
		return m_hDC;
	};
};

#endif

#endif 