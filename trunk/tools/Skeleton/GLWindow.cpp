#include "GLWindow.h"
#include <gl/GL.h>


GLWindow::GLWindow(HWND hWindowHandle, UINT uiPixelWidth, UINT uiPixelHeight)
{
	m_hWnd = hWindowHandle;
	m_dbWidth = static_cast<double>(uiPixelWidth);
	m_dbHeight = static_cast<double>(uiPixelHeight);
}

void GLWindow::Initialize() throw(int)
{
	m_hDC = GetDC(m_hWnd);
	if (m_hDC == NULL) throw(1);

	PIXELFORMATDESCRIPTOR Pfd;
	ZeroMemory(&Pfd, sizeof(Pfd));
	Pfd.nSize = sizeof(Pfd);
	Pfd.nVersion = 1;
	Pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	Pfd.iPixelType = PFD_TYPE_RGBA;
	Pfd.cColorBits = 24;
	Pfd.cDepthBits = 16;
	Pfd.iLayerType = PFD_MAIN_PLANE;

	int iFormat = ChoosePixelFormat(m_hDC, &Pfd);
	if (iFormat == 0) throw(2);

	if (SetPixelFormat(m_hDC, iFormat, &Pfd) == FALSE) throw(3);

	m_hGLRC = wglCreateContext(m_hDC);
	if (m_hGLRC == NULL) throw(4);

	if (wglMakeCurrent(m_hDC, m_hGLRC) == FALSE) throw(5);

	////////////////

	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glPushMatrix();
		glLoadIdentity();
		glScaled(2.0 / m_dbWidth, 2.0 / m_dbHeight, 1.0);
		glTranslated(-m_dbWidth / 2.0, -m_dbHeight / 2.0, 0.0);
		glBegin(GL_POINTS);
}

void GLWindow::Destroy() throw(int)
{
		glEnd();
	glPopMatrix();
	glFlush();

	if (wglMakeCurrent(m_hDC, NULL) == FALSE) throw(1);
	if (wglDeleteContext(m_hGLRC) == FALSE) throw(2);
}

void GLWindow::SetColor(GLubyte ubRed, GLubyte ubGreen, GLubyte ubBlue) const
{
	glColor3ub(ubRed, ubGreen, ubBlue);
}

void GLWindow::PutPixel(UINT x, UINT y) const
{
	glVertex3i(static_cast<GLint>(x), static_cast<GLint>(y), 0);
}

void GLWindow::ClearScreen() const
{
	glEnd();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f );
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBegin(GL_POINTS);
}

void GLWindow::SwapBuffers() const throw(int)
{
	glEnd();
	if (::SwapBuffers(m_hDC) == FALSE) throw(1);
	glBegin(GL_POINTS);
}