#include "IInputContext.h"
#include "IInputTapRecognizerHandler.h"

#if defined(__WIN32__)

IInputContext* IInputContext::m_sharedInstance = nullptr;

IInputContext::IInputContext(void* _hwnd) :
m_isTapped(false),
m_mousePosition(POINT())
{
	m_sharedInstance = this;
}

IInputContext::~IInputContext(void)
{

}

void IInputContext::TapRecognizerPressed(const glm::ivec2& _point)
{
	for(const auto& handler : m_handlers)
    {
        handler->_Get_Commands()._ExecuteInputTapRecognizerDidPressedCommand(_point);
    }
}

void IInputContext::TapRecognizerMoved(const glm::ivec2& _point)
{
	for(const auto& handler : m_handlers)
    {
        handler->_Get_Commands()._ExecuteInputTapRecognizerDidMovedCommand(_point);
    }
}
    
void IInputContext::TapRecognizerReleased(const glm::ivec2& _point)
{
	for(const auto& handler : m_handlers)
    {
        handler->_Get_Commands()._ExecuteInputTapRecognizerDidReleasedCommand(_point);
    }
}

void IInputContext::RegisterTapRecognizerHandler(std::shared_ptr<IInputTapRecognizerHandler> _handler)
{
	 m_handlers.insert(_handler);
}

void IInputContext::UnregisterTapRecognizerHandler(std::shared_ptr<IInputTapRecognizerHandler> _handler)
{
	 m_handlers.erase(_handler);
}

LRESULT CALLBACK IInputContext::InputProcess(HWND _hwnd, UINT _message, WPARAM _paramW, LPARAM _paramL)
{
	if(m_sharedInstance == nullptr)
	{
		return DefWindowProc(_hwnd, _message, _paramW, _paramL);
	}

	GetCursorPos(&m_sharedInstance->m_mousePosition);
	ScreenToClient(_hwnd, &m_sharedInstance->m_mousePosition);
	glm::ivec2 mousePosition = glm::ivec2(m_sharedInstance->m_mousePosition.x, m_sharedInstance->m_mousePosition.y);

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
			m_sharedInstance->TapRecognizerPressed(mousePosition);
			m_sharedInstance->m_isTapped = true;
			return 0;              
		}
	case WM_LBUTTONUP :
		{
			m_sharedInstance->TapRecognizerReleased(mousePosition);
			m_sharedInstance->m_isTapped = false;
			return 0;      
		}
	}

	if(m_sharedInstance->m_isTapped)
	{
		m_sharedInstance->TapRecognizerMoved(mousePosition);
	}

	return DefWindowProc(_hwnd, _message, _paramW, _paramL);
}


#endif