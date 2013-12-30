/* 
// Copyright 2007 Alexandros Panagopoulos
//
// This software is distributed under the terms of the GNU Lesser General Public Licence
//
// This file is part of BeGUI library.
//
//    BeGUI is free software: you can redistribute it and/or modify
//    it under the terms of the GNU Lesser General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    BeGUI is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU Lesser General Public License for more details.
//
//    You should have received a copy of the GNU Lesser General Public License
//    along with BeGUI.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "FrameWindow.h"
#include "ResourceManager.h"
#include "Font.h"
#include "util.h"
#ifdef _WIN32
	#include "FrameWindow_Win32.h"
#endif

using namespace begui;

// static singleton instance
FrameWindow *FrameWindow::m_pInst = 0;

int g_lastXPos = 0;
int g_lastYPos = 0;

FrameWindow::FrameWindow()
{
	// default style for a frame window
	setStyle(Window::MULTIPLE);
}

FrameWindow::~FrameWindow()
{
	free();
}

FrameWindow* FrameWindow::createInstance()
{
	if (m_pInst)
		return m_pInst;
#ifdef _WIN32
	m_pInst = new FrameWindow_Win32;
#endif
	ASSERT(m_pInst);
	return m_pInst;
}

void FrameWindow::create(int left, int top, int width, int height, const std::string &title, const Options *opt,
						 const std::string &style_name)
{
	// set the various options for the created window
	if (opt) {
		m_options = *opt;
	}
	else {
		m_options.bOwnDraw = true;
		m_options.bFullScreen = false;
		m_options.nColorBits = 16;
		m_options.nDepthBits = 16;
		m_options.nStencilBits = 0;
	}

	// create the OpenGL window
	try {
		createGLWindow(left, top, width, height, title, m_options);
	}
	catch (std::exception e) {
		Console::error("Failed to create OpenGL window: " + (std::string)e.what() + "\n");
		throw e;
	}

	// only if the window is own-drawn does it have borders and caption
	m_bHasBorders = m_bHasCaption = m_options.bOwnDraw;
	m_bMovable = m_options.bOwnDraw;

	m_bCanMinimize = true;
	m_bCanMaximize = true;

	Window::create(left, top, width, height, title, style_name);
}

void FrameWindow::free()
{
	// release the OpenGL window
	freeGLWindow();
}

void FrameWindow::frameRender()
{
	resetViewport();

	Window::frameRender();
}

void FrameWindow::setPos(int x, int y)
{
	Window::setPos(x,y);
}

void FrameWindow::setSize(int w, int h)
{
	Window::setSize(w,h);
}

void FrameWindow::resetViewport()
{
	if (m_options.bOwnDraw) {
		Rect<int> border = getInactiveBorders();

		// Setup and orthogonal, pixel-to-pixel projection matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(m_left, m_right+border.left+border.right, m_bottom+(border.top+border.bottom), m_top, 0.0, 1.0);

		// Setup the viewport
		glViewport(0,0,getWidth()+border.left+border.right, getHeight()+border.top+border.bottom);
	}
	else {
		// Setup and orthogonal, pixel-to-pixel projection matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(m_left, m_right, m_bottom, m_top, 0.0, 1.0);

		// Setup the viewport
		glViewport(m_left, -m_top, getWidth(), getHeight());
	}
}

bool FrameWindow::onMouseDown(int x, int y, int button)
{
	if (m_options.bOwnDraw) {
		x += m_left;
		y += m_top;
		g_lastXPos = x;
		g_lastYPos = y;
	}
	return Window::onMouseDown(x,y,button);
}

bool FrameWindow::onMouseMove(int x, int y, int prevx, int prevy)
{
	if (m_options.bOwnDraw) {
		x += m_left;
		y += m_top;
		prevx = g_lastXPos;
		prevy = g_lastYPos;
		g_lastXPos = x;
		g_lastYPos = y;
	}
	return Window::onMouseMove(x,y,prevx,prevy);
}

bool FrameWindow::onMouseUp(int x, int y, int button)
{
	if (m_options.bOwnDraw) {
		x += m_left;
		y += m_top;
	}
	return Window::onMouseUp(x,y,button);
}

void FrameWindow::onKeyDown(int key)
{
	Window::onKeyDown(key);
}

void FrameWindow::onKeyUp(int key)
{
	Window::onKeyUp(key);
}

void FrameWindow::onMouseWheel(int zDelta)
{
	Window::onMouseWheel (zDelta);
}

void FrameWindow::calcClientArea()
{
	if (m_options.bOwnDraw) {
		Window::calcClientArea();
	}
	else {
		// ISSUE: doesnt take into account the OS's borders, which should be subtracted from the 
		// width and height. Doesnt matter know, might need to be fixed later though.
		m_clientArea = Rect<int>(0,0,getWidth(),getHeight());
	}
}


void FrameWindow::onCaptionBtn(int id)
{
	switch (id) {
		case 101:
			// close btn
			//m_onClose(CLOSE);
			exit(0);
			break;
		default:
			Window::onCaptionBtn(id);
	}
}
