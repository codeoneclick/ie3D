#include "HCommon.h"

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