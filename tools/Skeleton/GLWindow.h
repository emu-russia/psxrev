#ifndef GLWINDOW_H
#define GLWINDOW_H

#include <Windows.h>
#include <gl/GL.h>

class GLWindow
{
	public:
		GLWindow(HWND hWindowHandle, UINT uiPixelWidth, UINT uiPixelHeight);
		~GLWindow(){};

		// Thrown exception values and their meanings:
		// 1: Failed: GetDC()
		// 2: Failed: ChoosePixelFormat()
		// 3: Failed: SetPixelFormat()
		// 4: Failed: wglCreateContext()
		// 5: Failed: wglMakeCurrent()
		void Initialize() throw(int);

		// Thrown exception values and their meanings:
		// 1: Failed: wglMakeCurrent()
		// 2: Failed: wglDeleteContext()
		void Destroy() throw(int);

		void SetColor(GLubyte ubRed, GLubyte ubGreen, GLubyte ubBlue) const;

		void PutPixel(UINT x, UINT y) const;

		void ClearScreen() const;

		// Thrown exception values and their meanings:
		// 1: Failed: SwapBuffers()
		void SwapBuffers() const throw(int);

	protected:
		double	m_dbWidth;
		double	m_dbHeight;
		HWND	m_hWnd;
		HDC	m_hDC;
		HGLRC	m_hGLRC;
};

#endif