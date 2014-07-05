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

#include "Viewport.h"

Viewport::Viewport() : m_zoom(1),
	m_projType(Viewport::ISOMETRIC),
	m_x(0), m_y(0), m_width(0), m_height(0),
	m_ncp(0),
	m_fcp(1.0f),
	m_fov(60.0f),
	m_aspectRatio(1),
	m_orthoScale(1)
{
}

void Viewport::setDimensions(int x, int y, int w, int h)
{
	m_x = x;
	m_y = y;
	m_width = w;
	m_height = h;
}

void Viewport::setOrthoProj(float ncp, float fcp, float scale, float aspectRatio)
{
	m_projType = Viewport::ISOMETRIC;
	m_ncp = ncp;
	m_fcp = fcp;
	m_orthoScale = scale;
	m_aspectRatio = aspectRatio;
}

void Viewport::setPerspectiveProj(float ncp, float fcp, float fov, float aspectRatio)
{
	m_projType = Viewport::PERSPECTIVE;
	m_ncp = ncp;
	m_fcp = fcp;
	m_fov = fov;
	m_aspectRatio = aspectRatio;
}

void Viewport::set()
{
	// set the projection matrix
	if (m_projType == Viewport::ISOMETRIC)
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(m_x, m_y, m_width*m_orthoScale*m_aspectRatio, m_height*m_orthoScale, m_ncp, m_fcp);
	}
	else if (m_projType == Viewport::PERSPECTIVE)
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(m_fov, m_aspectRatio, m_ncp, m_fcp);
	}
	else
		Console::print("ERROR: (Viewport::set) Unknown projection type!\n");

	// set the opengl viewport
	glViewport(m_x, m_y, m_width, m_height);
}

void Viewport::setLookAtBox(const BBox& box, const Vector3& up)
{
	Vector3 bboxSz = box.getSize();
	Vector3 bboxCt = box.getCenter();
	float maxDim = (bboxSz.x > bboxSz.y)?bboxSz.x:bboxSz.y;
	maxDim = (maxDim > bboxSz.z)?maxDim:bboxSz.z;

	// set the view matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(bboxCt.x, bboxCt.y, bboxCt.z-3*maxDim, bboxCt.x, bboxCt.y, bboxCt.z, up.x, up.y, up.z);

	m_zoom = 1.0f;
	m_pan = Vector2(0,0);
}

void Viewport::zoom(float f)
{
	m_zoom *= 1+f;
}

void Viewport::pan(const Vector2& t)
{
	m_pan += t;
}

void Viewport::rotate(const Vector2& r)
{
}

void Viewport::setMatrix()
{
	glTranslatef(m_pan.x, m_pan.y, 0);
	glScalef(m_zoom, m_zoom, m_zoom);
}