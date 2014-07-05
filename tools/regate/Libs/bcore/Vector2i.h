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

#ifndef _VECTOR2I_H45631_INCLUDED_
#define _VECTOR2I_H45631_INCLUDED_

#pragma once

#include <math.h>

class Vector2i
{
public:
	int x, y;

public:
	Vector2i() : x(0), y(0) {}
	Vector2i(int _x, int _y) : x(_x), y(_y) {}
	Vector2i(const Vector2i& v) : x(v.x), y(v.y) {}

	Vector2i& operator = (const Vector2i& v) { x=v.x; y=v.y; return *this; }
	
	// vector operators
			inline Vector2i&	operator += (const Vector2i& v)						{ x+=v.x; y+=v.y; return *this; };
			inline Vector2i&	operator -= (const Vector2i& v)						{ x-=v.x; y-=v.y; return *this; };
	friend	inline Vector2i		operator + (const Vector2i& v1, const Vector2i& v2)	{ return Vector2i(v1.x+v2.x, v1.y+v2.y); };
	friend	inline Vector2i		operator - (const Vector2i& v1, const Vector2i& v2)	{ return Vector2i(v1.x-v2.x, v1.y-v2.y); };
	
	inline double	length() const		{ return sqrt((double)(x*x + y*y)); }
	inline int		length2() const		{ return x*x + y*y; }
	
	// unary operators
	friend inline Vector2i	operator - (const Vector2i& v)	{ return Vector2i(-v.x, -v.y); }

	// scalar operators
	friend	inline Vector2i		operator * (const Vector2i& v, float f)	{ return Vector2i(int(v.x*f), int(v.y*f)); };
	friend	inline Vector2i		operator * (float f, const Vector2i& v)	{ return Vector2i(int(v.x*f), int(v.y*f)); };
	friend	inline Vector2i		operator / (const Vector2i& v, float f)	{ return Vector2i(int(v.x/f), int(v.y/f)); };
	friend	inline Vector2i		operator / (float f, const Vector2i& v)	{ return Vector2i(int(v.x/f), int(v.y/f)); };
			inline Vector2i&	operator *= (float f)					{ x=int(x*f); y=int(y*f); return *this; };
			inline Vector2i&	operator /= (float f)					{ x=int(x/f); y=int(y/f); return *this; };
	
	// products (not as operators, to avoid confusion)
	inline int		dot(const Vector2i& v) const			{ return x*v.x + y*v.y; }
	inline float	angle(const Vector2i& v) const		{ return acosf((float)(dot(v)/(length()*v.length()))); }
	inline float	angleUnitV(const Vector2i& v) const	{ return acosf((float)dot(v)); }	// WORKS ONLY ON UNIT VECTORS
};

#endif