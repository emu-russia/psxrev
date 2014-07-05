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

#ifndef _FRAMEWINDOW_WIN32_H42631_INCLUDED_
#define _FRAMEWINDOW_WIN32_H42631_INCLUDED_

#pragma once

#ifdef _WIN32			// this is a Windows-only class!

#include "Window.h"
#include "FrameWindow.h"
#include "../bcore/RenderPass.h"

//#define NOMINMAX	// turn off the generation of min/max macros, so as not to clash with std::min/std::max
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

namespace begui {

/**
 * FrameWindow_Win32: 
 *
 */
class FrameWindow_Win32 : public FrameWindow
{
	friend class FrameWindow;

private:
	HINSTANCE	m_hInstance;
	HWND		m_hWnd;
	HDC			m_hDC;
	HGLRC		m_hRC;
	HDC			m_hMemDC;

	RenderPass m_frameRenderPass;

public:
	virtual ~FrameWindow_Win32();

	virtual void frameRender();
	virtual void setPos(int x, int y);
	virtual void setSize(int w, int h);
	virtual void minimize();
	virtual void maximize();
	virtual void restore();
	
	LRESULT wndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	HWND	getHWnd() const			{ return m_hWnd; }
	HINSTANCE getHInstance() const	{ return m_hInstance; }

protected:
	FrameWindow_Win32();

	virtual void createGLWindow(int left, int top, int width, int height, const std::string &title, const Options &opt);
	virtual void initializeSubsystems();
	virtual void freeGLWindow();
};

};

#endif

#endif