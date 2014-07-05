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

#ifndef _FRAMEWINDOW_H42631_INCLUDED_
#define _FRAMEWINDOW_H42631_INCLUDED_

#pragma once

#include "common.h"
#include "Container.h"
#include "Window.h"
#include "Menu.h"
#include "Dialog.h"
#include "Button.h"

namespace begui {

/**
 * FrameWindow: the frame window handles the main window of the application.
 *			This means that FrameWindow has to provide an interface with the OS,
 *			which handles application windows.
 *			FrameWindow can have 3 forms:
 *				-an own-drawn window, alpha blended on the desktop, when 
 *				supported by the OS. In this case, FrameWindow has to draw its own
 *				borders and caption, and handle all window events, like resizing and
 *				moving
 *				-a simple window, based on the windows provided by the OS. In this
 *				case, all window events (like resizing, moving, minimizing) are handled
 *				by the OS
 *				-a frame window stub, for applications were the frame window is owned
 *				and operated by other code, but the facilities of the FrameWindow are
 *				still desirable (example: having a full GUI inside the viewport of a 
 *				3D app)
 *
 */
class FrameWindow : public Window
{
public:
	struct Options {
		bool bOwnDraw;
		bool bFullScreen;
		int	nColorBits;
		int nDepthBits;
		int nStencilBits;
		std::string windowStyleName;
	};

public:
	virtual ~FrameWindow();

	// An instance of a FrameWindow is created only by calling createInstance()
	static FrameWindow* createInstance();
	static FrameWindow* inst()				{ return m_pInst; }

	virtual void create(int left, int top, int width, int height, const std::string &title, const Options *opt = 0,
						const std::string &style_name = "std_framewnd");
	virtual void free();
	virtual void setPos(int x, int y);
	virtual void setSize(int sx, int sy);

	bool	useSyncRendering() const		{ return m_bSyncRendering; }
	void	setSyncRendering(bool bSync)	{ m_bSyncRendering = bSync; }

	Options	getOptions() const	{ return m_options; }

	virtual void frameRender();
	virtual void resetViewport();

	virtual bool onMouseDown(int x, int y, int button);
	virtual bool onMouseMove(int x, int y, int prevx, int prevy);
	virtual bool onMouseUp(int x, int y, int button);
	virtual void onKeyDown(int key);
	virtual void onKeyUp(int key);
	virtual void onMouseWheel(int zDelta);
	
protected:
	FrameWindow();
	virtual void createGLWindow(int left, int top, int width, int height, const std::string &title, const Options &opt) = 0;
	virtual void freeGLWindow() = 0;
	
	// overridden methods
	virtual void onCaptionBtn(int id);
	virtual void calcClientArea();

protected:
	Options	m_options;
	bool	m_bSyncRendering;

	static FrameWindow *m_pInst;

private:
	// hide some Window methods
	virtual void create(int left, int top, int width, int height, const std::string &title) { ASSERT(0); };
};

};

#endif