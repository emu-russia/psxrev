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

/*
 *  Simple trackball-like motion adapted (ripped off) from projtex.c
 *  (written by David Yu and David Blythe).  See the SIGGRAPH '96
 *  Advanced OpenGL course notes.
 */


/* includes */
#include <math.h>
#include "trackball.h"
#include <ctime>


/* globals */
static GLuint    tb_lasttime;
static GLfloat   tb_lastposition[3];

static GLfloat   tb_angle = 0.0;
static GLfloat   tb_axis[3];
static GLfloat   tb_transform[4][4];

static GLuint    tb_width;
static GLuint    tb_height;

static GLboolean tb_tracking = GL_FALSE;
static GLboolean tb_animate = GL_TRUE;


/* functions */
static void _tbPointToVector(int x, int y, int width, int height, float v[3])
{
	float d, a;

	/* project x, y onto a hemi-sphere centered within width, height. */
	v[0] = float((2.0f * x - width) / width);
	v[1] = float((height - 2.0f * y) / height);
	d = sqrtf(v[0] * v[0] + v[1] * v[1]);
	v[2] = cosf((3.14159265f / 2.0f) * ((d < 1.0f) ? d : 1.0f));
	a = 1.0f / sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
	v[0] *= a;
	v[1] *= a;
	v[2] *= a;
}

void _tbStartMotion(int x, int y, int button, int time)
{
	tb_tracking = GL_TRUE;
	tb_lasttime = time;
	_tbPointToVector(x, y, tb_width, tb_height, tb_lastposition);
}

void _tbStopMotion(int button, unsigned time)
{
	/*assert(tb_button != -1);

	tb_tracking = GL_FALSE;

	if (time == tb_lasttime && tb_animate) {
		glutIdleFunc(_tbAnimate);
	}
	else {
		tb_angle = 0.0;
		if (tb_animate)
			glutIdleFunc(0);
	}*/

	tb_angle = 0;
}

void Trackball::animate(GLboolean animate)
{
	tb_animate = animate;
}

void Trackball::init()
{
	tb_angle = 0.0;

	/* put the identity in the trackball transform */
	glPushMatrix();
	glLoadIdentity();
	glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat *)tb_transform);
	glPopMatrix();
}

void Trackball::setMatrix()
{
	glPushMatrix();
	glLoadIdentity();
	glRotatef(tb_angle, tb_axis[0], tb_axis[1], tb_axis[2]);
	glMultMatrixf((GLfloat *)tb_transform);
	glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat *)tb_transform);
	glPopMatrix();

	glMultMatrixf((GLfloat *)tb_transform);
	

	tb_angle = 0;
}

void Trackball::reshape(int width, int height)
{
	tb_width  = width;
	tb_height = height;
}

void Trackball::mouseDown(int x, int y)
{
	int button = 1;//TEMP
	_tbStartMotion(x, y, button, (int)(1000*clock()/CLOCKS_PER_SEC));
}

void Trackball::mouseUp(int x, int y)
{
	int button = 1;//TEMP
	_tbStopMotion(button, (int)(1000*clock()/CLOCKS_PER_SEC));
}

void Trackball::motion(int x, int y)
{
	GLfloat current_position[3], dx, dy, dz;

	if (tb_tracking == GL_FALSE)
		return;

	_tbPointToVector(x, y, tb_width, tb_height, current_position);

	/* calculate the angle to rotate by (directly proportional to the
		length of the mouse movement */
	dx = current_position[0] - tb_lastposition[0];
	dy = current_position[1] - tb_lastposition[1];
	dz = current_position[2] - tb_lastposition[2];
	tb_angle = 90.0f * sqrtf(dx * dx + dy * dy + dz * dz);

	/* calculate the axis of rotation (cross product) */
	tb_axis[0] = tb_lastposition[1] * current_position[2] - 
				tb_lastposition[2] * current_position[1];
	tb_axis[1] = tb_lastposition[2] * current_position[0] - 
				tb_lastposition[0] * current_position[2];
	tb_axis[2] = tb_lastposition[0] * current_position[1] - 
				tb_lastposition[1] * current_position[0];

	/* reset for next time */
	tb_lasttime = (int)(1000*clock()/CLOCKS_PER_SEC);
	tb_lastposition[0] = current_position[0];
	tb_lastposition[1] = current_position[1];
	tb_lastposition[2] = current_position[2];

	/* remember to draw new position */
	//glutPostRedisplay();
}
