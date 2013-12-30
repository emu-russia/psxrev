/* 
// Copyright 2007 Alexandros Panagopoulos
//
// This software is distributed under the terms of the GNU Lesser General Public Licence
//
// This file is part of Be3D library.
//
//    Be3D is free software: you can redistribute it and/or modify
//    it under the terms of the GNU Lesser General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    Be3D is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU Lesser General Public License for more details.
//
//    You should have received a copy of the GNU Lesser General Public License
//    along with Be3D.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef _DRAWLINE_H4369_INCLUDED_
#define _DRAWLINE_H4369_INCLUDED_

#pragma once

#include "common.h"

/**
 * render_2d_line: a general templated function to rasterize 2d lines.
 *		P: must support the operators -(P&), /(scalar) and *(scalar)
 *		T: must have a method draw(P&)
 *
 */
template <class P, class T>
void render_2d_line(const P &pt1, const P &pt2, T &renderer)
{
	P dp = pt2 - pt1;

	if (abs(dp.x) > abs(dp.y))
	{
		P ptstep = dp/dp.x;

		int xend = (int)pt2.x;
		int xstart = (int)pt1.x;
		int xstep = (xend > xstart) ? 1 : -1;
		xend+=xstep;	// xend indicates the pixel after the last pixel of the line
		for (int x=xstart; x!=xend; x+=xstep)
		{
			P curP = pt1 + ptstep*(x - xstart);
			renderer.draw(curP);
		}
	}
	else
	{
		P ptstep = dp/dp.y;

		int yend = (int)pt2.y;
		int ystart = (int)pt1.y;
		int ystep = (yend > ystart) ? 1 : -1;
		yend+=ystep;
		for (int y=ystart; y!=yend; y+=ystep)
		{
			P curP = pt1 + ptstep*(y - ystart);
			renderer.draw(curP);
		}
	}
}

#endif