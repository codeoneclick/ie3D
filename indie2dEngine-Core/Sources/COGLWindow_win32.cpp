
#include "COGLWindow.h"
#include "CCommonOS.h"

CGLWindow_Win32::CGLWindow_Win32(void)
{
	m_hWnd = 0;
	m_hDC = 0;

	WNDCLASS windowClass;                                    
	DWORD windowExtStyle =  WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;                        
	DWORD windowStyle = WS_OVERLAPPEDWINDOW;                    
	RECT windowRectangle;                                
	windowRectangle.left = 0;                      
	windowRectangle.right = Get_ScreenWidth();                
	windowRectangle.top = 0;                        
	windowRectangle.bottom = Get_ScreenHeight();                      

	windowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;  
	windowClass.lpfnWndProc = (WNDPROC)CInputContext::InputProcess;          
	windowClass.cbClsExtra = 0;                                                                    
	windowClass.cbWndExtra = 0;                                                            
	windowClass.hInstance = GetModuleHandle(NULL);                                            
	windowClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);                  
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);                  
	windowClass.hbrBackground = NULL;                                                                
	windowClass.lpszMenuName = NULL;                                                
	windowClass.lpszClassName = L"indieEngine";                                            

	if (!RegisterClass(&windowClass))                                                                      
	{
		MessageBox(NULL,L"Failed To Register The Window Class.", L"indieEngine" ,MB_OK | MB_ICONEXCLAMATION);
		return;                                                                                
	}

	AdjustWindowRectEx(&windowRectangle, windowStyle, false , windowExtStyle);
	if (!(m_hWnd = CreateWindowEx(windowExtStyle,                                                  
		L"indieEngine",                                              
		L"indieEngine",                                                              
		windowStyle |                                                  
		WS_CLIPSIBLINGS |                                      
		WS_CLIPCHILDREN,                                        
		0, 0,                                                          
		windowRectangle.right - windowRectangle.left,  
		windowRectangle.bottom - windowRectangle.top,  
		NULL,                                                          
		NULL,                                                          
		GetModuleHandle(NULL),                                                      
		NULL)))                                                
	{
		MessageBox(NULL, L"Window Creation Error.", L"indieEngine", MB_OK | MB_ICONEXCLAMATION);            
		return;                                                        
	}

	if (!(m_hDC = GetDC(m_hWnd)))                                                        
	{
		MessageBox(NULL, L"Can't Create GL Device Context.", L"indieEngine", MB_OK | MB_ICONEXCLAMATION);
		return;                                                        
	}

	ShowWindow(m_hWnd, SW_SHOW);  
	UpdateWindow(m_hWnd);     
}