//
//  COGLWindow_iOS.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/17/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef COGLWindow_iOS_h
#define COGLWindow_iOS_h

#include "HCommon.h"

#ifdef __APPLE__

#include <Foundation/Foundation.h>
#include <UIKit/UIKit.h>
#include <QuartzCore/QuartzCore.h>

@interface COGLWindow_iOS : UIView

@end

#else if defined(__WIN32__)

class CGLWindow_Win32
{
private:

	HWND m_hWnd;
	HDC	m_hDC;

protected:

public:

	CGLWindow_Win32(void);
	~CGLWindow_Win32(void);

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
