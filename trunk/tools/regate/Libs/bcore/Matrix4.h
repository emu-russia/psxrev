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

#ifndef _MATRIX4_H45631_INCLUDED_
#define _MATRIX4_H45631_INCLUDED_

#pragma once

#include "Vector3.h"
#include "Vector4.h"
#include <math.h>

class Matrix4
{
public:
	double	m[4][4];

public:
	Matrix4() { }
	Matrix4(double* elems)
	{
		memcpy(m, elems, sizeof(double)*4*4);
	}
	Matrix4(const Matrix4& mat)
	{
		memcpy(m, (void*)mat.m, sizeof(double)*4*4);
	}

	// Vector multiplication operations
	Vector4 operator * (const Vector4& v)	{
		Vector4 out;
		out.x = (float)(v.x*m[0][0] + v.y*m[0][1] + v.z*m[0][2] + v.w*m[0][3]);
		out.y = (float)(v.x*m[1][0] + v.y*m[1][1] + v.z*m[1][2] + v.w*m[1][3]);
		out.z = (float)(v.x*m[2][0] + v.y*m[2][1] + v.z*m[2][2] + v.w*m[2][3]);
		out.w = (float)(v.x*m[3][0] + v.y*m[3][1] + v.z*m[3][2] + v.w*m[3][3]);	// no division
		return out;
	};
	
	Vector3 operator * (const Vector3& v)	{
		Vector3 out;
		out.x = (float)(v.x*m[0][0] + v.y*m[0][1] + v.z*m[0][2] + m[0][3]);
		out.y = (float)(v.x*m[1][0] + v.y*m[1][1] + v.z*m[1][2] + m[1][3]);
		out.z = (float)(v.x*m[2][0] + v.y*m[2][1] + v.z*m[2][2] + m[2][3]);
		return out;
	};

	Matrix4 operator * (const Matrix4& mat) {
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
	}

	Matrix4& operator *= (const Matrix4& mat) {
		Matrix4 res;
		for (int i=0; i<4; ++i)
			for (int j=0; j<4; ++j)
			{
				double val = 0;
				for (int k=0; k<4; ++k)
					val += m[i][k]*mat.m[k][j];
				res.m[i][j] = val;
			}
		(*this) = res;
		return (*this);
	}

	void setIdentity()
	{
		m[0][0] = 1; m[0][1] = 0; m[0][2] = 0; m[0][3] = 0;
		m[1][0] = 0; m[1][1] = 1; m[1][2] = 0; m[1][3] = 0;
		m[2][0] = 0; m[2][1] = 0; m[2][2] = 1; m[2][3] = 0;
		m[3][0] = 0; m[3][1] = 0; m[3][2] = 0; m[3][3] = 1;
	}

	void loadGLMatrix(int glMatrix)
	{
		glGetDoublev(glMatrix, (double*)m);
		transpose();
	}

	void transpose()
	{
		double tm[4][4];
		for (int i=0; i<4; ++i)
			for (int j=0; j<4; ++j)
			{
				tm[j][i] = m[i][j];
			}
		memcpy(m, (void*)tm, sizeof(double)*4*4);
	}

	void setRotX(float angle)
	{
		m[0][0] = 1; m[0][1] = 0; m[0][2] = 0; m[0][3] = 0;
		m[1][0] = 0; m[1][1] = cos(angle); m[1][2] = -sin(angle); m[1][3] = 0;
		m[2][0] = 0; m[2][1] = sin(angle); m[2][2] = cos(angle); m[2][3] = 0;
		m[3][0] = 0; m[3][1] = 0; m[3][2] = 0; m[3][3] = 1;
	}
	
	void setRotY(float angle)
	{
		m[0][0] = cos(angle); m[0][1] = 0; m[0][2] = sin(angle); m[0][3] = 0;
		m[1][0] = 0; m[1][1] = 1; m[1][2] = 0; m[1][3] = 0;
		m[2][0] = -sin(angle); m[2][1] = 0; m[2][2] = cos(angle); m[2][3] = 0;
		m[3][0] = 0; m[3][1] = 0; m[3][2] = 0; m[3][3] = 1;
	}
	
	void setRotZ(float angle)
	{
		m[0][0] = cos(angle); m[0][1] = -sin(angle); m[0][2] = 0; m[0][3] = 0;
		m[1][0] = sin(angle); m[1][1] = cos(angle); m[1][2] = 0; m[1][3] = 0;
		m[2][0] = 0; m[2][1] = 0; m[2][2] = 1; m[2][3] = 0;
		m[3][0] = 0; m[3][1] = 0; m[3][2] = 0; m[3][3] = 1;
	}

	Vector3 getTranslation() { return Vector3((float)m[0][3], (float)m[1][3], (float)m[2][3]); }
};

#endif