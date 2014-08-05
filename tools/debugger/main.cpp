// Тестовое приложение для отладчика.

#include <Windows.h>
#include <tchar.h>

#include "debugger.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	DebugOpen (_T("Debugger"), _T("Test Session"));

	MessageBox (0, _T("Press key to exit"), _T("Blocker"), 0);

	DebugDestroy ();
}