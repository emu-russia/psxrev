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

#ifndef _DRAWTRIANGLE_H45631_INCLUDED_
#define _DRAWTRIANGLE_H45631_INCLUDED_

#pragma once

#include "common.h"
#include "drawline.h"

template <class P, class R>
void draw_triangle(P p1, P p2, P p3, R &renderer)
{
	p1.x = (int)p1.x;
	p1.y = (int)p1.y;
	p2.x = (int)p2.x;
	p2.y = (int)p2.y;
	p3.x = (int)p3.x;
	p3.y = (int)p3.y;

	// sort points in descending y order
	if (p1.y > p2.y) std::swap(p1, p2);
	if (p2.y > p3.y) {
		if (p1.y > p3.y) {
			std::swap(p1, p3);
			std::swap(p2, p3);
		}
		else std::swap(p2, p3);
	}

	// degenerate cases:
	if (p1.y == p3.y) {
		// triangle is a line - draw it
		render_2d_line(p1, p2, renderer);
		render_2d_line(p2, p3, renderer);
		return;
	}

	// run a scanline
	int s;
	for (s=p1.y; s<p2.y; ++s)
	{
		double t1 = (s-(int)p1.y)/(p3.y-p1.y);
		P v1 = (1-t1)*p1 + t1*p3;
		v1.y = s;	// correct rounding errors...

		double t2 = (s-(int)p1.y)/(p2.y-p1.y);
		P v2 = (1-t2)*p1 + t2*p2;
		v2.y = s;

		render_2d_line(v1, v2, renderer);
	}
	if (p2.y == p3.y) {
		// degenerate - draw a line and return
		render_2d_line(p2, p3, renderer);
		return;
	}
	for (s=p2.y; s<=p3.y; ++s)
	{
		double t1 = (s-(int)p1.y)/(p3.y-p1.y);
		P v1 = (1-t1)*p1 + t1*p3;
		v1.y = s;

		double t2 = (s-(int)p2.y)/(p3.y-p2.y);
		P v2 = (1-t2)*p2 + t2*p3;
		v2.y = s;

		render_2d_line(v1, v2, renderer);
	}
}