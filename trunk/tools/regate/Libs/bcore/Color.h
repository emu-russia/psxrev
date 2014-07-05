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

#ifndef _COLOR_H2397_INCLUDED_
#define _COLOR_H2397_INCLUDED_

#pragma once

#include <math.h>

class Color
{
public:
	float r,g,b;

public:
	Color() : r(0), g(0), b(0) { };
	Color(float _x, float _y, float _z) : r(_x), g(_y), b(_z) { };
	Color(const Color& v) : r(v.r), g(v.g), b(v.b) { };
	Color(const unsigned char *c) : r((float)c[0]/255.0f), g((float)c[1]/255.0f), b((float)c[2]/255.0f) { }

	inline float	length() const		{ return sqrtf(r*r + g*g + b*b); }
	//inline float	length2() const		{ return r*r + g*g + b*b; }
	//inline void		normalize()			{ *this /= length(); }
	inline float	avg() const			{ return (r + g + b)/3.0f; }

	// products (not as operators, to avoid confusion)
	//inline float	dot(const Color& v)	{ return r*v.r + g*v.g + b*v.b; }
	//inline Color	cross(const Color& v)	{ return Color(g*v.b-v.g*b, b*v.r-v.b*r, r*v.g-v.r*g); }

	inline Color compProduct(const Color& cl) { return Color(r*cl.r, g*cl.g, b*cl.b); }
	inline void	clampTop() { if (r>1) r=1; if (g>1) g=1; if (b>1) b=1; }
	inline void	clampBottom() { if (r<0) r=0; if (g<0) g=0; if (b<0) b=0; }
	inline void clamp()		{ clampTop(); clampBottom(); }

	// unary operators
	friend inline Color	operator - (const Color& v)	{ return Color(-v.r, -v.g, -v.b); }

	// scalar operators
	friend	inline Color	operator * (const Color& v, float f)	{ return Color(v.r*f, v.g*f, v.b*f); };
	friend	inline Color	operator * (float f, const Color& v)	{ return Color(v.r*f, v.g*f, v.b*f); };
	friend	inline Color	operator / (const Color& v, float f)	{ return Color(v.r/f, v.g/f, v.b/f); };
	friend	inline Color	operator / (float f, const Color& v)	{ return Color(v.r/f, v.g/f, v.b/f); };
			inline Color&	operator *= (float f)					{ r*=f; g*=f; b*=f; return *this; };
			inline Color&	operator /= (float f)					{ r/=f; g/=f; b/=f; return *this; };

	// vector operators
			inline Color&	operator += (const Color& v)						{ r+=v.r; g+=v.g; b+=v.b; return *this; };
			inline Color&	operator -= (const Color& v)						{ r-=v.r; g-=v.g; b-=v.b; return *this; };
	friend	inline Color	operator + (const Color& v1, const Color& v2)	{ return Color(v1.r+v2.r, v1.g+v2.g, v1.b+v2.b); };
	friend	inline Color	operator - (const Color& v1, const Color& v2)	{ return Color(v1.r-v2.r, v1.g-v2.g, v1.b-v2.b); };

	// equality operators
	inline bool operator == (const Color& v)		{ return (v.r == r && v.g == g && v.b == b); }
	inline bool operator != (const Color& v)		{ return (v.r == r && v.g == g && v.b == b); }
	inline bool equalsTheta (const Color& v, float theta)		{ return ((fabs(v.r-r)<theta) && (fabs(v.g-g)<theta) && (fabs(v.b-b)<theta)); }

	// r,g,b values are from 0 to 1
	// h = [0,360], s = [0,1], v = [0,1]
	//		if s == 0, then h = 0 //changed from -1 (undefined)
	inline void toHSV(float *h, float *s, float *v )
	{
		float min, max, delta;
		min = (r<g)?r:g;
		min = (min<b)?min:b;//MIN( r, g, b );
		max = (r>g)?r:g;
		max = (max>b)?max:b;//MAX( r, g, b );
		*v = max;				// v
		delta = max - min;
		if( max != 0 )
			*s = delta / max;		// s
		else {
			*s = 0;
			*h = 0;	//-1;
			return;
		}
		if( r == max )
			*h = ( g - b ) / delta;		// between yellow & magenta
		else if( g == max )
			*h = 2 + ( b - r ) / delta;	// between cyan & yellow
		else
			*h = 4 + ( r - g ) / delta;	// between magenta & cyan
		*h *= 60;				// degrees
		if( *h < 0 )
			*h += 360;
	}

	static inline Color fromHSV(float h, float s, float v)
	{
		Color c;
		int i;
		float f, p, q, t;

		if( s == 0 ) {
			// achromatic (grey)
			c.r = c.g = c.b = v;
			return c;
		}

		h /= 60;			// sector 0 to 5
		i = (int)floor(h);
		f = h - i;			// factorial part of h
		p = v * ( 1 - s );
		q = v * ( 1 - s * f );
		t = v * ( 1 - s * ( 1 - f ) );

		switch( i ) {
			case 0:
				c.r = v;
				c.g = t;
				c.b = p;
				break;
			case 1:
				c.r = q;
				c.g = v;
				c.b = p;
				break;
			case 2:
				c.r = p;
				c.g = v;
				c.b = t;
				break;
			case 3:
				c.r = p;
				c.g = q;
				c.b = v;
				break;
			case 4:
				c.r = t;
				c.g = p;
				c.b = v;
				break;
			case 5:		// case 5:
			default:
				c.r = v;
				c.g = p;
				c.b = q;
				break;
		//	default:
		//		ASSERT(0);
		}
		return c;
	}
};

#endif