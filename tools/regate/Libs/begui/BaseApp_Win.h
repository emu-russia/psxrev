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

#ifndef _BASEAPP_WIN_H42631_INCLUDED_
#define _BASEAPP_WIN_H42631_INCLUDED_

#pragma once

#include "common.h"
#include "../bcore/RenderPass.h"
#include "FrameWindow.h"

namespace begui {

class BaseApp_Win
{
private:
	static BaseApp_Win	*m_pInst;

public:
	virtual bool initialize(const std::string &title, size_t width, size_t height, 
						Window::Style frame_style=Window::MULTIPLE,
						FrameWindow::Options *opt=0);
	virtual int run();
	
	void setSyncRendering(bool bSyncRendering)	{ FrameWindow::inst()->setSyncRendering(bSyncRendering); }

	static BaseApp_Win* inst()	{ if (!m_pInst) m_pInst = new BaseApp_Win(); return m_pInst; }

	// overridables
	virtual bool onCreate() { return true; }
	virtual void onIdle()	{ }

protected:
	BaseApp_Win();
	virtual ~BaseApp_Win();
	
	virtual void updateFrame();
	virtual void renderFrame();
};

};

#endif