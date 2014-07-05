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

#ifndef _MISC_H45631_INCLUDED_
#define _MISC_H45631_INCLUDED_

#pragma once

#include "common.h"

// get the maximum element of an array
template <class T>
unsigned int max_elem_id(T* elems, unsigned int nElems)
{
	if (nElems < 1)
		return 0;
	unsigned int max_e_id = 0;
	T *max_e = &elems[0];
	for (unsigned int i=0; i<nElems; ++i)
		if (elems[i] > *max_e) {
			max_e = &elems[i];
			max_e_id = i;
		}
	return max_e_id;
}

template <class T>
T clamp(const T& val, const T& vmin, const T& vmax)
{
	if (val < vmin)
		return vmin;
	if (val > vmax)
		return vmax;
	return val;
}

template <class T>
inline int round(T x) {
	if (x-(int)x >= 0.5)
		return (int)x+1;
	return (int)x;
}

__forceinline bool pt_in_rect(int x, int y, int left, int top, int right, int bottom)
{
	if (x<left || x>right || y<top || y>bottom)
		return false;
	return true;
}

#endif