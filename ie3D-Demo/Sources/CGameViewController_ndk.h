//
//  CGameViewController_ndk.h
//  indieEngineDemo
//
//  Created by Sergey Sergeev on 11/29/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CGameViewController_ndk_h
#define CGameViewController_ndk_h

#if defined(__NDK__)

class IOGLWindow;
class IGameController;
class CGameViewController_ndk
{
private:
    
protected:
    
	IOGLWindow* m_window;
	IGameController* m_workflow;
    
public:
    
	CGameViewController_ndk(void);
	~CGameViewController_ndk(void);
};

#endif

#endif
