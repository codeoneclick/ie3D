#include "IOGLWindow.h"

#if defined(__WIN32__)

#include "CCommonOS.h"
#include "IInputContext.h"


IOGLWindow::IOGLWindow(void)
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
	windowClass.lpfnWndProc = (WNDPROC)IInputContext::InputProcess;          
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

#if defined(__USE_OPENGL__)
	static PIXELFORMATDESCRIPTOR pixelFormatDescriptor =				
	{
		sizeof(PIXELFORMATDESCRIPTOR),				
		1,											
		PFD_DRAW_TO_WINDOW |						
		PFD_SUPPORT_OPENGL |						
		PFD_DOUBLEBUFFER,							
		PFD_TYPE_RGBA,							
		16,										
		0, 0, 0, 0, 0, 0,							
		0,											
		0,											
		0,											
		0, 0, 0, 0,									
		16,											  
		0,											
		0,											
		PFD_MAIN_PLANE,								
		0,											
		0, 0, 0
	};
#endif
	if (!(m_hDC = GetDC(m_hWnd)))                                                        
	{
		MessageBox(NULL, L"Can't Create GL Device Context.", L"indieEngine", MB_OK | MB_ICONEXCLAMATION);
		return;                                                        
	}
#if defined(__USE_OPENGL__)
	GLuint pixelFormat = 0;
	if (!(pixelFormat = ChoosePixelFormat(m_hDC, &pixelFormatDescriptor)))
	{
		MessageBox(NULL, L"Can't ChoosePixelFormat.", L"indieEngine", MB_OK | MB_ICONEXCLAMATION);
		return; 
	}

	if(!SetPixelFormat(m_hDC, pixelFormat, &pixelFormatDescriptor))
	{
		MessageBox(NULL, L"Can't SetPixelFormat.", L"indieEngine", MB_OK | MB_ICONEXCLAMATION);
		return; 		
	}
#endif


	ShowWindow(m_hWnd, SW_SHOW);  
	UpdateWindow(m_hWnd);     
}

IOGLWindow::~IOGLWindow(void)
{

}

#endif