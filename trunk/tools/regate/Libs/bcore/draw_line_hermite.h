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

#ifndef _DRAWLINEHERMITE_H4009_INCLUDED_
#define _DRAWLINEHERMITE_H4009_INCLUDED_

#pragma once

#include "common.h"
#include "interpolation.h"

/**
 * render_2d_line: a general templated function to rasterize 2d lines.
 *		P, I: must support the operators -(P&), /(scalar) and *(scalar)
 *		T: must have a method draw(P&)
 *			i1, i2: the endnode information to be interpolated
 *			t1, t2: the tangents of this information at the endnodes (used for hermite interpolation)
 */
template <class P, class I, class R>
void draw_line_hermite(const P &pt1, const P &pt2,
					   const I &i1, const I &i2, const I &t1, const I &t2,
					   R &renderer)
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
			// compute the new point
			P curP = pt1 + ptstep*(x - xstart);

			// compute the interpolated information for this point
			I curI = interp_hermite(i1, i2, t1, t2, xstart, xend, x);

			renderer.draw(curP, curI);
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
			
			// compute the interpolated information for this point
			I curI = interp_hermite(i1, i2, t1, t2, ystart, yend, y);

			renderer.draw(curP, curI);
		}
	}
}

#endif