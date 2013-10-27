//
//  Win32-Main.cpp
//  indi2dEngine-Demo
//
//  Created by Sergey Sergeev on 5/3/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#if defined(__WIN32__)

#include "CGameViewController_win32.h"
#include "CGameLoopExecutor.h"

int main(int argc, const char* argv[])
{
	CGameViewController_win32* controller = new CGameViewController_win32();
	Run();
	return 0;
}

#endif