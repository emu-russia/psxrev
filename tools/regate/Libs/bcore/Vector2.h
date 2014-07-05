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

#ifndef _VECTOR2_H45631_INCLUDED_
#define _VECTOR2_H45631_INCLUDED_

#pragma once

#include <math.h>

class Vector2
{
public:
	float x, y;

public:
	Vector2() : x(0), y(0) { };
	Vector2(float _x, float _y) : x(_x), y(_y) { }
	Vector2(const Vector2& v) : x(v.x), y(v.y) { };
	
	inline float	length() const		{ return sqrtf(x*x + y*y); }
	inline float	length2() const		{ return x*x + y*y; }
	inline void		normalize()			{ *this /= length(); }
	
	// products (not as operators, to avoid confusion)
	inline float	dot(const Vector2& v) const			{ return x*v.x + y*v.y; }
	inline float	angle(const Vector2& v) const		{ return acosf(dot(v)/(length()*v.length())); }
	inline float	angleUnitV(const Vector2& v) const	{ return acosf(dot(v)); }	// WORKS ONLY ON UNIT VECTORS

	// unary operators
	friend inline Vector2	operator - (const Vector2& v)	{ return Vector2(-v.x, -v.y); }

	// scalar operators
	friend	inline Vector2	operator * (const Vector2& v, float f)	{ return Vector2(v.x*f, v.y*f); };
	friend	inline Vector2	operator * (float f, const Vector2& v)	{ return Vector2(v.x*f, v.y*f); };
	friend	inline Vector2	operator / (const Vector2& v, float f)	{ return Vector2(v.x/f, v.y/f); };
	friend	inline Vector2	operator / (float f, const Vector2& v)	{ return Vector2(v.x/f, v.y/f); };
			inline Vector2&	operator *= (float f)					{ x*=f; y*=f; return *this; };
			inline Vector2&	operator /= (float f)					{ x/=f; y/=f; return *this; };

	// vector operators
			inline Vector2&	operator += (const Vector2& v)						{ x+=v.x; y+=v.y; return *this; };
			inline Vector2&	operator -= (const Vector2& v)						{ x-=v.x; y-=v.y; return *this; };
	friend	inline Vector2	operator + (const Vector2& v1, const Vector2& v2)	{ return Vector2(v1.x+v2.x, v1.y+v2.y); };
	friend	inline Vector2	operator - (const Vector2& v1, const Vector2& v2)	{ return Vector2(v1.x-v2.x, v1.y-v2.y); };

	// equality operators
	inline bool operator == (const Vector2& v)		{ return (v.x == x && v.y == y); }
	inline bool operator != (const Vector2& v)		{ return (v.x == x && v.y == y); }
	inline bool equalsTheta (const Vector2& v, float theta)		{ return ((fabs(v.x-x)<theta) && (fabs(v.y-y)<theta)); }
};

#endif