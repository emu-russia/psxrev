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

#include "BaseApp_Win.h"
#include "util.h"
#include "FrameWindow.h"
#include "Font.h"
#include "ResourceManager.h"

using namespace begui;

BaseApp_Win	*BaseApp_Win::m_pInst = NULL;


BaseApp_Win::BaseApp_Win()
{
}

BaseApp_Win::~BaseApp_Win()
{
}

bool BaseApp_Win::initialize(const std::string &title, size_t width, size_t height, Window::Style frame_style,
							 FrameWindow::Options *pOpt)
{
	// set the desired options of the new window
	FrameWindow::Options opt;
	if (pOpt)
		opt = *pOpt;
	else {
		opt.bOwnDraw = true;
		opt.bFullScreen = false;
		opt.nColorBits = 16;
		opt.nDepthBits = 16;
		opt.nStencilBits = 0;
	}

	// Create Our OpenGL Window
	FrameWindow::createInstance();
	std::string wnd_style = "std_framewnd";
	if (opt.windowStyleName.length() > 0)
		wnd_style = opt.windowStyleName;
	try {
		FrameWindow::inst()->create(0, 0, (int)width, (int)height, title, &opt, wnd_style);
	}
	catch (std::exception &e) {
		Console::error("Failed to create main window: %s\n", e.what());
		return false;
	}

	// use synchronous rendering?
	FrameWindow::inst()->setSyncRendering(false);

	// do any extra initialization here
	if (!onCreate())
		return false;

	// update and render the frame once after creating the window
	updateFrame();
	renderFrame();

	return true;
}

void BaseApp_Win::updateFrame()
{
	// update all timeseries objects with the current time
	Updater::inst()->update_all_current_time();

	// update the main window
	FrameWindow::inst()->frameUpdate();
}

void BaseApp_Win::renderFrame()
{
	FrameWindow::inst()->frameRender();
}

int BaseApp_Win::run()
{
	MSG		msg;									// Windows Message Structure
	BOOL	done=FALSE;								// Bool Variable To Exit Loop

	while(!done)									// Loop That Runs While done=FALSE
	{
		// if we are not using synchronous rendering, wait for messages
		if (!FrameWindow::inst()->useSyncRendering())
			WaitMessage();

		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))	// Is There A Message Waiting?
		{
			if (msg.message == 0)
			{
				// Draw The Scene.  Watch for Quit Messages From DrawGLScene()
				if (FrameWindow::inst()->isActive())				// Program Active?
				{
					updateFrame();
					if (FrameWindow::inst()->useSyncRendering())
						renderFrame();
				}
			}
			if (msg.message==WM_QUIT)				// Have We Received A Quit Message?
			{
				done=TRUE;							// If So done=TRUE
			}
			else									// If Not, Deal With Window Messages
			{
				TranslateMessage(&msg);				// Translate The Message
				DispatchMessage(&msg);				// Dispatch The Message
			}

			// update and render the frame after a message is processed
			updateFrame();
			renderFrame();
		}
		else										// If There Are No Messages
		{
			// idle-time rendering. This will be reached only if 
			// synchronous rendering is used
			onIdle();

			if (FrameWindow::inst()->isActive())
			{
				updateFrame();
				if (FrameWindow::inst()->useSyncRendering())
					renderFrame();
			}
		}
	}

	// Shutdown
	FrameWindow::inst()->free();

	return (msg.wParam);
}