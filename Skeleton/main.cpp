#include <Windows.h>
#include <gl/GL.h>
#include <string>
#include <math.h>
#include "GLWindow.h"

HWND g_hWnd;
const UINT WIN_WIDTH = 600;
const UINT WIN_HEIGHT = 400;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	std::string ClassName("MyWindowClass");
	WNDCLASSEX wcx;
	wcx.cbSize		= sizeof(wcx);
	wcx.style		= 0;
	wcx.lpfnWndProc		= DefWindowProc;
	wcx.cbClsExtra		= 0;
	wcx.cbWndExtra		= 0;
	wcx.hInstance		= hInstance;
	wcx.hIcon		= NULL;
	wcx.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcx.hbrBackground	= (HBRUSH) 2;
	wcx.lpszMenuName	= NULL;
	wcx.lpszClassName	= (LPCWSTR)ClassName.c_str();
	wcx.hIconSm		= NULL;

	ATOM Atom = RegisterClassEx(&wcx);
	
	std::string WindowName("GLWindow Test");
	g_hWnd = CreateWindowEx(0,
		(LPCWSTR)ClassName.c_str(),
		(LPCWSTR)WindowName.c_str(),
		WS_OVERLAPPED | WS_VISIBLE | WS_TILEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		WIN_WIDTH, WIN_HEIGHT,
		HWND_DESKTOP,
		(HMENU) NULL,
		hInstance,
		(LPVOID) NULL);

    GLWindow glWnd(g_hWnd, WIN_WIDTH, WIN_HEIGHT);
	glWnd.Initialize();

///////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////
	//////////////// MY CODE STARTS HERE //////////////////////////////////

	// Draw a diagonal line
	for (UINT x=0, y; x<WIN_WIDTH; x++)
	{
		glWnd.SetColor(255,	(GLbyte) (255.0 * x / (double) WIN_WIDTH),
					(GLbyte) (255.0 * x / (double) WIN_WIDTH));
		y = ((double) WIN_HEIGHT / WIN_WIDTH) * x;
		glWnd.PutPixel(x, y);
	}
	
	// Display it on the screen
	glWnd.SwapBuffers();

	// Draw a quarter of a circle
	UINT x, y;
	for (double teta=0.0; teta<1.5708 /* PI/2 */; teta+=0.001)
	{
		x = WIN_HEIGHT * cos(teta);
		y = WIN_HEIGHT * sin(teta);
		glWnd.SetColor(	(GLbyte) (150.0 * teta),
				(GLbyte) (256.0 - 150.0 * teta),
				128.0);
		glWnd.PutPixel(x, y);
	}

	// Switch between line and circle several times
	for (UINT i=0; i<10; i++)
	{
		glWnd.SwapBuffers();
		Sleep(1000);
	}
	
	// Close OpenGL engine
	glWnd.Destroy();

	// Quit the program
	PostQuitMessage(0);

	/////////////// MY CODE ENDS HERE 

    MSG Msg;
	const UINT wMsgFilterMin = 0;
	const UINT wMsgFilterMax = 0;
	while (GetMessage(&Msg, (HWND) NULL, wMsgFilterMin, wMsgFilterMax) > 0)
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}

	return 0;
}