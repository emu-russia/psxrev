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

#ifndef _INTERPOLATION_H45631_INCLUDED_
#define _INTERPOLATION_H45631_INCLUDED_

#pragma once

#include "common.h"

template <class I, class T>
I interp_hermite(const I &i1, const I &i2, const I &ti1, const I &ti2, T xstart, T xend, T x)
{
	double h = xend - xstart;
	double t = (double)(x - xstart)/(xend - xstart);
	double t2 = t*t;
	double t3 = t2*t;

	// evaluate the hermite basis functions
	double h00 = 2*t3 - 3*t2 + 1;
	double h10 = t3 - 2*t2 + t;
	double h01 = -2*t3 + 3*t2;
	double h11 = t3 - t2;

	// calculate the interpolated value
	return h00*i1 + h10*h*ti1 + h01*i2 + h11*h*ti2;
}

#endif