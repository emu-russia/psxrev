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

#ifndef _DRAWTRIANGLEHERMITE_H4658_INCLUDED_
#define _DRAWTRIANGLEHERMITE_H4658_INCLUDED_

#pragma once

#include "common.h"
#include "draw_line_hermite.h"

/**
 *	p1,p2,p2: The 2d points defining the triangle
 *	i1,i2,i3: Information at each triangle corner that has to be interpolated and passed on to the renderer
 *	t1,t2,t3: Tangents of the information at i1,i2,i3
 *
 */
template <class P, class I, class R>
void draw_triangle_hermite(P p1, P p2, P p3,
						   I i1, I i2, I i3,
						   I t1, I t2, I t3,
						   R &renderer)
{
	p1.x = (int)p1.x;
	p1.y = (int)p1.y;
	p2.x = (int)p2.x;
	p2.y = (int)p2.y;
	p3.x = (int)p3.x;
	p3.y = (int)p3.y;

	// sort points in descending y order
	if (p1.y > p2.y) {
		std::swap(p1, p2); std::swap(i1, i2); std::swap(t1, t2);
	}
	if (p2.y > p3.y) {
		if (p1.y > p3.y) {
			std::swap(p1, p3); std::swap(p2, p3);
			std::swap(i1, i3); std::swap(i2, i3);
			std::swap(t1, t3); std::swap(t2, t3);
		}
		else { std::swap(p2, p3); std::swap(i2, i3); std::swap(t2, t3); }
	}

	// degenerate cases:
	if (p1.y == p3.y) {
		// triangle is a line - draw it
		draw_line_hermite(p1, p2, i1, i2, t1, t2, renderer);
		draw_line_hermite(p2, p3, i2, i3, t2, t3, renderer);
		return;
	}

	// run a scanline
	int s;
	for (s=p1.y; s<p2.y; ++s)
	{
		// compute the two endpoints for this scanline
		double f1 = (s-p1.y)/(p3.y-p1.y);
		P v1 = (1-f1)*p1 + f1*p3;
		v1.y = s;	// correct rounding errors...

		double f2 = (s-p1.y)/(p2.y-p1.y);
		P v2 = (1-f2)*p1 + f2*p2;
		v2.y = s;

		// compute the hermite-interpolated information for this scanline endpoints
		I iv1 = interp_hermite(i1, i3, t1, t3, p1.y, p3.y, (float)s);
		I iv2 = interp_hermite(i1, i2, t1, t2, p1.y, p2.y, (float)s);

		// interpolate the tangents linearly... (is this a problem??)
		I tv1 = (1-f1)*t1 + f1*t3;
		I tv2 = (1-f2)*t1 + f2*t2;

		// draw the scanline
		draw_line_hermite(v1, v2, iv1, iv2, tv1, tv2, renderer);
	}
	if (p2.y == p3.y) {
		// degenerate case - draw a line and return
		draw_line_hermite(p2, p3, i2, i3, t2, t3, renderer);
		return;
	}
	for (s=p2.y; s<=p3.y; ++s)
	{
		double f1 = (s-p1.y)/(p3.y-p1.y);
		P v1 = (1-f1)*p1 + f1*p3;
		v1.y = s;

		double f2 = (s-p2.y)/(p3.y-p2.y);
		P v2 = (1-f2)*p2 + f2*p3;
		v2.y = s;

		// compute the hermite-interpolated information for this scanline endpoints
		I iv1 = interp_hermite(i1, i3, t1, t3, p1.y, p3.y, (float)s);
		I iv2 = interp_hermite(i2, i3, t2, t3, p2.y, p3.y, (float)s);

		// interpolate the tangents linearly... (is this a problem??)
		I tv1 = (1-f1)*t1 + f1*t3;
		I tv2 = (1-f2)*t2 + f2*t3;

		// draw the scanline
		draw_line_hermite(v1, v2, iv1, iv2, tv1, tv2, renderer);
	}
}

#endif