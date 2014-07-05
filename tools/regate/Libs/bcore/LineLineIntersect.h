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

#ifndef _LINELINEINTERSECT_H45631_INCLUDED_
#define _LINELINEINTERSECT_H45631_INCLUDED_

#pragma once

__forceinline bool line_line_intersect(const Vector2 &pt00, const Vector2 &pt01,
									   const Vector2 &pt10, const Vector2 &pt11, 
									   Vector2 *inters_pt)
{
	// WARNING: SHITTY CODE! CHANGE ME! WARNING: SHITTY CODE! CHANGE ME! WARNING: SHITTY CODE! CHANGE ME! 
	// WARNING: SHITTY CODE! CHANGE ME! WARNING: SHITTY CODE! CHANGE ME! WARNING: SHITTY CODE! CHANGE ME! 
	// WARNING: SHITTY CODE! CHANGE ME! WARNING: SHITTY CODE! CHANGE ME! WARNING: SHITTY CODE! CHANGE ME! 
	// WARNING: SHITTY CODE! CHANGE ME! WARNING: SHITTY CODE! CHANGE ME! WARNING: SHITTY CODE! CHANGE ME! 
	// WARNING: SHITTY CODE! CHANGE ME! WARNING: SHITTY CODE! CHANGE ME! WARNING: SHITTY CODE! CHANGE ME! 
	// WARNING: SHITTY CODE! CHANGE ME! WARNING: SHITTY CODE! CHANGE ME! WARNING: SHITTY CODE! CHANGE ME! 
	// WARNING: SHITTY CODE! CHANGE ME! WARNING: SHITTY CODE! CHANGE ME! WARNING: SHITTY CODE! CHANGE ME! 
	float x0 = pt00.x;
	float y0 = pt00.y;
	float x1 = pt01.x;
	float y1 = pt01.y;
	float x2 = pt10.x;
	float y2 = pt10.y;
	float x3 = pt11.x;
	float y3 = pt11.y;

	float a1,b1,c1, // constants of linear equations
		a2,b2,c2,
		det_inv,  // the inverse of the determinant of the coefficient matrix
		m1,m2;    // the slopes of each line

	// compute slopes, note the cludge for infinity, however, this will
	// be close enough

	if ((x1-x0)!=0)
		m1 = (y1-y0)/(x1-x0);
	else
		m1 = (float)1e+10;   // close enough to infinity

	if ((x3-x2)!=0)
		m2 = (y3-y2)/(x3-x2);
	else
		m2 = (float)1e+10;   // close enough to infinity

	// compute constants
	a1 = m1;
	a2 = m2;
	b1 = -1;
	b2 = -1;
	c1 = (y0-m1*x0);
	c2 = (y2-m2*x2);

	// compute the inverse of the determinate
	float f = a1*b2 - a2*b1;
	if (fabs(f) <= 0.00001)
		return false;
	det_inv = 1/f;

	// use Kramers rule to compute xi and yi
	inters_pt->x =((b1*c2 - b2*c1)*det_inv);
	inters_pt->y =((a2*c1 - a1*c2)*det_inv);

	return true;
}

#endif