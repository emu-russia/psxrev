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

#ifndef _VECTOR3_H45631_INCLUDED_
#define _VECTOR3_H45631_INCLUDED_

#pragma once

#include <math.h>

class Vector3
{
public:
	float x,y,z;

public:
	Vector3() : x(0), y(0), z(0) { };
	Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) { };
	Vector3(const Vector3& v) : x(v.x), y(v.y), z(v.z) { };

	inline float	length() const		{ return sqrtf(x*x + y*y + z*z); }
	inline float	length2() const		{ return x*x + y*y + z*z; }
	inline void		normalize()			{ *this /= length(); }

	// products (not as operators, to avoid confusion)
	inline float	dot(const Vector3& v) const			{ return x*v.x + y*v.y + z*v.z; }
	inline Vector3	cross(const Vector3& v) const		{ return Vector3(y*v.z-v.y*z, z*v.x-v.z*x, x*v.y-v.x*y); }
	inline float	angle(const Vector3& v) const		{ return acosf(dot(v)/(length()*v.length())); }
	inline float	angleUnitV(const Vector3& v) const	{ return acosf(dot(v)); }	// WORKS ONLY ON UNIT VECTORS

	// unary operators
	friend inline Vector3	operator - (const Vector3& v)	{ return Vector3(-v.x, -v.y, -v.z); }

	// scalar operators
	friend	inline Vector3	operator * (const Vector3& v, float f)	{ return Vector3(v.x*f, v.y*f, v.z*f); };
	friend	inline Vector3	operator * (float f, const Vector3& v)	{ return Vector3(v.x*f, v.y*f, v.z*f); };
	friend	inline Vector3	operator / (const Vector3& v, float f)	{ return Vector3(v.x/f, v.y/f, v.z/f); };
	friend	inline Vector3	operator / (float f, const Vector3& v)	{ return Vector3(v.x/f, v.y/f, v.z/f); };
			inline Vector3&	operator *= (float f)					{ x*=f; y*=f; z*=f; return *this; };
			inline Vector3&	operator /= (float f)					{ x/=f; y/=f; z/=f; return *this; };

	// vector operators
			inline Vector3&	operator += (const Vector3& v)						{ x+=v.x; y+=v.y; z+=v.z; return *this; };
			inline Vector3&	operator -= (const Vector3& v)						{ x-=v.x; y-=v.y; z-=v.z; return *this; };
	friend	inline Vector3	operator + (const Vector3& v1, const Vector3& v2)	{ return Vector3(v1.x+v2.x, v1.y+v2.y, v1.z+v2.z); };
	friend	inline Vector3	operator - (const Vector3& v1, const Vector3& v2)	{ return Vector3(v1.x-v2.x, v1.y-v2.y, v1.z-v2.z); };

	// equality operators
	inline bool operator == (const Vector3& v)		{ return (v.x == x && v.y == y && v.z == z); }
	inline bool operator != (const Vector3& v)		{ return (v.x == x && v.y == y && v.z == z); }
	inline bool equalsTheta (const Vector3& v, float theta)		{ return ((fabs(v.x-x)<theta) && (fabs(v.y-y)<theta) && (fabs(v.z-z)<theta)); }
};

#endif