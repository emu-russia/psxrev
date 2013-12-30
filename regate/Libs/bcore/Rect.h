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

#ifndef _RECT_H45631_INCLUDED_
#define _RECT_H45631_INCLUDED_

#pragma once

#include "misc.h"

template <class T>
class Rect
{
public:
	T left, right, top, bottom;

	Rect() { };
	Rect(const T& _left, const T& _top, const T& _right, const T& _bottom) : left(_left), 
							right(_right), top(_top), bottom(_bottom) { }
	inline T getWidth() const	{ return right - left; }
	inline T getHeight() const	{ return bottom - top; }

	inline bool contains(T x, T y) const {
		if (x<left || x>right || y<top || y>bottom)
			return false;
		return true;
	}
};

#endif