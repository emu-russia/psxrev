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

#include "util.h"

using namespace begui;

bool g_buttonDown[MOUSE_BUTTONS_NUM] = {false, false, false};
int g_mousePosX, g_mousePosY;
int g_mouseWheelDelta;
unsigned long g_lastClickTime[MOUSE_BUTTONS_NUM] = {0, 0, 0};
unsigned long g_prevClickTime[MOUSE_BUTTONS_NUM] = {0, 0, 0};
int g_lastMDownPosX[MOUSE_BUTTONS_NUM], g_lastMDownPosY[MOUSE_BUTTONS_NUM];
int g_prevMDownPosX[MOUSE_BUTTONS_NUM], g_prevMDownPosY[MOUSE_BUTTONS_NUM];
bool g_keyDown[288] = {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, \
false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, \
false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, \
false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, \
false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, \
false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, \
false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, \
false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, \
false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, \
false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, \
false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, \
false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, \
false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, \
false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, \
false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, \
false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, \
false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, \
false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false};

unsigned int g_mouseClickRepeatInterv = 30;	// msec
unsigned int g_keybRepeatInterv = 30;		// msec
unsigned long g_lastKeyDownTime = 0;

bool input::isMouseButtonDown(int mouseButton)
{
	ASSERT(mouseButton >= 0 && mouseButton < MOUSE_BUTTONS_NUM);
	return g_buttonDown[mouseButton];
}

bool input::isKeyDown(int key)
{
	return g_keyDown[key];
}

void input::mouseButtonDown(int x, int y, int mouseButton)
{
	ASSERT(mouseButton >= 0 && mouseButton < MOUSE_BUTTONS_NUM);
	g_buttonDown[mouseButton] = true;
	g_prevClickTime[mouseButton] = g_lastClickTime[mouseButton];
	g_lastClickTime[mouseButton] = system::current_time();
	
	g_mousePosX = x;
	g_mousePosY = y;
	g_prevMDownPosX[mouseButton] = g_lastMDownPosX[mouseButton];
	g_lastMDownPosX[mouseButton] = x;
	g_prevMDownPosY[mouseButton] = g_lastMDownPosY[mouseButton];
	g_lastMDownPosY[mouseButton] = y;
}

void input::mouseButtonUp(int x, int y, int mouseButton)
{
	ASSERT(mouseButton >= 0 && mouseButton < MOUSE_BUTTONS_NUM);
	g_buttonDown[mouseButton] = false;
	
	g_mousePosX = x;
	g_mousePosY = y;
}

void input::mousePos(int x, int y)
{
	g_mousePosX = x;
	g_mousePosY = y;
}

bool input::isDoubleClick(int mouseButton)
{
	ASSERT(mouseButton >= 0 && mouseButton < MOUSE_BUTTONS_NUM);

	unsigned long last_click_time = g_lastClickTime[mouseButton];

	// check that the checked button was the last one to be clicked
	for (int i=0; i<MOUSE_BUTTONS_NUM; ++i)
		if (g_lastClickTime[i] > last_click_time)
			return false;

	// check if the mouse pointer moved significantly
	int dbg1 = abs(g_lastMDownPosX[mouseButton] - g_prevMDownPosX[mouseButton]);
	int dbg2 = abs(g_lastMDownPosY[mouseButton] - g_prevMDownPosY[mouseButton]);
	int a = g_lastMDownPosX[mouseButton];
	int b = g_prevMDownPosX[mouseButton];
	int c = g_lastMDownPosY[mouseButton];
	int d = g_prevMDownPosY[mouseButton];
	if (abs(g_lastMDownPosX[mouseButton] - g_prevMDownPosX[mouseButton]) > DOUBLE_CLICK_ACC_RADIUS)
		return false;
	if (abs(g_lastMDownPosY[mouseButton] - g_prevMDownPosY[mouseButton]) > DOUBLE_CLICK_ACC_RADIUS)
		return false;

	// check that the double click was quick enough
	if (g_lastClickTime[mouseButton] - g_prevClickTime[mouseButton] <= DOUBLE_CLICK_TIME && 
		g_lastClickTime[mouseButton] - g_prevClickTime[mouseButton] > 0)	// last check to avoid VERY improbable wrap around effect
		return true;
	return false;
}

void input::keyDown(int key)
{
	g_keyDown[key] = true;
}

void input::keyUp(int key)
{
	g_keyDown[key] = false;
}

Vector2i input::lastMousePos()
{
	return Vector2i(g_mousePosX, g_mousePosY);
}

void input::setMouseClickRepeatInterv(unsigned int msec)
{
	g_mouseClickRepeatInterv = msec;
}

void input::setKeybRepeatInterv(unsigned int msec)
{
	g_keybRepeatInterv = msec;
}

unsigned int input::getMouseClickRepeatInterv()
{
	return g_mouseClickRepeatInterv;
}

unsigned int input::getKeybRepeatInterv()
{
	return g_keybRepeatInterv;
}

void input::mouseWheel (int zDelta)
{
	g_mouseWheelDelta = zDelta;
}