#include "IInputContext.h"

IInputContext::IInputContext(void* _hwnd)
{

}

IInputContext::~IInputContext(void)
{

}

void IInputContext::TapRecognizerPressed(const glm::ivec2& _point)
{

}

void IInputContext::TapRecognizerMoved(const glm::ivec2& _point)
{

}
    
void IInputContext::TapRecognizerReleased(const glm::ivec2& _point)
{

}

void IInputContext::RegisterTapRecognizerHandler(std::shared_ptr<IInputTapRecognizerHandler> _handler)
{

}

void IInputContext::UnregisterTapRecognizerHandler(std::shared_ptr<IInputTapRecognizerHandler> _handler)
{

}

LRESULT CALLBACK IInputContext::InputProcess(HWND _HWND, UINT _message, WPARAM _paramW, LPARAM _paramL)  
{
	switch (_message)                                                                        
	{
	case WM_ACTIVATE:                                              
		{
			return 0;                                                              
		}

	case WM_SYSCOMMAND:                                            
		{
			switch (_paramW)                                        
			{
			case SC_SCREENSAVE:                            
			case SC_MONITORPOWER:                          
				return 0;                                                      
			}
			break;                                                                  
		}

	case WM_CLOSE:                                                          
		{
			PostQuitMessage(0);                                            
			return 0;                                                      
		}

	case WM_KEYDOWN:                                                        
		{                                 
			return 0;                                                              
		}

	case WM_KEYUP:                                                          
		{                                
			return 0;                                                              
		}

	case WM_SIZE:                                                          
		{
			return 0;                                                              
		}

	case WM_RBUTTONDOWN :
		{
			return 0;              
		}
	case WM_RBUTTONUP :
		{
			return 0;      
		}

	case WM_LBUTTONDOWN :
		{
			return 0;              
		}
	case WM_LBUTTONUP :
		{
			return 0;      
		}
	}
	return DefWindowProc(_HWND, _message, _paramW, _paramL);
}
