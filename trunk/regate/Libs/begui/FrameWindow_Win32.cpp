#include "FrameWindow_Win32.h"
#include "util.h"
#include "Font.h"

#ifdef _WIN32

using namespace begui;

LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


FrameWindow_Win32::FrameWindow_Win32() :
	m_hInstance(0),
	m_hDC(0),
	m_hRC(0),
	m_hWnd(0)
{
}

FrameWindow_Win32::~FrameWindow_Win32()
{
}

void FrameWindow_Win32::createGLWindow(int left, int top, int width, int height, const std::string &title, const Options &opt)
{
	GLuint		pixelFormat;			// Holds The Results After Searching For A Match
	WNDCLASS	wc;						// Windows Class Structure
	DWORD		dwExStyle;				// Window Extended Style
	DWORD		dwStyle;				// Window Style
	RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left	 =(long)0;			// Set Left Value To 0
	WindowRect.right =(long)width;		// Set Right Value To Requested Width
	WindowRect.top	 =(long)0;			// Set Top Value To 0
	WindowRect.bottom=(long)height;		// Set Bottom Value To Requested Height

	m_options = opt;

	// Grab An Instance For Our Window
	m_hInstance			= GetModuleHandle(NULL);

	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc		= (WNDPROC) WndProc;					// WndProc Handles Messages
	wc.cbClsExtra		= 0;									// No Extra Window Data
	wc.cbWndExtra		= 0;									// No Extra Window Data
	wc.hInstance		= m_hInstance;							// Set The Instance
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	wc.hbrBackground	= NULL;									// No Background Required For GL
	wc.lpszMenuName		= NULL;									// We Don't Want A Menu
	wc.lpszClassName	= "BeGUIFrameWindow";					// Set The Class Name

	// register the window class
	if (!RegisterClass(&wc))
		throw std::exception("Failed To Register The Window Class");

	// try to switch to fullscreen mode
	if (m_options.bFullScreen)
	{
		DEVMODE dmScreenSettings;
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize			= sizeof(dmScreenSettings);		// Size Of The Devmode Structure
		dmScreenSettings.dmPelsWidth	= width;						// Selected Screen Width
		dmScreenSettings.dmPelsHeight	= height;						// Selected Screen Height
		dmScreenSettings.dmBitsPerPel	= m_options.nColorBits;			// Selected Bits Per Pixel
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
		if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
		{
			// If The Mode Fails, Offer Two Options.  Quit Or Use Windowed Mode.
			if (MessageBox(NULL,"The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?","BeGUI",MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
			{
				m_options.bFullScreen = false;
			}
			else
				throw std::exception("Unable to switch to fullscreen mode. User requested termination\n");
		}
	}

	// set the window style
	if (m_options.bFullScreen)
	{
		dwExStyle = WS_EX_APPWINDOW;
		dwStyle = WS_POPUP;
		ShowCursor(FALSE);	// hide mouse cursor
	}
	else
	{
		if (m_options.bOwnDraw) {
			// make a window without caption and border if we are going to render everything ourselves
			dwExStyle = WS_EX_APPWINDOW;
			dwStyle = WS_POPUP;
		}
		else {
			dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
			dwStyle = WS_OVERLAPPEDWINDOW;
		}
	}
	
	// Adjust Window To True Requested Size
	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);

	try {
		// Create The Window
		if (!(m_hWnd = CreateWindowEx( dwExStyle,						// Extended Style For The Window
									"BeGUIFrameWindow",					// Class Name
									title.c_str(),						// Window Title
									dwStyle |							// Defined Window Style
									WS_CLIPSIBLINGS |					// Required Window Style
									WS_CLIPCHILDREN,					// Required Window Style
									0, 0,								// Window Position
									WindowRect.right-WindowRect.left,	// Calculate Window Width
									WindowRect.bottom-WindowRect.top,	// Calculate Window Height
									NULL,								// No Parent Window
									NULL,								// No Menu
									m_hInstance,						// Instance
									NULL)))								// Dont Pass Anything To WM_CREATE
			throw std::exception("CreateWindowEx failed. Cannot create window.");

		// pixel format descriptor
		static	PIXELFORMATDESCRIPTOR pfd =
		{
			sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
			1,											// Version Number
			PFD_DRAW_TO_WINDOW |						// Format Must Support Window
			PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
			PFD_DOUBLEBUFFER,							// Must Support Double Buffering
			PFD_TYPE_RGBA,								// Request An RGBA Format
			m_options.nColorBits,						// Select Our Color Depth
			0, 0, 0, 0, 0, 0,							// Color Bits Ignored
			0,											// No Alpha Buffer
			0,											// Shift Bit Ignored
			0,											// No Accumulation Buffer
			0, 0, 0, 0,									// Accumulation Bits Ignored
			m_options.nDepthBits,						// 16Bit Z-Buffer (Depth Buffer)  
			m_options.nStencilBits,						// No Stencil Buffer
			0,											// No Auxiliary Buffer
			PFD_MAIN_PLANE,								// Main Drawing Layer
			0,											// Reserved
			0, 0, 0										// Layer Masks Ignored
		};

		// Get a device context for the window
		if (!(m_hDC = GetDC(m_hWnd)))
			throw std::exception("Can't Create A GL Device Context");

		// Find a matching pixel format
		if (!(pixelFormat = ChoosePixelFormat(m_hDC, &pfd)))
			throw std::exception("Can't Find A Suitable PixelFormat");

		// Set the pixel format
		if(!SetPixelFormat(m_hDC, pixelFormat, &pfd))
			throw std::exception("Can't Set The PixelFormat");

		// Create a rendering context
		if (!(m_hRC = wglCreateContext(m_hDC)))
			throw std::exception("Can't Create A GL Rendering Context");

		// Activate the rendering context
		if(!wglMakeCurrent(m_hDC, m_hRC))
			throw std::exception("Can't Activate The GL Rendering Context");
		
		// Initialize GLEW for extensions
		GLenum err = glewInit();
		if (err != GLEW_OK)
			throw std::exception((char*)glewGetErrorString(err));
		Console::print("Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

		// Check hardware support
		std::string err_str = "";
		bool bSimpleOk = true;
		if (!GL_VERSION_2_0)
			err_str = "WARNING: OpenGL 2.0 not supported. Try updating the graphics card drivers.";
		if (!glewIsSupported("GL_EXT_framebuffer_object"))
			err_str = "frame buffer objects not supported";
		if (err_str.length() > 0) {
			Console::print("ERROR: " + err_str + "\n");
			if (opt.bOwnDraw && bSimpleOk)
			{
				// inform the user
				Console::print("WARNING: cannot use layered windows! Falling back to standard window style\n");
				MessageBox(m_hWnd, "WARNING: cannot use layered windows! Falling back to standard window style", "Error",
					MB_OK|MB_ICONEXCLAMATION);

				// destroy the current window
				freeGLWindow();

				// create a new, simple window
				Options opt2 = opt;
				opt2.bOwnDraw = false;
				createGLWindow(left, top, width, height, title, opt2);
				return;
			}
			else if (!bSimpleOk) {
				Console::print("ERROR: OpenGL requirements not met. Cannot initialize BeGUI\n");
				MessageBox(m_hWnd, "ERROR: OpenGL requirements not met. Cannot initialize BeGUI", "Error",
					MB_OK|MB_ICONEXCLAMATION);
				freeGLWindow();
				throw std::exception("beGUI could not start");
			}
		}

		// Create a layered window?
		if (m_options.bOwnDraw)
		{
			// change the window style
			SetWindowLong(m_hWnd, GWL_EXSTYLE, GetWindowLong(m_hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);

			HDC dc = GetDC(0);
			m_hMemDC = CreateCompatibleDC(0);
			if (!m_hMemDC)
				throw std::exception("failed to create memory device context for layered window");
			std::vector<unsigned char> data(width*height*4, 0);
			HBITMAP hBMP = CreateBitmap(width,height, 1, 32, &data[0]);
			if (!hBMP)
				throw std::exception("failed to create DIB bitmap for layered window");
			HBITMAP bmpold = (HBITMAP)SelectObject(m_hMemDC, hBMP);
			SIZE sz = {width,height};
			POINT ptSrc = {0,0};
			BLENDFUNCTION blend = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
			
			if (!UpdateLayeredWindow(m_hWnd, dc, 0, &sz, m_hMemDC, &ptSrc, 0, &blend, ULW_ALPHA))
				throw std::exception("UpdateLayeredWindow failed");

			SelectObject(m_hMemDC, bmpold);
			DeleteObject(hBMP);
		
			// Create an offscreen rendering surface
			if (!m_frameRenderPass.setup(RenderPass::PIXEL_RGBA8, width, height, 0, false))
				throw std::exception("failed to create offscreen rendering pass");
			Console::print("created offscreen rendering surface (%d x %d)\n", width, height);
		}
		
		// after creating the OpenGL window, it's time to initialize all
		// subsystems, before creating our main application window
		initializeSubsystems();
		
		// set the size of the display area
		display::setSize(width, height);

		// Show the window
		ShowWindow(m_hWnd, SW_SHOW);
		
		SetForegroundWindow(m_hWnd);	// Slightly Higher Priority
		
		// Set keyboard focus to the window
		SetFocus(m_hWnd);
	}
	catch (std::exception e) {
		char str_msg[1024];
		sprintf(str_msg, "Failed to create main window, with error:\n%s", e.what());
		MessageBox(0, str_msg, "Error creating main window...", MB_OK);
		freeGLWindow();
		throw e;
	}
}

void FrameWindow_Win32::initializeSubsystems()
{
	// Get windows directory
	char win_dir[MAX_PATH+1];
	GetWindowsDirectory(win_dir, MAX_PATH);

	// Initialize font subsystem
	if (!FontManager::initialize())
		throw std::exception("failed to initialize font subsystem");
	if (!FontManager::setFont(strcat(win_dir, "\\Fonts\\tahoma.ttf"), 11))
		throw std::exception("failed to set default font");
	
	// Initialize the window manager and load resources
	ResourceManager::inst()->loadResources();
	
	// set some OpenGL states
	glClearColor(0, 0, 0, 0);
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	if (m_options.bOwnDraw)
		glBlendEquationSeparate(GL_FUNC_ADD, GL_MAX);
}

void FrameWindow_Win32::freeGLWindow()
{
	// if in full-screen mode, go back to windowed mode
	if (m_options.bFullScreen)
	{
		ChangeDisplaySettings(NULL,0);
		ShowCursor(TRUE);
	}

	// release rendering context
	if (m_hRC)
	{
		if (!wglMakeCurrent(NULL,NULL))
			Console::error("Release Of DC And RC Failed.\n");

		if (!wglDeleteContext(m_hRC))
			Console::error("Release Rendering Context Failed.\n");
		m_hRC = NULL;
	}

	// release dc
	if (m_hDC && !ReleaseDC(m_hWnd, m_hDC))
	{
		Console::error("Release Device Context Failed.\n");
		m_hDC = NULL;
	}

	// destroy window
	if (m_hWnd && !DestroyWindow(m_hWnd))
	{
		Console::error("Could Not Release hWnd.\n");
		m_hWnd = NULL;
	}

	// unregister window class
	if (!UnregisterClass("BeGUIFrameWindow", m_hInstance))
	{
		Console::error("Could Not Unregister Class.\n");
		m_hInstance = NULL;
	}
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// make sure that the frame window is the Win32 object, and call its
	// wndProc function
	FrameWindow_Win32 *pFrame = 0;
	try {
		pFrame = dynamic_cast<FrameWindow_Win32*>(FrameWindow::inst());
		ASSERT(pFrame);
	} catch (std::bad_cast &) {
		ASSERT(0);
	}
	return pFrame->wndProc(hWnd, uMsg, wParam, lParam);
}

LRESULT FrameWindow_Win32::wndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_ACTIVATE:
		{
			if (!HIWORD(wParam))
				onActivate();
			else
				onDeactivate();
			return 0;
		}

		case WM_SYSCOMMAND:
		{
			switch (wParam)
			{
				case SC_SCREENSAVE:					// Screensaver Trying To Start?
				case SC_MONITORPOWER:				// Monitor Trying To Enter Powersave?
				return 0;							// Prevent From Happening
			}
			break;
		}

		case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}

		case WM_PAINT:
		{
			if (!m_bSyncRendering) {
				frameRender();
				if (m_options.bOwnDraw)	// we are using an offscreen render buffer
					SwapBuffers(m_hDC);
			}
			break;// let DefWindowProc remove the WM_PAINT message from the queue
		}

		case WM_KEYDOWN:
		{
			int key = -1;
			switch (wParam) 
            { 
                case VK_LEFT: key = KEY_LEFT; break;
                case VK_RIGHT: key = KEY_RIGHT; break;
                case VK_UP: key = KEY_UP; break;
                case VK_DOWN: key = KEY_DOWN; break;
                case VK_HOME: key = KEY_HOME; break;
                case VK_END: key = KEY_END; break;
                case VK_INSERT: key = KEY_INSERT; break;
                case VK_DELETE: key = KEY_DELETE; break;
				case 33: key = KEY_PAGEUP; break;
				case 34: key = KEY_PAGEDOWN; break;
				case 16:	// BUG: cannot distringuish between left and right shift, the code is the same
					key = KEY_LSHIFT; break;
				case 17:	// BUG: cannot distringuish between left and right ctrl, the code is the same
					key = KEY_LCTRL; break;
			}
			if (key >= 0) {
				input::keyDown(key);
				onKeyDown(key);
			}
			return 0;
		}

		case WM_KEYUP:
		{
			int key = (int)wParam;
			switch (wParam) 
            { 
                case VK_LEFT: key = KEY_LEFT; break;
                case VK_RIGHT: key = KEY_RIGHT; break;
                case VK_UP: key = KEY_UP; break;
                case VK_DOWN: key = KEY_DOWN; break;
                case VK_HOME: key = KEY_HOME; break;
                case VK_END: key = KEY_END; break;
                case VK_INSERT: key = KEY_INSERT; break;
                case VK_DELETE: key = KEY_DELETE; break;
				case 33: key = KEY_PAGEUP; break;
				case 34: key = KEY_PAGEDOWN; break;
				case 16:	// BUG: cannot distringuish between left and right shift, the code is the same
					key = KEY_LSHIFT; break;
				case 17:	// BUG: cannot distringuish between left and right ctrl, the code is the same
					key = KEY_LCTRL; break;
			}
			if (key >= 0) {
				onKeyUp(key);
				input::keyUp(key);
			}
			return 0;
		}
		case WM_CHAR:
			{
				//input::keyDown(wParam);
				onKeyDown((int)wParam);
				break;
			}

		case WM_SIZE:								// Resize The OpenGL Window
		{
			setSize(LOWORD(lParam),HIWORD(lParam));  // LoWord=Width, HiWord=Height
			return 0;								// Jump Back
		}

		case WM_LBUTTONDOWN:
			{
				short int xPos = LOWORD(lParam); 
				short int yPos = HIWORD(lParam);
				input::mouseButtonDown(xPos, yPos, MOUSE_BUTTON_LEFT);
				onMouseDown(xPos, yPos, MOUSE_BUTTON_LEFT);
				
				// capture the mouse, for moving windows around etc
				SetCapture(m_hWnd);

				return 0;
			}
		case WM_MOUSEMOVE:
			{
				short int xPos = LOWORD(lParam); 
				short int yPos = HIWORD(lParam);
				Vector2i lastMousePos = input::lastMousePos();
				input::mousePos(xPos, yPos);
				onMouseMove(xPos, yPos, lastMousePos.x, lastMousePos.y);
				return 0;
			}
		case WM_LBUTTONUP:
			{
				short int xPos = LOWORD(lParam); 
				short int yPos = HIWORD(lParam);
				input::mouseButtonUp(xPos, yPos, MOUSE_BUTTON_LEFT);
				onMouseUp(xPos, yPos, MOUSE_BUTTON_LEFT);

				// release capture
				ReleaseCapture();

				return 0;
			}

		case WM_MOUSEWHEEL:
			{
			        int zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
				input::mouseWheel (zDelta);
				onMouseWheel (zDelta);
				return 0;
			}

	}

	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

void FrameWindow_Win32::frameRender()
{
	Rect<int> border = getInactiveBorders();

	int w = display::getWidth() + border.left + border.right;
	int h = display::getHeight() + border.top + border.bottom;

	static std::vector<unsigned char> data(w*h*4);
	static std::vector<unsigned char> data2(w*h*4);

	if (m_options.bOwnDraw) {
		// check if the render buffer has the right size. If not, update
		if (m_frameRenderPass.getWidth() != w || m_frameRenderPass.getHeight() != h) {
			m_frameRenderPass.setup(m_frameRenderPass.getPixelFormat(), w, h, 0, false);
			data.resize(w*h*4);
			data2.resize(w*h*4);
		}

		m_frameRenderPass.beginPass();
	}

	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// render the main window
	FrameWindow::frameRender();
	
	if (m_options.bOwnDraw) {
		glReadPixels(0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, &data[0]);

		m_frameRenderPass.endPass();

		// convert the data for windows
		// NOTE: this loop takes a lot of time, so it's a good candidate for optimization!
		for (int j=0; j<h; ++j) {
			int j2 = h-j-1;
			unsigned char *p1 = &data[0] + 4*j2*w;
			unsigned char *p2 = &data2[0] + 4*j*w;
			for (int i=0; i<w; ++i)
			{
				*p2 = *(p1+2);
				*(p2+1) = *(p1+1);
				*(p2+2) = *p1;
				*(p2+3) = *(p1+3);
				p1 += 4;
				p2 += 4;
			}
		}

		// copy the image data to the window
		HDC dc = GetDC(0);
		HBITMAP hBMP = CreateBitmap(w, h, 1, 32, &data2[0]);
		HBITMAP bmpold = (HBITMAP)SelectObject(m_hMemDC, hBMP);
		SIZE sz = {w, h};
		POINT ptSrc = {0,0};
		BLENDFUNCTION blend = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
		POINT screenPos = {getLeft()-border.left, getTop()-border.top};
		if (m_state == MAXIMIZED) {
			screenPos.x = -border.left;
			screenPos.y = -border.top;
		}
		
		if (!UpdateLayeredWindow(m_hWnd, dc, &screenPos, &sz, m_hMemDC, &ptSrc, 0, &blend, ULW_ALPHA))
			Console::error("UpdateLayeredWindow failed\n");
		
		SelectObject(m_hMemDC, bmpold);
		DeleteObject(hBMP);
		ReleaseDC(0, dc);

	}
	else
	{
		// display the new frame
		SwapBuffers(m_hDC);
	}
}

void FrameWindow_Win32::setPos(int x, int y)
{
	FrameWindow::setPos(x,y);
}

void FrameWindow_Win32::setSize(int w, int h)
{
	display::setSize(w,h);
	FrameWindow::setSize(w, h);
}


void FrameWindow_Win32::minimize()
{
	m_state = MINIMIZED;
	ShowWindow(m_hWnd, SW_MINIMIZE);
}

void FrameWindow_Win32::maximize()
{
	m_state = MAXIMIZED;
	ShowWindow(m_hWnd, SW_MAXIMIZE);
}

void FrameWindow_Win32::restore()
{
	m_state = VISIBLE;
	ShowWindow(m_hWnd, SW_RESTORE);
}

#endif