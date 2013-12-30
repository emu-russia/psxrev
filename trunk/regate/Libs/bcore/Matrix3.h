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

#ifndef _MATRIX3_H45631_INCLUDED_
#define _MATRIX3_H45631_INCLUDED_

#pragma once

#include "Vector3.h"
#include "Vector4.h"
#include <math.h>

class Matrix3
{
public:
	double	m[3][3];

public:
	Matrix3() { }
	Matrix3(double* elems)
	{
		memcpy(m, elems, sizeof(double)*3*3);
	}
	Matrix3(const Matrix3& mat)
	{
		memcpy(m, (void*)mat.m, sizeof(double)*3*3);
	}

	// Vector multiplication operations
	Vector3 operator * (const Vector3& v)	{
		Vector3 out;
		out.x = (float)(v.x*m[0][0] + v.y*m[0][1] + v.z*m[0][2]);
		out.y = (float)(v.x*m[1][0] + v.y*m[1][1] + v.z*m[1][2]);
		out.z = (float)(v.x*m[2][0] + v.y*m[2][1] + v.z*m[2][2]);
		return out;
	};

	/*Matrix4 operator * (const Matrix4& mat) {
		Matrix4 res;
		for (int i=0; i<4; ++i)
			for (int j=0; j<4; ++j)
			{
				double val = 0;
				for (int k=0; k<4; ++k)
					val += m[i][k]*mat.m[k][j];
				res.m[i][j] = val;
			}
		return res;
	}*/

	void transpose()
	{
		double tm[3][3];
		for (int i=0; i<3; ++i)
			for (int j=0; j<3; ++j)
			{
				tm[j][i] = m[i][j];
			}
		memcpy(m, (void*)tm, sizeof(double)*3*3);
	}

};

#endif