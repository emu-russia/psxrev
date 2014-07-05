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
#include "FrameWindow.h"

using namespace begui;

// display dimensions
int	g_displayWidth = 0;
int g_displayHeight = 0;

// a stack of masks for the scissor test
std::vector<Rect<int> > g_maskStack;

// a stack of reference frames for the active rendering surface
std::vector<Rect<int> > g_refFrameStack;

int display::getWidth()
{
	return g_displayWidth;
}

int display::getHeight()
{
	return g_displayHeight;
}

void display::setSize(int w, int h)
{
	ASSERT(w >= 0);
	ASSERT(h >= 0);

	g_displayWidth = w;
	g_displayHeight = h;
}

// Mask the screen except this given rectangle. Any
// rendering after this call will be restricted only within
// this rectangle
void display::pushMask(int x, int y, int w, int h, bool ignoreParent)
{
	Rect<int> rect(x, g_displayHeight-y-h+1, x+w, g_displayHeight-y+1);
	if (g_refFrameStack.size() > 0) {
		// use the currently set reference frame
		Rect<int> &ref = g_refFrameStack.back();
		x-=ref.left;
		y-=ref.top;
		rect = Rect<int>(x, ref.getHeight()-y-h+1, x+w, ref.getHeight()-y+1);
	}
	if (FrameWindow::inst()) {
		Rect<int> fb = FrameWindow::inst()->getInactiveBorders();

		int fw = FrameWindow::inst()->getRight()-FrameWindow::inst()->getLeft() + (fb.left+fb.right);
		int fh = FrameWindow::inst()->getBottom()-FrameWindow::inst()->getTop() + (fb.top+fb.bottom);

		if (FrameWindow::inst()->getOptions().bOwnDraw) {
			x -= FrameWindow::inst()->getLeft();
			y -= FrameWindow::inst()->getTop();
		}

		rect = Rect<int>(x, fh-y-h+1, x+w, fh-y+1);
	}

	//crop the new mask given the previous mask in the stack
	if (g_maskStack.size() > 0 && !ignoreParent) {
		Rect<int> &pr = g_maskStack.back();
		if (rect.left < pr.left) rect.left = pr.left;
		if (rect.top < pr.top) rect.top = pr.top;
		if (rect.right > pr.right) rect.right = pr.right;
		if (rect.bottom > pr.bottom) rect.bottom = pr.bottom;
	}

	// set the new mask
	g_maskStack.push_back(rect);
	glScissor(rect.left, rect.top, rect.getWidth(), rect.getHeight());
	glEnable(GL_SCISSOR_TEST);
}

// Remove any previous masks. Rendering can be done anywhere
// on the screen after this call.
void display::popMask()
{
	if (g_maskStack.size() > 0)
		g_maskStack.pop_back();

	if (g_maskStack.size() == 0)
		glDisable(GL_SCISSOR_TEST);
	else {
		// load the previous mask
		Rect<int> &rect = g_maskStack.back();	
		glScissor(rect.left, rect.top, rect.getWidth(), rect.getHeight());
		glEnable(GL_SCISSOR_TEST);
	}
}

void display::pushRefFrame(int x, int y, int w, int h)
{
	g_refFrameStack.push_back(Rect<int>(x,y,x+w,y+h));
}

void display::popRefFrame()
{
	g_refFrameStack.pop_back();
}