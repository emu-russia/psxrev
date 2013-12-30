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

#ifndef _VECTOR4_H45631_INCLUDED_
#define _VECTOR4_H45631_INCLUDED_

#pragma once

class Vector4
{
public:
	float x,y,z,w;

public:
	Vector4() : x(0), y(0), z(0), w(1) { };
	Vector4(float _x, float _y, float _z, float _w=1) : x(_x), y(_y), z(_z), w(_w) { };
	Vector4(const Vector3& v) : x(v.x), y(v.y), z(v.z), w(1) { };
	Vector4(const Vector4& v) : x(v.x), y(v.y), z(v.z), w(v.w) { };
	
	inline float	length() const		{ return sqrtf(x*x + y*y + z*z + w*w); }
	inline float	length2() const		{ return x*x + y*y + z*z + w*w; }
	inline void		normalize()			{ *this /= length(); }
	
	// unary operators
	friend inline Vector4	operator - (const Vector4& v)	{ return Vector4(-v.x, -v.y, -v.z, -v.w); }

	// scalar operators
	friend	inline Vector4	operator * (const Vector4& v, float f)	{ return Vector4(v.x*f, v.y*f, v.z*f, v.w*f); };
	friend	inline Vector4	operator * (float f, const Vector4& v)	{ return Vector4(v.x*f, v.y*f, v.z*f, v.w*f); };
	friend	inline Vector4	operator / (const Vector4& v, float f)	{ return Vector4(v.x/f, v.y/f, v.z/f, v.w/f); };
	friend	inline Vector4	operator / (float f, const Vector4& v)	{ return Vector4(v.x/f, v.y/f, v.z/f, v.w/f); };
			inline Vector4&	operator *= (float f)					{ x*=f; y*=f; z*=f; w*=f; return *this; };
			inline Vector4&	operator /= (float f)					{ x/=f; y/=f; z/=f; w/=f; return *this; };
			
	// vector operators
			inline Vector4&	operator += (const Vector4& v)						{ x+=v.x; y+=v.y; z+=v.z; w+=v.w; return *this; };
			inline Vector4&	operator -= (const Vector4& v)						{ x-=v.x; y-=v.y; z-=v.z; w-=v.w; return *this; };
	friend	inline Vector4	operator + (const Vector4& v1, const Vector4& v2)	{ return Vector4(v1.x+v2.x, v1.y+v2.y, v1.z+v2.z, v1.w+v2.w); };
	friend	inline Vector4	operator - (const Vector4& v1, const Vector4& v2)	{ return Vector4(v1.x-v2.x, v1.y-v2.y, v1.z-v2.z, v1.w-v2.w); };

	// equality operators
	inline bool operator == (const Vector4& v)		{ return (v.x == x && v.y == y && v.z == z && v.w == w); }
	inline bool operator != (const Vector4& v)		{ return (v.x == x && v.y == y && v.z == z && v.w == w); }
	inline bool equalsTheta (const Vector4& v, float theta)		{ return ((fabs(v.x-x)<theta) && (fabs(v.y-y)<theta) && (fabs(v.z-z)<theta) && (fabs(v.w-w)<theta)); }
};

#endif