#include "CGameViewController_win32.h"
#include "CGameLoopExecutor.h"

int main(int argc, const char* argv[])
{
	CGameViewController_win32* controller = new CGameViewController_win32();
	Run();
	return 0;
}