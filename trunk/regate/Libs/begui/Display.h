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

#ifndef _DISPLAY_H42631_INCLUDED_
#define _DISPLAY_H42631_INCLUDED_
#pragma once

#include "common.h"

namespace begui {

namespace display
{
	// Mask the screen except this given rectangle. Any
	// rendering after this call will be restricted only within
	// this rectangle
	void pushMask(int x, int y, int w, int h, bool ignoreParent = false);

	// Remove any previous masks. Rendering can be done anywhere
	// on the screen after this call.
	void popMask();

	int getWidth();
	int getHeight();
	void setSize(int w, int h);

	// display reference frame (should be the rectangle corresponding to the
	// active rendering surface)
	void pushRefFrame(int x, int y, int w, int h);
	void popRefFrame();

	enum PrimitiveType {
		POINTS,
		LINES,
		TRIANGLES,
		QUADS
	};
	// rendering commands
	void clear();	// clears the screen
	void startBatch(PrimitiveType prim);
	void endBatch();
	void vertex(const Vector2& pos, const Vector2 *uv = 0, const Color *color = 0);
	void setColor(float r, float g, float b, float a);
	void setColor(const Color &cl, float a);
	void enableBlending(bool bEnable);
	void enableTexture(int texlevel, bool bEnable);
};

};

#endif