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

#ifndef _COMMON_H24688_INCLUDED_
#define _COMMON_H24688_INCLUDED_

#pragma once

/**
 * Common includes, macros, preproc directives and typedefs 
 * used throughout the program
 *
 */

//#define _CRT_SECURE_NO_WARNINGS
#undef _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES
#define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES 1

#define SAFE_DELETE(x) { if (x) { delete x; x=0; } }
#define SAFE_DELETE_VEC(x) { if (x) { delete[] x; x=0; } }
#ifndef ASSERT
	#define ASSERT(x)	{ assert(x); }
#endif

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

/**
 * OpenGL-relevant includes (opengl, glu32, glaux, glut, glui)
 */
#include <GL/glew.h>
#include <GL/wglew.h>

//#include <gl\gl.h>	// opengl
//#include <gl\glu.h>	// glu32
//#include "..\gl\glaux.h"		// DEPRECATED! REMOVE! (also from project directory) - used in Image.cpp for loading bmps
//#include <gl\glut.h>
//#include <gl\glui.h>
//#include <GL/glext.h>

/**
 * Some useful STL types
 */
#include <vector>
#include <list>
#include <string>

/**
 * Some basic 3d structs
 */
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Color.h"
#include "Matrix4.h"
#include "Matrix3.h"

/**
 * A console
 */
#include "Console.h"

/**
 * Constants
 */
#define PI 3.14159f

// define integer types with size guarantee
typedef unsigned __int8 uint8_t;
typedef unsigned __int16 uint16_t;
typedef unsigned __int32 uint32_t;
typedef __int8 int8_t;
typedef __int16 int16_t;
typedef __int32 int32_t;

// define float types with size guarantee (?)
typedef float float32_t;
typedef double float64_t;

/**
 * Some useful enums
 */
enum Axis {
	X_AXIS = 0,
	Y_AXIS,
	Z_AXIS
};

enum TexGenMode {
	TG_NORMAL,		// object normal
	TG_ISNORMAL,	// intermediate surface normal
	TG_OBJCENTROID,	// object centroid
	TG_UV,			// ready made uv's
	TG_SPHERICAL_ENVMAP,	// spherical environment map
	TG_CUBIC_ENVMAP			// cubic environment map
};

enum TexGenPrimitive {
	TG_SPHERE,
	TG_CYLINDER,
	TG_BOX,
	TG_PLANE
};

#endif