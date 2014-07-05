/* 
// Copyright 2008 Alexandros Panagopoulos
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

#ifndef _INPUT_H42631_INCLUDED_
#define _INPUT_H42631_INCLUDED_
#pragma once

#include "../bcore/Vector2i.h"

namespace begui {
	
#define MOUSE_BUTTON_LEFT 0
#define MOUSE_BUTTON_MIDDLE 1
#define MOUSE_BUTTON_RIGHT 2
#define MOUSE_BUTTONS_NUM 3
#define DOUBLE_CLICK_TIME 300	// in msec
#define DOUBLE_CLICK_ACC_RADIUS 2	// the max radius, in pixels, of the second click in a double click sequence, so that
									// the click sequence is still considered a double click

#define KEY_LEFT 256
#define KEY_RIGHT 257
#define KEY_UP 258
#define KEY_DOWN 259
#define KEY_HOME 260
#define KEY_END 261
#define KEY_INSERT 262
#define KEY_DELETE 263
#define KEY_PAGEUP 264
#define KEY_PAGEDOWN 265
#define KEY_LSHIFT 266
#define KEY_RSHIFT 267
#define KEY_LCTRL 268
#define KEY_RCTRL 269
#define KEY_LALT 270
#define KEY_RALT 271
#define KEY_ENTER 13

namespace input
{
	void	mouseButtonDown(int x, int y, int mouseButton);
	void	mouseButtonUp(int x, int y, int mouseButton);
	void	mousePos(int x, int y);
	bool	isMouseButtonDown(int mouseButton);
	bool	isDoubleClick(int mouseButton);
	Vector2i lastMousePos();
	
	void keyDown(int key);
	void keyUp(int key);
	bool isKeyDown(int key);

	void setMouseClickRepeatInterv(unsigned int msec);
	void setKeybRepeatInterv(unsigned int msec);
	unsigned int getMouseClickRepeatInterv();
	unsigned int getKeybRepeatInterv();

	void	mouseWheel (int zDelta);

};

};

#endif